#ifndef UDP_ACTOR_H
#define UDP_ACTOR_H
#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "log.h"
#include "actor.h"
#include <serial_actor.h>
#include <wifi_actor.h>
#include <msgs.h>

class UdpActor : public Actor
{
private:
  std::string endpoint_name;
  uint32_t endpoint_id;
  int _timer_publish;
  std::vector<std::uint32_t> _events;
  std::vector<std::uint32_t> _subscribes;
  std::vector<std::uint32_t> _services;
  Buffer _txd_udp_buffer;
  Buffer _rxd_udp_buffer;
  sockaddr_in _peer_addr{};
  int _socket = -1;
  bool _started = false;
  bool _running = false;

  void udp_listener_loop();
  void send_udp(const Envelope &envelope);
  void init();
  void stop();

public:
  UdpActor(const char *hostname);
  ~UdpActor();
  void receive(const ActorMessage &);
  void setup();
  void start_udp_listener();
  void send_event(const Msg& msg);
  void broadcast_event(const Msg& msg);
  void on_udp_raw(Buffer &request);
  void on_message(Envelope *message);
  static void udp_listener_task(void *param);
};

#endif // UDP_ACTOR_H