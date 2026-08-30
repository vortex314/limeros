#ifndef _WIFI_ACTOR_H_
#define _WIFI_ACTOR_H_
#include <actor.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <functional>
#include <vector>
#include <msgs.h>
#include <typed_message.h>

class WifiConnected : public ActorPayload
{
  TYPED_MESSAGE(WifiConnected)
};

class WifiDisconnected : public ActorPayload
{
  TYPED_MESSAGE(WifiDisconnected)
};

class WifiActor : public Actor
{
private:
  esp_netif_t *esp_netif;
  esp_event_handler_instance_t handler_any_id;
  esp_event_handler_instance_t handler_got_ip;
  bool _wifi_connected = false;
  // Delayed reconnect: set by the event handler (esp_event task), consumed by
  // the periodic timer in the EventBus loop task. Delaying the reconnect
  // avoids a rapid connect/disconnect loop that an immediate
  // esp_wifi_connect() inside the DISCONNECTED handler can cause.
  volatile bool _reconnect_pending = false;
  uint64_t _reconnect_at_ms = 0;
  int _timer_publish;
  int _timer_publish_props;
  int _prop_counter = 0;
  std::string wifi_ssid;
  std::string wifi_password;
  int channel;
  std::vector<std::string> ssid_list;
  std::shared_ptr<WifiConnected> _wifi_connected_msg = std::make_shared<WifiConnected>();
  std::shared_ptr<WifiDisconnected> _wifi_disconnected_msg = std::make_shared<WifiDisconnected>();
  std::shared_ptr<WifiEvent> _wifi_event = std::make_shared<WifiEvent>();
  

public:
  WifiActor();
  ~WifiActor();
  void receive(const ActorMessage &);
  void setup();
  void handle_timer(int id);
  Res net_init();
  Res wifi_set_config(const char *ssid, const char *password);
  void publish_info(esp_netif_t *esp_netif);

  static void event_handler(void *arg, esp_event_base_t event_base,
                            int32_t event_id, void *event_data);
  Res scan();
  Res connect();
};

std::string ip4addr_to_str(esp_ip4_addr_t *ip);

#endif
