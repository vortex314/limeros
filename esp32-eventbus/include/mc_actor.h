#ifndef MC_ACTOR_H
#define MC_ACTOR_H
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"

#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "log.h"
#include "actor.h"
#include <wifi_actor.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <string.h>
#include <lwip/def.h>
#include <lwip/inet.h>
#include <msgs.h>

class RxdMsg : public Msg
{
public:
  static const uint32_t MSG_ID = FNV("RxdMsg");
  static constexpr const char *MSG_NAME = "RxdMsg";

  virtual uint32_t msg_id() const { return MSG_ID; };
  virtual const char *msg_name() const { return MSG_NAME; };
  RxdMsg(Envelope env) : env(env) {}
  Envelope env;
};

class TxdMsg : public Msg
{
public:
  static const uint32_t MSG_ID = FNV("TxdMsg");
  static constexpr const char *MSG_NAME = "TxdMsg";

  virtual uint32_t msg_id() const { return MSG_ID; };
  virtual const char *msg_name() const { return MSG_NAME; };
  TxdMsg(Envelope env) : env(env) {}
  Envelope env;
};

class Transmitting : public Msg
{
public:
  static const uint32_t MSG_ID = FNV("Transmitting");
  static constexpr const char *MSG_NAME = "Transmitting";

  virtual uint32_t msg_id() const { return MSG_ID; };
  virtual const char *msg_name() const { return MSG_NAME; };
};

class McActor : public Actor
{
private:
  std::string endpoint_name;
  uint32_t endpoint_id;
  std::string _hostname;
  int _unicast_socket;
  int _multicast_socket;
  sockaddr_in _unicast_src_addr;
  sockaddr_in _multicast_addr;
  bool _running = true;
  int _timer_publish;
  uint32_t _ping_counter = 0;
  bool _connected = false;
  std::unordered_map<uint32_t, std::pair<sockaddr_in, uint64_t>> _source_map;
  Option<sockaddr_in> _broker_addr = Option<sockaddr_in>::None();
  uint32_t _last_ping_number = 1;
  std::vector<std::uint32_t> _events;
  std::vector<std::uint32_t> _subscribes;
  std::vector<std::uint32_t> _services;
  Buffer _txd_unicast_buffer;
  Buffer _rxd_unicast_buffer;
  Buffer _txd_multicast_buffer;
  Buffer _rxd_multicast_buffer;

public:
  McActor(const char *name, const char *hostname);
  ~McActor();
  void on_message(const ActorMessage &);
  void on_start();
  void init_event();
  void stop_event();
  void start_unicast_listener();
  void start_multicast_listener();
  void stop_listener();
  void send_unicast(const Envelope &envelope);
  void send_multicast(const Envelope &envelope);
  void on_udp_raw(const Buffer &request, const sockaddr_in &sender_addr);
  void on_udp_message(Envelope *udp_message, const sockaddr_in &sender_addr);
  void send_ping_req(uint32_t dst, uint32_t number);
  void send_ping_rep(uint32_t dst, uint32_t number);
  Result<Bytes> encode_message(uint32_t dst, uint32_t src, uint32_t type, const Bytes &value);
  static void unicast_listener_task(void *param);
  static void multicast_listener_task(void *param);
};

#endif // MC_ACTOR_H
