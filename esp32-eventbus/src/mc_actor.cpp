#include "mc_actor.h"
#include <poly_json.h>

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
    UdpMessage msg;
    msg.dst = dst;
    msg.src = src;
    msg.typ = type;
    msg.msg = payload;
    return UdpMessage::serialize(msg);
}

void McActor::send_unicast(uint32_t dst, uint32_t src, uint32_t type, const Bytes &payload)
{
    if (!_connected)
    {
        ERROR("Cannot send unicast message, not connected to WiFi");
        return;
    }

    const char *requested_dst = dst;
    dst = dst ? dst : (_broker_name.has_value() ? _broker_name->c_str() : NULL);
    const std::string payload_json = poly::json::serialize_or_empty(payload);
    DEBUG("send_unicast dst: %s , src: %s : %s = %s", dst ? dst : "broker", src ? src : "unknown", type, payload_json.c_str());

    sockaddr_in dst_addr{};
    bool have_destination = false;

    if (dst && _source_map.find(dst) != _source_map.end())
    {
        dst_addr = _source_map[dst].first;
        have_destination = true;
    }
    else if (_broker_addr.has_value())
    {
        dst_addr = *_broker_addr;
        have_destination = true;
    }

    if (!have_destination || dst_addr.sin_family == 0)
    {
        DEBUG("Skipping unicast '%s' from '%s': broker/destination not discovered yet", type, src ? src : "unknown");
        return;
    }

    auto r = encode_message(dst, src, type, payload);
    if (r.is_err())
    {
        ERROR("Failed to encode unicast message: [%d] %s", r.unwrap_err().rc, r.unwrap_err().msg);
        return;
    }
    r.just([&](const Bytes &buffer)
           {
    DEBUG("Send message_type '%s' to %s @ %s", type, dst ? dst : "-", socketAddrToString(&dst_addr).c_str());

    int sent = sendto(_unicast_socket, buffer.data(), buffer.size(), 0, (sockaddr *)&dst_addr, sizeof(dst_addr));
    if (sent < 0)
    {
        ERROR("Failed to send unicast message to %s @ %s: %s (errno=%d, %u bytes)",
              dst ? dst : "broker",
              socketAddrToString(&dst_addr).c_str(),
              strerror(errno),
              errno,
              static_cast<unsigned>(buffer.size()));

        if (requested_dst == NULL)
        {
            _broker_addr.reset();
            _broker_name.reset();
        }
    }
    else
    {
        DEBUG("Sent message to %s (%d bytes)", socketAddrToString(&dst_addr).c_str(), sent);
    } });

    // send bytes
}



void McActor::stop_listener()
{
    _running = false;
}

void McActor::on_message(const Envelope &envelope)
{
    const Msg &msg = *envelope.msg;
    msg.handle<DeviceAliveEvent>([&](const auto &msg) // accept actor capabilities
                           { for ( auto& pub : msg.publishes) {
                            _publishes.push_back(pub);
                         };
                         for ( auto& sub : msg.subscribes) {
                            _subscribes.push_back(sub);
                         };
                         for ( auto& serv : msg.services) {
                            _services.push_back(serv);
                         } });
    msg.handle<TimerMsg>([&](const TimerMsg &msg)
                         { DeviceAliveEvent alive;
                         for (const auto& pub : _publishes) {
                            alive.publishes.push_back(pub);
                         }
                         for (const auto& serv : _services) {
                            alive.services.push_back(serv);
                         }
                         for (const auto& sub : _subscribes) {
                            alive.subscribes.push_back(sub);
                         }
                            auto bytes = DeviceAliveEvent::to_poly(alive).unwrap();
                            send_multicast(NULL,_hostname.c_str(),DeviceAliveEvent::name_value,bytes);
                            send_ping_req(NULL,0); });

    msg.handle<PingRequest>([&](const auto &m) { /*INFO("Received PingReq from '%s' number %d",
                                               envelope.src.has_value() ? envelope.src->name() : "-",
                                               m.number.has_value() ? *(m.number) : -1 );*/
                                                 if (m.number.has_value())
                                                 {
                                                     send_ping_rep(envelope.src->name(), *(m.number));
                                                 }
    });

    msg.handle<WifiConnected>([&](const auto &msg)
                              { _connected = true; 
                                init_event();
                                start_unicast_listener();
                                start_multicast_listener(); });

    msg.handle<WifiDisconnected>([&](const auto &msg) { /*stop_listener(); */ });

    if (!_connected) // ignore other messages if not connected
        return;
    msg.handle<Max31855Event>([&](const auto &msg)
                              { Max31855Event::to_poly(msg).just([&](const Bytes &serialized_msg)
                                            { 
                                                DEBUG("Max31855Event as poly: %s", serialized_msg.to_string().c_str());
                                                send_unicast(NULL, _hostname.c_str(), msg.type_name(), serialized_msg); }); });
    msg.handle<HeatingEvent>([&](const auto &msg)
                             { HeatingEvent::to_poly(msg).just([&](const auto &serialized_msg)
                                                               { send_unicast(NULL, _hostname.c_str(), msg.type_name(), serialized_msg); }); });
    msg.handle<SysEvent>([&](const auto &msg)
                         { SysEvent::to_poly(msg).just([&](const auto &serialized_msg)
                                                       { send_unicast(NULL, _hostname.c_str(), msg.type_name(), serialized_msg); }); });
    msg.handle<WifiEvent>([&](const auto &msg)
                          { WifiEvent::to_poly(msg).just([&](const auto &serialized_msg)
                                                         { send_unicast(NULL, _hostname.c_str(), msg.type_name(), serialized_msg); }); });
    msg.handle<HoverboardEvent>([&](const auto &msg)
                                { HoverboardEvent::to_poly(msg).just([&](const auto &serialized_msg)
                                                                     { send_unicast(NULL, _hostname.c_str(), msg.type_name(), serialized_msg); }); });
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
    auto r = UdpMessage::json_deserialize(request);
    if (r.is_err())
    {
        ERROR("Failed to deserialize UdpMessage");
        return;
    }
    UdpMessage *udp_msg = r.unwrap();
    DEBUG("Received UDP message  %s", std::string(request.begin(), request.end()).c_str());
    on_udp_message(*udp_msg, sender_addr);
    delete udp_msg;
}

template <typename M>
bool put_on_actor_bus(Actor *actor, UdpMessage &udp_msg)
{
    if (strcmp(udp_msg.typ->c_str(), M::name_value) == 0)
    {
        M::from_poly(*(udp_msg.msg)).just([&](const M *msg)
                                          { actor->emit(new Envelope(
                                                udp_msg.src.has_value() ? ActorRef(udp_msg.src->c_str()) : NULL_ACTOR,
                                                udp_msg.dst.has_value() ? ActorRef(udp_msg.dst->c_str()) : NULL_ACTOR,
                                                msg)); });
        INFO("Message '%s' to bus '%s' : %s",
             M::name_value, udp_msg.src.has_value() ? udp_msg.src->c_str() : "-",
             udp_msg.msg.has_value() ? udp_msg.msg->as_string().c_str() : "-");
        return true;
    }
    return false;
}

void McActor::on_udp_message(UdpMessage &udp_msg, const sockaddr_in &sender_addr)
{
    if (!udp_msg.typ.has_value() || !udp_msg.msg.has_value())
    {
        ERROR("UdpMessage missing fields: typ=%s msg=%s",
              udp_msg.typ.has_value() ? "yes" : "no",
              udp_msg.msg.has_value() ? "yes" : "no");
        return;
    }

    Bytes &payload = *(udp_msg.msg);
    const char *typ = udp_msg.typ->c_str();

    put_on_actor_bus<PingRequest>(this, udp_msg);
    put_on_actor_bus<HoverboardRequest>(this, udp_msg);
    put_on_actor_bus<HeatingRequest>(this, udp_msg);
    put_on_actor_bus<SysRequest>(this, udp_msg);
    //  put_on_actor_bus<DeviceAliveEvent>(this, udp_msg);

    if (strcmp(typ, PingReply::name_value) == 0)
    {
        send_ping_req(udp_msg.src->c_str(), ++_last_ping_number);
    }
    else if (strcmp(typ, DeviceAliveEvent::name_value) == 0)
    {
        DEBUG("MC Actor received DeviceAliveEvent message");
        DeviceAliveEvent::from_poly(payload).just([&](const DeviceAliveEvent *alive)
                                            {
                if (!udp_msg.src.has_value()) {
                    ERROR("DeviceAliveEvent message missing src");
                    return;
                }
                std::string source = *(udp_msg.src);

//                INFO("  Endpoint: %s @ %s ", etp.c_str(), socketAddrToString((sockaddr_in*)&sender_addr).c_str());
                _source_map[source] = std::pair<sockaddr_in,uint64_t>(sender_addr,esp_timer_get_time());
                const bool is_broker_source = (source == "broker") || (source.find("broker") != std::string::npos);
                if (is_broker_source)
                {
                    DEBUG("Received DeviceAliveEvent from broker-like source '%s' at %s", source.c_str(), socketAddrToString((sockaddr_in*)&sender_addr).c_str());
                    if (_broker_addr.has_value() && memcmp(&_broker_addr.value(), &sender_addr, sizeof(sender_addr)) != 0)
                    {
                        INFO("Broker address changed from %s to %s", socketAddrToString(&_broker_addr.value()).c_str(), socketAddrToString((sockaddr_in*)&sender_addr).c_str());
                    }
                    _broker_addr = sender_addr;
                    _broker_name = source;
                };
                            delete alive; });
    }
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
