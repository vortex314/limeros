#ifndef SERIAL_ACTOR_H
#define SERIAL_ACTOR_H
#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "log.h"
#include "actor.h"
#include <msgs.h>

class TxdMsg : public ActorPayload
{
TYPED_MESSAGE(TxdMsg)
  TxdMsg(Envelope env) : env(env) {}
  Envelope env;
};

class Transmitting : public ActorPayload
{
TYPED_MESSAGE(Transmitting)

};

class SerialActor : public Actor
{
private:
  std::string endpoint_name;
  uint32_t endpoint_id;
  int _timer_publish;
  std::vector<std::uint32_t> _events;
  std::vector<std::uint32_t> _subscribes;
  std::vector<std::uint32_t> _services;
  Buffer _txd_serial_buffer;
  std::vector<uint8_t> _serial_frame;
  SemaphoreHandle_t _serial_tx_mutex = nullptr;

  void serial_listener_loop();
  void send_serial(const Envelope &envelope);

public:
  SerialActor(const char *hostname);
  ~SerialActor();
  void receive(const ActorMessage &);
  void setup();
  void start_serial_listener();
  void send_event(const Msg& msg);
  void broadcast_event(const Msg& msg);
  void on_serial_raw(Buffer &request);
  void on_message(Envelope *message);
  static void serial_listener_task(void *param);
};

#endif // SERIAL_ACTOR_H