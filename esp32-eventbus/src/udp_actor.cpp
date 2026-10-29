#include "udp_actor.h"

#include <errno.h>
#include <string.h>

constexpr char UDP_PEER_ADDRESS[] = "192.168.0.148";
constexpr uint16_t UDP_PORT = 50001;
constexpr size_t UDP_BUFFER_SIZE = 1000;

UdpActor::UdpActor(const char *hostname)
    : Actor(), _txd_udp_buffer(UDP_BUFFER_SIZE), _rxd_udp_buffer(UDP_BUFFER_SIZE)
{
    _timer_publish = timer_repetitive(1000);
    #ifdef ENABLE_HOVERBOARD
    endpoint_name = "hoverboard";
    endpoint_id = FNV("hoverboard");
    #endif
    #ifdef ENABLE_MOTOR
    endpoint_name = "motor";
    endpoint_id = FNV("motor");
    #endif
    #ifdef ENABLE_CUTTER
    endpoint_name = "motor";
    endpoint_id = FNV("motor");
    #endif
    INFO("UDP actor initialized with endpoint name '%s' and id %u", endpoint_name, endpoint_id);
}

UdpActor::~UdpActor()
{
    stop();
}

void UdpActor::setup()
{
    INFO("UDP actor waiting for WiFi before opening %s:%u", UDP_PEER_ADDRESS, UDP_PORT);
}

void UdpActor::init()
{
    if (_started)
    {
        return;
    }
        INFO("UDP actor initialized with endpoint name '%s' and id %u", endpoint_name, endpoint_id);


    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (_socket < 0)
    {
        ERROR("Failed to create UDP socket: %s", strerror(errno));
        return;
    }

    sockaddr_in local_addr{};
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(UDP_PORT);
    if (bind(_socket, reinterpret_cast<sockaddr *>(&local_addr), sizeof(local_addr)) < 0)
    {
        ERROR("Failed to bind UDP port %u: %s", UDP_PORT, strerror(errno));
        close(_socket);
        _socket = -1;
        return;
    }

    _peer_addr = {};
    _peer_addr.sin_family = AF_INET;
    _peer_addr.sin_port = htons(UDP_PORT);
    if (inet_pton(AF_INET, UDP_PEER_ADDRESS, &_peer_addr.sin_addr) != 1)
    {
        ERROR("Invalid UDP peer address %s", UDP_PEER_ADDRESS);
        close(_socket);
        _socket = -1;
        return;
    }

    timeval timeout{};
    timeout.tv_usec = 200000;
    setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    _running = true;
    _started = true;
    INFO("UDP actor started for %s:%u", UDP_PEER_ADDRESS, UDP_PORT);
    start_udp_listener();
}

void UdpActor::stop()
{
    _running = false;
    _started = false;
    if (_socket >= 0)
    {
        close(_socket);
        _socket = -1;
    }
}

void UdpActor::receive(const ActorMessage &msg)
{
    msg.handle<WifiConnected>([&](const WifiConnected &) { init(); });
    msg.handle<WifiDisconnected>([&](const WifiDisconnected &) { stop(); });

    msg.handle<EndpointAnnounce>([&](const EndpointAnnounce &announce)
    {
        announce.services.handle_if([&](const std::vector<uint32_t> &services)
        {
            _services.insert(_services.end(), services.begin(), services.end());
        });
        announce.events.handle_if([&](const std::vector<uint32_t> &events)
        {
            _events.insert(_events.end(), events.begin(), events.end());
        });
        announce.subscribes.handle_if([&](const std::vector<uint32_t> &subscribes)
        {
            _subscribes.insert(_subscribes.end(), subscribes.begin(), subscribes.end());
        });
    });

    if (!_running)
    {
        return;
    }

    msg.handle<TimerMsg>([&](const TimerMsg &)
    {
        EndpointAnnounce announce;
        announce.id = endpoint_id;
        announce.name = endpoint_name;
        announce.events = _events;
        announce.services = _services;
        announce.subscribes = _subscribes;
        broadcast_event(announce);
    });
    msg.handle<TxdMsg>([&](const TxdMsg &message) { send_udp(message.env); });
    msg.handle<SysEvent>([&](const SysEvent &event) { send_event(event); });
    msg.handle<HoverboardEvent>([&](const HoverboardEvent &event) { send_event(event); });
    msg.handle<MotorEvent>([&](const MotorEvent &event) { send_event(event); });
    msg.handle<MotorReply>([&](const MotorReply &event) { send_event(event); });
}

void UdpActor::send_event(const Msg &msg)
{
    broadcast_event(msg);
}

void UdpActor::broadcast_event(const Msg &msg)
{
    Buffer payload(UDP_BUFFER_SIZE);
    if (msg.encode(payload) != 0)
    {
        ERROR("Failed to encode message of type %s", msg.msg_name());
        return;
    }

    Envelope envelope;
    envelope.src = endpoint_id;
    envelope.msg_type = msg.msg_id();
    envelope.payload = payload.to_vector();
    send_udp(envelope);
}

void UdpActor::send_udp(const Envelope &envelope)
{
    if (!_running)
    {
        return;
    }
    if (envelope.encode(_txd_udp_buffer) != 0)
    {
        ERROR("Failed to encode UDP envelope");
        return;
    }

    int sent = sendto(_socket, _txd_udp_buffer.data(), _txd_udp_buffer.size(), 0,
                      reinterpret_cast<sockaddr *>(&_peer_addr), sizeof(_peer_addr));
    if (sent < 0)
    {
        ERROR("Failed to send UDP frame to %s:%u: %s", UDP_PEER_ADDRESS, UDP_PORT, strerror(errno));
    }
    else
    {
        INFO("Sent UDP frame type=%u bytes=%d to %s:%u",
             envelope.msg_type ? *envelope.msg_type : 0,
             sent,
             UDP_PEER_ADDRESS,
             UDP_PORT);
    }
}

void UdpActor::start_udp_listener()
{
    xTaskCreatePinnedToCore(udp_listener_task, "udp_listener", 8192, this, 5, nullptr, 1);
}

void UdpActor::udp_listener_task(void *param)
{
    static_cast<UdpActor *>(param)->udp_listener_loop();
}

void UdpActor::udp_listener_loop()
{
    while (_running)
    {
        sockaddr_in sender{};
        socklen_t sender_len = sizeof(sender);
        int received = recvfrom(_socket, _rxd_udp_buffer.data(), _rxd_udp_buffer.capacity(), 0,
                                reinterpret_cast<sockaddr *>(&sender), &sender_len);
        if (received < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            if (_running)
            {
                ERROR("UDP receive failed: %s", strerror(errno));
            }
            break;
        }
        if (sender.sin_addr.s_addr != _peer_addr.sin_addr.s_addr || sender.sin_port != _peer_addr.sin_port)
        {
            WARN("Ignoring UDP packet from an unexpected peer");
            continue;
        }
        INFO("Received UDP frame bytes=%d from configured peer", received);
        _rxd_udp_buffer.resize(static_cast<size_t>(received));
        on_udp_raw(_rxd_udp_buffer);
    }
    vTaskDelete(nullptr);
}

void UdpActor::on_udp_raw(Buffer &request)
{
    Envelope *envelope = new Envelope();
    const int decode_result = envelope->decode(request);
    if (decode_result == 0)
    {
        on_message(envelope);
    }
    else
    {
        ERROR("Discarding invalid UDP envelope: CBOR error %d", decode_result);
        delete envelope;
    }
}

void UdpActor::on_message(Envelope *env)
{
    if (!env->msg_type || !env->payload)
    {
        ERROR("UDP message missing fields");
        delete env;
        return;
    }
    INFO("Received UDP envelope type=%u", *env->msg_type);
    if (env->msg_type == EndpointAnnounceReply::MSG_ID)
    {
        Buffer payload(*(env->payload));
        EndpointAnnounceReply reply;
        if (reply.decode(payload) == 0)
        {
            INFO("Received EndpointAnnounceReply over UDP");
        }
    }
    else if (env->msg_type == MotorRequest::MSG_ID)
    {
        Buffer payload(*(env->payload));
        MotorRequest *request = new MotorRequest();
        if (request->decode(payload) == 0)
        {
            INFO("Received MotorRequest over UDP");
            emit(request);
        }
        else
        {
            ERROR("Failed to decode MotorRequest from UDP");
            delete request;
        }
    }
    delete env;
}