#ifndef SERIAL_ACTOR_H
#define SERIAL_ACTOR_H
#include <actor_framework.h>
#include <Arduino.h>
#include "timer.h"
#include "messages.h"

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


public:
    explicit SerialActor(HardwareSerial &serial, ActorId greenLedActorId);
    void setup() override;
    void update() override;
    void receive(const ActorMessage &msg) override;
};

#endif // SERIAL_ACTOR_H