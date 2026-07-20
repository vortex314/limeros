#include "mc_actor.h"

constexpr int MULTICAST_PORT = 50000;
constexpr const char *MULTICAST_GROUP = "239.0.0.1";
constexpr int UNICAST_PORT = 50001;
constexpr int BUF_SIZE = 1024;
constexpr const char *BROKER_IP = "192.168.0.148";

std::string socketAddrToString(struct sockaddr_in *addr)
{
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), ip_str, INET_ADDRSTRLEN);

    // ntohs converts "Network To Host Short" (fixes byte order)
    int port = ntohs(addr->sin_port);

    return std::string(ip_str) + ":" + std::to_string(port);
}

McActor::McActor(const char *name, const char *hostname)
    : Actor(name)
{
    _hostname = std::string(hostname);
    _timer_publish = timer_repetitive(1000);
}

McActor::~McActor()
{
}

void McActor::on_start()
{
    INFO("UDP Actor started");
}

int validate_rc(int rc, const char *msg)
{
    if (rc < 0)
    {
        ERROR("'%s' failed: [%d] %s", msg, rc, strerror(errno));
        return rc;
    }
    return 0;
}

void McActor::init_event()
{
    // create multicast socket
    _unicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (validate_rc(_unicast_socket, "unicast socket"))
        return;
    _multicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (validate_rc(_multicast_socket, "multicast socket"))
        return;

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000; // 1ms timeout
    setsockopt(_unicast_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    //  fcntl(_unicast_socket, F_SETFL, O_NONBLOCK);

    memset(&_unicast_src_addr, 0, sizeof(_unicast_src_addr));
    _unicast_src_addr.sin_family = AF_INET;
    _unicast_src_addr.sin_port = htons(UNICAST_PORT);
    _unicast_src_addr.sin_addr.s_addr = inet_addr(BROKER_IP);

    memset(&_multicast_addr, 0, sizeof(_multicast_addr));
    _multicast_addr.sin_family = AF_INET;
    _multicast_addr.sin_port = htons(MULTICAST_PORT);
    _multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
    setsockopt(_multicast_socket, IPPROTO_IP, IP_MULTICAST_IF, &_multicast_addr.sin_addr, sizeof(_multicast_addr.sin_addr));

    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = 0; // any port

    int rc = bind(_unicast_socket, (sockaddr *)&local, sizeof(local));
    validate_rc(rc, "unicast bind");
    //   rc = bind(_event_socket, (sockaddr *)&local, sizeof(local));
    //   validate_rc(rc, "event bind");
    // Bind multicast socket to receive on multicast port and join group
    sockaddr_in mcast_bind{};
    mcast_bind.sin_family = AF_INET;
    mcast_bind.sin_addr.s_addr = INADDR_ANY;
    mcast_bind.sin_port = htons(MULTICAST_PORT);

    int reuse = 1;
    setsockopt(_multicast_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    rc = bind(_multicast_socket, (sockaddr *)&mcast_bind, sizeof(mcast_bind));
    validate_rc(rc, "multicast bind");

    struct ip_mreq imreq;
    imreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
    imreq.imr_interface.s_addr = htonl(INADDR_ANY); // default interface
    rc = setsockopt(_multicast_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imreq, sizeof(imreq));
    validate_rc(rc, "join multicast group");

    unsigned char loop = 0;
    setsockopt(_multicast_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

    // Wait for broker discovery via DeviceAliveEvent before sending unicast traffic.
    _broker_addr.reset();
    _broker_name.reset();
}

void McActor::stop_event()
{
    if (_multicast_socket >= 0)
    {
        close(_multicast_socket);
        _multicast_socket = -1;
    }
}

Result<Bytes> McActor::encode_message(uint32_t dst, uint32_t src, uint32_t type, const Bytes &payload)
{
    Envelope msg;
    Bytes buffer;
    msg.dst = dst;
    msg.src = src;
    msg.msg_type = type;
    msg.payload = payload;
    CborEncoder encoder;
    cbor_encoder_init(&encoder, nullptr, 0, 0);
    return msg.encode(buffer);
}

void McActor::send_unicast(Envelope &envelope)
{
    if (!_connected)
    {
        ERROR("Cannot send unicast message, not connected to WiFi");
        return;
    }
    if (!_broker_addr.has_value())
    {
        ERROR("Cannot send unicast message, broker address not discovered yet");
        return;
    }

    sockaddr_in dst_addr{};
    dst_addr = *_broker_addr;
    Bytes buffer(1000);
    if (envelope.encode(buffer) == 0)
    {
        DEBUG("Send message_type '%s' to %s @ %s", envelope.msg_type.has_value() ? std::to_string(*(envelope.msg_type)).c_str() : "-", socketAddrToString(&dst_addr).c_str(), socketAddrToString(&dst_addr).c_str());

        int sent = sendto(_unicast_socket, buffer.data(), buffer.size(), 0, (sockaddr *)&dst_addr, sizeof(dst_addr));
        if (sent < 0)
        {
            ERROR("Failed to send unicast message to %s @ %s: %s (errno=%d, %u bytes)",
                  "broker",
                  socketAddrToString(&dst_addr).c_str(),
                  strerror(errno),
                  errno,
                  static_cast<unsigned>(buffer.size()));
        }
        else
        {
            DEBUG("Sent message to %s (%d bytes)", socketAddrToString(&dst_addr).c_str(), sent);
        }
    }
}

void McActor::stop_listener()
{
    _running = false;
}

static Bytes mc_buffer(1000);

void McActor::on_message(const ActorMessage &msg)
{
    msg.handle_if<EndpointAnnounce>([&](const EndpointAnnounce &msg)
                                    {
        msg.services.for_each([&](const std::vector<uint32_t> &services)
        {
             for (auto v : services) 
                _services.push_back(v);
        });
        msg.events.for_each([&](const std::vector<uint32_t> &events)
        {
            for (auto v : events)
                _events.push_back(v);
        });
        msg.subscribes.for_each([&](const std::vector<uint32_t> &subscribes)
        {
            for (auto v : subscribes)
                _subscribes.push_back(v);
        }); });
    msg.handle_if<TimerMsg>([&](const TimerMsg &msg)
                            {
        EndpointAnnounce announce;
        announce.id = endpoint_id;
        announce.name = endpoint_name;
        if (announce.encode(mc_buffer) == 0)
        {
            Envelope env;
            env.src = endpoint_id;
            env.msg_type = EndpointAnnounce::msg_id();
            env.payload = mc_buffer;
            send_multicast(env);
        } });

    msg.handle_if<WifiConnected>([&](const WifiConnected &msg)
                                 { _connected = true; 
                                init_event();
                                start_unicast_listener();
                                start_multicast_listener(); });

    msg.handle_if<WifiDisconnected>([&](const auto &msg) { /*stop_listener(); */ });

    if (!_connected) // ignore other messages if not connected
        return;
    msg.handle_if<Unicast>([&](const Unicast &pub)
                           { send_unicast(pub.env); });
}

void McActor::start_unicast_listener()
{
    xTaskCreatePinnedToCore(unicast_listener_task, "udp_listener_task", 8192, this, 5, NULL, 1);
}

void McActor::unicast_listener_task(void *pvParameters)
{
    McActor *actor = static_cast<McActor *>(pvParameters);
    while (true)
    {
        char buf[BUF_SIZE];
        while (actor->_running)
        {
            sockaddr_in sender{};
            socklen_t sender_len = sizeof(sender);
            int len = recvfrom(actor->_unicast_socket, buf, BUF_SIZE - 1, 0, (sockaddr *)&sender, &sender_len);
            if (len < 0)
            {
                ERROR("recvfrom failed: %s", strerror(errno));

                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                    continue;
                }
                ERROR("recvfrom failed: %s", strerror(errno));
                continue;
            }
            actor->on_udp_raw(Bytes(buf, buf + len), sender);
        }
    }
}

void McActor::start_multicast_listener()
{
    xTaskCreatePinnedToCore(multicast_listener_task, "udp_listener_task", 8192, this, 5, NULL, 1);
}

void McActor::multicast_listener_task(void *pvParameters)
{
    McActor *actor = static_cast<McActor *>(pvParameters);
    while (true)
    {
        char buf[BUF_SIZE];
        while (actor->_running)
        {
            sockaddr_in sender{};
            socklen_t sender_len = sizeof(sender);
            int len = recvfrom(actor->_multicast_socket, buf, BUF_SIZE - 1, 0, (sockaddr *)&sender, &sender_len);
            if (len < 0)
            {
                ERROR("recvfrom failed: %s", strerror(errno));

                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                    continue;
                }
                ERROR("recvfrom failed: %s", strerror(errno));
                continue;
            }
            DEBUG("MC Actor received multicast message (%d bytes)", len);
            actor->on_udp_raw(Bytes(buf, buf + len), sender);
        }
    }
}

void McActor::on_udp_raw(const Bytes &request, const sockaddr_in &sender_addr)
{
    Envelope *envelope = new Envelope();
    if (envelope->decode(request) == 0)
    {
        on_udp_message(envelope, sender_addr);
    }
    else
    {
        ERROR("Failed to decode Envelope from UDP message");
    }
    delete envelope;
}

void McActor::on_udp_message(Envelope *env, const sockaddr_in &sender_addr)
{
    if (!env->msg_type || !env->payload)
    {
        ERROR("UdpMessage missing fields: typ=%s msg=%s",
              env->msg_type ? "yes" : "no",
              env->payload ? "yes" : "no");
        return;
    }
    if (env->msg_type == PingReply::msg_id())
    {
        send_ping_req(env->src.value(), ++_last_ping_number);
    }
    else
        emit(env);
}

void McActor::send_multicast(const char *dst, const char *src, const char *type, const Bytes &payload)
{
    if (!_connected)
    {
        ERROR("Cannot send multicast message, not connected to WiFi");
        return;
    }
    //   INFO("send_multicast %s <= %s : %s = %s ", dst ? dst : "-", src ? src : "-", type, std::string(payload.begin(), payload.end()).c_str());
    encode_message(dst, src, type, payload).just([&](const Bytes &buffer)
                                                 {  
    int sent = sendto(_unicast_socket, buffer.data(), buffer.size(), 0, (sockaddr *)&_multicast_addr, sizeof(_multicast_addr));
    if (sent < 0)
    {
        ERROR("Multicast failed: %s [%lu]", strerror(errno), buffer.size());
    } });
}

void McActor::send_ping_req(const char *dst, uint32_t number)
{
    static int64_t start_time = esp_timer_get_time();
    static uint32_t counter = 0;
    PingRequest ping;
    ping.number = number;
    //    INFO("MC Actor sending Ping %d to %s", number, dst ? dst : "broker");
    send_unicast(dst, _hostname.c_str(), ping.type_name(), PingRequest::to_poly(ping).unwrap());
    counter++;
    if (counter % 1000 == 0)
    {
        int64_t now = esp_timer_get_time();
        INFO("ping %f msg/sec", (counter * 10000000.0) / (now - start_time));
        start_time = now;
        counter = 0;
    }
}

void McActor::send_ping_rep(const char *dst, uint32_t number)
{
    PingReply pong;
    pong.number = number;
    auto b = PingReply::to_poly(pong);
    //  INFO("Msg Ping resp %d to %s", b.is_ok() ? b.unwrap().size() : -1, dst ? dst : "broker");
    send_unicast(dst, _hostname.c_str(), pong.type_name(), PingReply::to_poly(pong).unwrap());
}
