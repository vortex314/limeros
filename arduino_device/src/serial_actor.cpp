
#include "serial_actor.h"
#include "msgs.h"

SerialActor::SerialActor(HardwareSerial &serial, ActorId greenLedActorId) : _serial(serial), _tx_led(greenLedActorId), _timer(id, 1) {}

void SerialActor::setup()
{
    _timer.setOwner(id);
    _timer.start(1000, true); // Start the timer with a 1 msec interval
    _serial.begin(115200);
}

void SerialActor::update()
{
    _timer.update();
}

void SerialActor::receive(const ActorMessage &msg)
{
    msg.handle<SerialSendFrameMsg>([this](const SerialSendFrameMsg &m)
                                   {
                                       if (_state == SerialState::IDLE)
                                       {
                                           _txBuffer.write(m.data, m.length);
                                           _state = SerialState::SENDING;
                                       } });

    msg.handle<TimerMsg>([this](const TimerMsg &m)
                         {
                             INFO("TimerMsg received, sending EndpointAnnounce message");
                             Buffer buffer(256);
                             Envelope env;
                             env.src = FNV("Imu");
                             env.msg_type = EndpointAnnounce::MSG_ID;
                             EndpointAnnounce announce;
                             announce.id = 1;
                             announce.name = "IMU Sensor";
                             announce.description = "Inertial Measurement Unit";
                             announce.events = std::vector<uint32_t>{ImuEvent::MSG_ID};
                             if (announce.encode(buffer))
                             {
                                WARN("Failed to encode EndpointAnnounce message");
                                 return;
                             }
                             env.payload = std::vector<uint8_t>(buffer.data(), buffer.data() + buffer.size());
                             if (env.encode(buffer))
                             {
                                WARN("Failed to encode Envelope message");
                                 return;
                             }
                             INFO("Sending Envelope message with payload size %lu", buffer.size());
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
                             Serial1.write(buffer.data(), buffer.size());
                             // convert to cbor
                             // cobs encode it
                             // send over serial
                             // pulse led
                         });
}
