
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
                             INFO("SerialActor::receive: TimerMsg received, sending EndpointAnnounce message");
                             Buffer buffer(256);
                             Envelope env;
                             env.src = FNV1A32("Imu");
                             env.msg_type = EndpointAnnounce::MSG_ID;
                             EndpointAnnounce announce;
                             announce.id = 1;
                             announce.name = std::string("IMU Sensor");
                             announce.description = std::string("Inertial Measurement Unit");
                             announce.services = std::vector<uint32_t>{FNV1A32("IMUService")};
                             announce.events = std::vector<uint32_t>{FNV1A32("IMUEvent")};
                             announce.replies = std::vector<uint32_t>{FNV1A32("IMUReply")};
                             announce.subscribes = std::vector<uint32_t>{FNV1A32("IMUSubscribe")};
                             INFO("SerialActor::receive: Sending EndpointAnnounce message");
                             if (announce.encode(buffer))
                             {
                                WARN("SerialActor::receive: Failed to encode EndpointAnnounce message");
                                 return;
                             }
                             env.payload = std::vector<uint8_t>(buffer.data(), buffer.data() + buffer.size());
                             if (env.encode(buffer))
                             {
                                WARN("SerialActor::receive: Failed to encode Envelope message");
                                 return;
                             }
                             INFO("SerialActor::receive: Sending Envelope message with payload size %zu", buffer.size());
                             cobs_encode(buffer.data(), buffer.size(), buffer.data(), buffer.size());
                             Serial1.write(buffer.data(), buffer.size());
                             // convert to cbor
                             // cobs encode it
                             // send over serial
                             // pulse led
                         });
}
