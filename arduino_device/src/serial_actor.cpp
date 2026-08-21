
#include "serial_actor.h"

#ifndef ENDPOINT_ID 
#define ENDPOINT_ID FNV("IMU")
#endif 

SerialActor::SerialActor(HardwareSerial &serial, ActorId greenLedActorId) : _serial(serial), _tx_led(greenLedActorId), _timer(1) {}

void SerialActor::setup()
{
    _endpopint_id = ENDPOINT_ID;
    _timer.setOwner(id);
    _timer.start(1000, true); // Start the timer with a 1 msec interval
    _serial.begin(115200);
    _led_pulse_msg = std::make_shared<ActorMessage>();
    _led_pulse_msg->src = id;
    _led_pulse_msg->dst = _tx_led;
    _led_pulse_msg->set_payload(new LedPulse(10));
    _announce_msg.id = _endpopint_id;
    _announce_msg.name = "IMU Sensor";
    _announce_msg.description = "Inertial Measurement Unit";
    _announce_msg.events = std::vector<uint32_t>{ImuEvent::MSG_ID};
}

void SerialActor::update()
{
    _timer.update();
}

void print_buffer_hex(Buffer &buffer)
{
    printf("[%d] ", buffer.size());
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}

void SerialActor::sendSerialMsg(Msg *msg)
{
    Buffer buffer(256);
    if (msg->encode(buffer))
    {
        WARN("Failed to encode Msg");
        return;
    }
    print_buffer_hex(buffer);
    Envelope env;
    env.src = FNV("Imu");
    env.msg_type = msg->msg_id();
    env.payload = std::vector<uint8_t>(buffer.data(), buffer.data() + buffer.size());
    if (env.encode(buffer))
    {
        WARN("Failed to encode Envelope message");
        return;
    }
    if (buffer.add_crc())
    {
        WARN("Failed to add CRC to buffer");
        return;
    }
    if (buffer.cobs_encode_inplace())
    {
        WARN("Failed to COBS encode buffer");
        return;
    }

    actor_system.send(_led_pulse_msg);

    Serial1.write(buffer.data(), buffer.size());
}



void SerialActor::receive(std::shared_ptr<ActorMessage> msg)
{
    msg->handle<SerialSendFrameMsg>([this](const SerialSendFrameMsg &m)
                                    {
                                       if (_state == SerialState::IDLE)
                                       {
                                           _txBuffer.write(m.data, m.length);
                                           _state = SerialState::SENDING;
                                       } });

    msg->handle<TimerMsg>([this](const TimerMsg &m)
                          {
                              INFO("TimerMsg received, sending EndpointAnnounce message");
                              sendSerialMsg(&_announce_msg); });
    msg->handle<FromDevice>([this](const FromDevice &m)
                            { INFO("FromDevice message received, sending to serial port.");
                                sendSerialMsg(m.msg); });
}
