#include "mc_actor.h"

constexpr int MULTICAST_PORT = 50000;
constexpr const char *MULTICAST_GROUP = "224.0.0.1";
constexpr int UNICAST_PORT = 50001;
constexpr int BUF_SIZE = 1024;
constexpr const char *BROKER_IP = "192.168.0.148";

std::string socketAddrToString(const struct sockaddr_in *addr)
{
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), ip_str, INET_ADDRSTRLEN);
    // ntohs converts "Network To Host Short" (fixes byte order)
    int port = ntohs(addr->sin_port);
    return std::string(ip_str) + ":" + std::to_string(port);
}

McActor::McActor(const char *name, const char *hostname)
    : Actor(name), _txd_unicast_buffer(1000), _rxd_unicast_buffer(1000), _txd_multicast_buffer(1000), _rxd_multicast_buffer(1000)
{
    _hostname = std::string(hostname);
    _timer_publish = timer_repetitive(1000);
    endpoint_name = "hoverboard";
    endpoint_id = FNV("hoverboard");
}

McActor::~McActor()
{
}

void McActor::on_start()
{
    INFO("UDP Actor started");
}

#define ERRNO_RET(rc)                                                             \
    if (rc < 0)                                                                   \
    {                                                                             \
        ERROR("Error: %s failed with errno %d: %s", #rc, errno, strerror(errno)); \
        return;                                                                   \
    }
bool valid_rc(int rc, const char *msg)
{
    if (rc < 0)
    {
        ERROR("'%s' failed: [%d] %s", msg, rc, strerror(errno));
        return false;
    }
    return true;
}

void McActor::init_event()
{
    // create multicast socket
    _unicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    ERRNO_RET(_unicast_socket);

    _multicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    ERRNO_RET(_multicast_socket);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000; // 1ms timeout
    setsockopt(_unicast_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    memset(&_multicast_addr, 0, sizeof(_multicast_addr));
    _multicast_addr.sin_family = AF_INET;
    _multicast_addr.sin_port = htons(MULTICAST_PORT);
    _multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
    setsockopt(_multicast_socket, IPPROTO_IP, IP_MULTICAST_IF, &_multicast_addr.sin_addr, sizeof(_multicast_addr.sin_addr));

    sockaddr_in _unicast_src_addr{};
    _unicast_src_addr.sin_family = AF_INET;
    _unicast_src_addr.sin_addr.s_addr = INADDR_ANY;
    _unicast_src_addr.sin_port = htons(UNICAST_PORT); // any port

    int rc = bind(_unicast_socket, (sockaddr *)&_unicast_src_addr, sizeof(_unicast_src_addr));
    ERRNO_RET(rc);

    sockaddr_in mcast_bind{};
    mcast_bind.sin_family = AF_INET;
    mcast_bind.sin_addr.s_addr = INADDR_ANY;
    mcast_bind.sin_port = htons(MULTICAST_PORT);

    int reuse = 1;
    setsockopt(_multicast_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    rc = bind(_multicast_socket, (sockaddr *)&mcast_bind, sizeof(mcast_bind));
    ERRNO_RET(rc);

    struct ip_mreq imreq;
    imreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
    imreq.imr_interface.s_addr = htonl(INADDR_ANY); // default interface
    rc = setsockopt(_multicast_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imreq, sizeof(imreq));
    ERRNO_RET(rc);

    unsigned char loop = 0;
    setsockopt(_multicast_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

    // Wait for broker discovery via DeviceAliveEvent before sending unicast traffic.
    _broker_addr.none();
}

void McActor::stop_event()
{
    if (_multicast_socket >= 0)
    {
        close(_multicast_socket);
        _multicast_socket = -1;
    }
}

void McActor::send_unicast(const Envelope &envelope)
{
    if (!_connected)
    {
        ERROR("Cannot send unicast message, not connected to WiFi");
        return;
    }
    if (!_broker_addr)
    {
        ERROR("Cannot send unicast message, broker address not discovered yet");
        return;
    }

    sockaddr_in dst_addr{};
    dst_addr = *_broker_addr;
    if (envelope.encode(_txd_unicast_buffer) == 0)
    {
        DEBUG("Send message_type '%s' to %s @ %s", envelope.msg_type.has_value() ? std::to_string(*(envelope.msg_type)).c_str() : "-", socketAddrToString(&dst_addr).c_str(), socketAddrToString(&dst_addr).c_str());

        int sent = sendto(_unicast_socket, _txd_unicast_buffer.data(), _txd_unicast_buffer.size(), 0, (sockaddr *)&dst_addr, sizeof(dst_addr));
        if (sent < 0)
        {
            ERROR("Failed to send unicast message to %s @ %s: %s (errno=%d, %u bytes)",
                  "broker",
                  socketAddrToString(&dst_addr).c_str(),
                  strerror(errno),
                  errno,
                  static_cast<unsigned>(_txd_unicast_buffer.size()));
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

void McActor::on_message(const ActorMessage &msg)
{
    // local actors specify their capabilities via EndpointAnnounce messages
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

    msg.handle_if<WifiConnected>([&](const WifiConnected &msg)
                                 { INFO("MC Actor received WifiConnected event, starting listeners");
                                    _connected = true; 
                                init_event();
                                start_unicast_listener();
                                start_multicast_listener(); });

    msg.handle_if<WifiDisconnected>([&](const auto &msg) { /*stop_listener(); */ });

    if (!_connected) // ignore other messages if not connected
        return;
    msg.handle_if<TimerMsg>([&](const TimerMsg &msg)
                            {
        EndpointAnnounce announce;
        announce.id = endpoint_id;
        announce.name = endpoint_name;
        announce.events = _events;
        announce.services = _services;
        announce.subscribes = _subscribes;
        broadcast_event(announce); });
    msg.handle_if<TxdMsg>([&](const TxdMsg &pub)
                          { send_unicast(pub.env); });
    msg.handle_if<SysEvent>([&](const SysEvent &msg)
                            { send_event(msg); });
    msg.handle_if<HoverboardEvent>([&](const HoverboardEvent &msg)
                                   { send_event(msg); });
}
static Buffer msg_buffer(1000);
void McActor::send_event(const Msg &msg)
{
    if (msg.encode(msg_buffer) != 0 || !_broker_addr)
    {
        ERROR("Failed to encode message of type %s", msg.msg_name());
        return;
    }
    Envelope envelope;
    envelope.src = endpoint_id;
    envelope.msg_type = msg.msg_id();
    envelope.payload = msg_buffer.to_vector();

    if (envelope.encode(_txd_unicast_buffer) == 0)
    {
        int sent = sendto(_unicast_socket, _txd_unicast_buffer.data(), _txd_unicast_buffer.size(), 0, (sockaddr *)&(*_broker_addr), sizeof(*_broker_addr));
        if (sent < 0)
        {
            ERROR("Failed to send message to %s @ %s: %s (errno=%d, %u bytes)",
                  "destination",
                  socketAddrToString(&_unicast_src_addr).c_str(),
                  strerror(errno),
                  errno,
                  static_cast<unsigned>(_txd_unicast_buffer.size()));
        }
        else
        {
            DEBUG("Sent message to %s (%d bytes)", socketAddrToString(&_unicast_src_addr).c_str(), sent);
        }
    }
}

void McActor::broadcast_event(const Msg &msg)
{
    if (msg.encode(msg_buffer) != 0)
    {
        ERROR("Failed to encode message of type %s", msg.msg_name());
        return;
    }
    Envelope envelope;
    envelope.src = endpoint_id;
    envelope.msg_type = msg.msg_id();
    envelope.payload = msg_buffer.to_vector();

    if (envelope.encode(_txd_multicast_buffer) == 0)
    {
        int sent = sendto(_multicast_socket, _txd_multicast_buffer.data(), _txd_multicast_buffer.size(), 0, (sockaddr *)&_multicast_addr, sizeof(_multicast_addr));
        if (sent < 0)
        {
            ERROR("Failed to send multicast message: %s (errno=%d, %u bytes)",
                  strerror(errno),
                  errno,
                  static_cast<unsigned>(_txd_multicast_buffer.size()));
        }
        else
        {
            DEBUG("Sent multicast message (%d bytes)", sent);
        }
    }
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
        while (actor->_running)
        {
            sockaddr_in sender{};
            socklen_t sender_len = sizeof(sender);
            int len = recvfrom(actor->_unicast_socket, actor->_rxd_unicast_buffer.data(), actor->_rxd_unicast_buffer.capacity(), 0, (sockaddr *)&sender, &sender_len);
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
            actor->_rxd_unicast_buffer.resize(len);
            INFO("<== UDP  [%d] from %s", len, socketAddrToString(&sender).c_str());
            actor->on_udp_raw(actor->_rxd_unicast_buffer, sender);
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
        while (actor->_running)
        {
            sockaddr_in sender{};
            socklen_t sender_len = sizeof(sender);
            int len = recvfrom(actor->_multicast_socket, actor->_rxd_multicast_buffer.data(), actor->_rxd_multicast_buffer.capacity() - 1, 0, (sockaddr *)&sender, &sender_len);
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
            actor->_rxd_multicast_buffer.resize(len);
            INFO("<== MC [%d] from %s", len, socketAddrToString(&sender).c_str());
            actor->on_udp_raw(actor->_rxd_multicast_buffer, sender);
        }
    }
}

extern const char *id_to_string(uint32_t id);
void show_envelope(const Envelope &env)
{
    INFO("Envelope: src=%s dst=%s msg_type=%s payload_size=%zu",
         env.src ? std::to_string(*(env.src)).c_str() : "-",
         env.dst ? std::to_string(*(env.dst)).c_str() : "-",
         env.msg_type ? id_to_string(*(env.msg_type)) : "-",
         env.payload ? env.payload->size() : 0);
}

void McActor::on_udp_raw(const Buffer &request, const sockaddr_in &sender_addr)
{
    Envelope *envelope = new Envelope();
    if (envelope->decode(request) == 0)
    {
        on_udp_message(envelope, sender_addr);
    }
    else
    {
        ERROR("Failed to decode Envelope from UDP message");
        delete envelope;
    }
}

void McActor::on_udp_message(Envelope *env, const sockaddr_in &sender_addr)
{
    if (!env->msg_type || !env->payload)
    {
        ERROR("UdpMessage %s missing fields: msg_type=%s payload=%s",
              env->msg_type ? id_to_string(*(env->msg_type)) : "None",
              env->msg_type ? "Some" : "None",
              env->payload ? "Some" : "None");
        delete env;
        return;
    }
    if (env->msg_type == PingReply::MSG_ID)
    {
        send_ping_req(env->src.value(), ++_last_ping_number);
        delete env;
        return;
    }
    if (env->msg_type == EndpointAnnounceReply::MSG_ID)
    {
        Buffer payload(*(env->payload));
        EndpointAnnounceReply reply;
        if (reply.decode(payload) == 0)
        {
            INFO("Received EndpointAnnounceReply from %s ", socketAddrToString(&sender_addr).c_str());
            if (_broker_addr)
            {
                if (_broker_addr->sin_addr.s_addr != sender_addr.sin_addr.s_addr)
                {
                    INFO("Setting (new) broker address to %s", socketAddrToString(&sender_addr).c_str());
                    _broker_addr = sender_addr;
                }
                // ignore otherwise, we already have a broker address
            }
            else
            {
                INFO("Setting broker address to %s", socketAddrToString(&sender_addr).c_str());
                _broker_addr = sender_addr;
            }
        }
        delete env;
        return;
    }
    emit(env);
}

void McActor::send_multicast(const Envelope &envelope)
{
    if (!_connected)
    {
        ERROR("Cannot send multicast message, not connected to WiFi");
        return;
    };
    if (envelope.encode(_txd_multicast_buffer) == 0)
    {
        INFO("==> MC '%s' to %s", envelope.msg_type ? id_to_string(*(envelope.msg_type)) : "None", MULTICAST_GROUP);
        int sent = sendto(_multicast_socket, _txd_multicast_buffer.data(), _txd_multicast_buffer.size(), 0, (sockaddr *)&_multicast_addr, sizeof(_multicast_addr));
        if (sent < 0)
        {
            ERROR("Multicast failed: %s [%lu]", strerror(errno), _txd_multicast_buffer.size());
        }
    };
}

void McActor::send_ping_req(uint32_t dst, uint32_t number)
{
    static int64_t start_time = esp_timer_get_time();
    static uint32_t counter = 0;
    PingRequest ping;
    Envelope env;
    env.src = endpoint_id;
    env.dst = dst;
    env.msg_type = PingRequest::MSG_ID;
    Buffer payload(50);
    ping.req_id = number;
    if (ping.encode(payload) == 0)
    {
        env.payload = payload.to_vector();
        //    INFO("MC Actor sending Ping %d to %s", number, dst ? dst : "broker");
        send_unicast(env);
        counter++;
        if (counter % 1000 == 0)
        {
            int64_t now = esp_timer_get_time();
            INFO("ping %f msg/sec", (counter * 10000000.0) / (now - start_time));
            start_time = now;
            counter = 0;
        }
    }
}

void McActor::send_ping_rep(uint32_t dst, uint32_t number)
{
    PingReply pong;
    Envelope env;
    Buffer payload(50);
    env.src = endpoint_id;
    env.dst = dst;
    env.msg_type = PingReply::MSG_ID;
    pong.req_id = number;
    if (pong.encode(payload) == 0)
    {
        env.payload = payload.to_vector();
        //    INFO("MC Actor sending PingReply %d to %s", number, dst ? dst : "broker");
        send_unicast(env);
    }
}
