#include "serial_actor.h"
#include "driver/uart.h"

constexpr int BUF_SIZE = 1024;
constexpr uart_port_t SERIAL_PORT = UART_NUM_0;
constexpr int SERIAL_BAUD_RATE = 115200;

SerialActor::SerialActor(const char *hostname)
    : Actor(), _txd_serial_buffer(1000)
{
    _timer_publish = timer_repetitive(1000);
    endpoint_name = "hoverboard";
    endpoint_id = FNV("hoverboard");
}

SerialActor::~SerialActor()
{
}

void SerialActor::setup()
{
    _serial_tx_mutex = xSemaphoreCreateMutex();
    if (_serial_tx_mutex == nullptr)
    {
        ERROR("Failed to create UART0 transmit mutex");
        return;
    }
    esp_err_t err = uart_driver_install(SERIAL_PORT, BUF_SIZE * 2, 0, 0, nullptr, 0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE)
    {
        ERROR("Failed to install UART0 driver: %s", esp_err_to_name(err));
        return;
    }
    INFO("UART0 serial actor started at %d baud", SERIAL_BAUD_RATE);
    start_serial_listener();
}

void SerialActor::receive(const ActorMessage &msg)
{
    // local actors specify their capabilities via EndpointAnnounce messages
    msg.handle<EndpointAnnounce>([&](const EndpointAnnounce &msg)
                                    {
        msg.services.handle_if([&](const std::vector<uint32_t> &services)
        {
             for (auto v : services)
                _services.push_back(v);
        });
        msg.events.handle_if([&](const std::vector<uint32_t> &events)
        {
            for (auto v : events)
                _events.push_back(v);
        });
        msg.subscribes.handle_if([&](const std::vector<uint32_t> &subscribes)
        {
            for (auto v : subscribes)
                _subscribes.push_back(v);
        }); });

    msg.handle<TimerMsg>([&](const TimerMsg &msg)
                            {
        EndpointAnnounce announce;
        announce.id = endpoint_id;
        announce.name = endpoint_name;
        announce.events = _events;
        announce.services = _services;
        announce.subscribes = _subscribes;
        broadcast_event(announce); });
    msg.handle<TxdMsg>([&](const TxdMsg &pub)
                          { send_serial(pub.env); });
    msg.handle<SysEvent>([&](const SysEvent &msg)
                            { send_event(msg); });
    msg.handle<HoverboardEvent>([&](const HoverboardEvent &msg)
                                   { send_event(msg); });
}
static Buffer msg_buffer(1000);
void SerialActor::send_event(const Msg &msg)
{
    broadcast_event(msg);
}

void SerialActor::broadcast_event(const Msg &msg)
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

    send_serial(envelope);
}

void SerialActor::start_serial_listener()
{
    xTaskCreatePinnedToCore(serial_listener_task, "uart0_listener", 8192, this, 5, NULL, 1);
}

void SerialActor::serial_listener_task(void *pvParameters)
{
    SerialActor *actor = static_cast<SerialActor *>(pvParameters);
    actor->serial_listener_loop();
}

extern const char *id_to_string(uint32_t id);
void show_envelope(const Envelope &env)
{
    INFO("Envelope: src=%s dst=%s msg_type=%s payload_size=%zu",
         env.src ? std::to_string(*(env.src)).c_str() : "-",
         env.dst ? std::to_string(*(env.dst)).c_str() : "-",
         env.msg_type ? id_to_string(*(env.msg_type)) : "-",
         env.payload ? (*env.payload).size() : 0);
}

void SerialActor::serial_listener_loop()
{
    uint8_t data[256];
    while (true)
    {
        const int len = uart_read_bytes(SERIAL_PORT, data, sizeof(data), pdMS_TO_TICKS(200));
        for (int index = 0; index < len; ++index)
        {
            if (data[index] == 0)
            {
                if (!_serial_frame.empty())
                {
                    _serial_frame.push_back(0);
                    Buffer frame(_serial_frame);
                    if (frame.cobs_decode_inplace() == 0)
                    {
                        on_serial_raw(frame);
                    }
                    _serial_frame.clear();
                }
                continue;
            }
            if (_serial_frame.size() < _txd_serial_buffer.capacity() - 1)
            {
                _serial_frame.push_back(data[index]);
            }
            else
            {
                WARN("UART0 frame exceeds %u bytes; discarding", static_cast<unsigned>(_txd_serial_buffer.capacity()));
                _serial_frame.clear();
            }
        }
    }
}

void SerialActor::on_serial_raw(Buffer &request)
{
    Envelope *envelope = new Envelope();
    if (envelope->decode(request) == 0)
    {
        on_message(envelope);
    }
    else
    {
        DEBUG("Discarding non-envelope UART0 frame");
        delete envelope;
    }
}

void SerialActor::on_message(Envelope *env)
{
    if (!env->msg_type || !env->payload)
    {
        ERROR("Serial message %s missing fields: msg_type=%s payload=%s",
              env->msg_type ? id_to_string(*(env->msg_type)) : "None",
              env->msg_type ? "Some" : "None",
              env->payload ? "Some" : "None");
        delete env;
        return;
    }
    if (env->msg_type == PingReply::MSG_ID)
    {
        delete env;
        return;
    }
    if (env->msg_type == EndpointAnnounceReply::MSG_ID)
    {
        Buffer payload(*(env->payload));
        EndpointAnnounceReply reply;
        if (reply.decode(payload) == 0)
        {
            INFO("Received EndpointAnnounceReply over UART0");
        }
        delete env;
        return;
    }
  //  emit(env);
}

void SerialActor::send_serial(const Envelope &envelope)
{
    if (envelope.encode(_txd_serial_buffer) != 0 ||
        _txd_serial_buffer.add_crc() != 0 ||
        _txd_serial_buffer.cobs_encode_inplace() != 0)
    {
        ERROR("Failed to encode UART0 envelope");
        return;
    }

    std::vector<uint8_t> framed_message;
    framed_message.reserve(_txd_serial_buffer.size() + 1);
    framed_message.push_back(0);
    framed_message.insert(framed_message.end(), _txd_serial_buffer.data(),
                          _txd_serial_buffer.data() + _txd_serial_buffer.size());

    xSemaphoreTake(_serial_tx_mutex, portMAX_DELAY);
    fwrite(framed_message.data(), 1, framed_message.size(), stdout);
    fflush(stdout);
    xSemaphoreGive(_serial_tx_mutex);
    DEBUG("Sent UART0 frame (%u bytes)", static_cast<unsigned>(_txd_serial_buffer.size()));
}