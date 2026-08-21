#ifndef SERIAL_ACTOR_H
#define SERIAL_ACTOR_H
#include "app_compat.h"
#include <actor_framework.h>
#include "timer.h"
#include "messages.h"
#include "msgs.h"
#include "circ_buf.h"
#include "cbor_codec.h"
#include "cobs_codec.h"

class SerialActor : public Actor
{
private:
    HardwareSerial &_serial;
    ActorId _tx_led;
    Timer _timer;
    enum class SerialState
    {
        IDLE,
        SENDING,
        RECEIVING,
    } _state = SerialState::IDLE;
    CircBuf<uint8_t> _txBuffer{256};
    CircBuf<uint8_t> _rxBuffer{256};
    Buffer _cborBuffer{256};
    std::shared_ptr<ActorMessage> _led_pulse_msg;
    EndpointAnnounce _announce_msg;
    uint32_t _endpopint_id;
    uint32_t sequence_number = 0;

public:
    explicit SerialActor(HardwareSerial &serial, ActorId greenLedActorId);
    void setup() override;
    void update() override;
    void receive(std::shared_ptr<ActorMessage> msg) override;
    void sendSerialMsg(Msg *msg);
};

#endif // SERIAL_ACTOR_H