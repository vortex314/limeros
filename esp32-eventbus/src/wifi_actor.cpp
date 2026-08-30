#include <esp_wifi.h>
#include <freertos/event_groups.h>
#include <string.h>
#include <strings.h>
#include <wifi_actor.h>
#include <esp_mac.h>
#include <util.h>

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "regex.h"

#ifndef WIFI_PASS
#error "WIFI_PASS not defined"
#endif

#ifndef WIFI_SSID
#error "WIFI_SSID not defined"
#endif

#define ESP_MAXIMUM_RETRY 5

static int s_retry_count = 0;

WifiActor::WifiActor() : Actor()
{
  _timer_publish = timer_repetitive(1000);
  //  _timer_publish_props = timer_repetitive(5000);
  wifi_ssid = "";
  wifi_password = STRINGIZE(WIFI_PASS);
  // esp_coex_preference_set(ESP_COEX_PREFER_BALANCE);
}

void WifiActor::setup()
{


  while (true)
  {
    auto r = scan();
    if (r.is_ok())
    {
      INFO("Scanned SSID: %s", wifi_ssid.c_str());
      break;
    }
    if (r.is_err())
    {
      INFO("Failed to scan: %s", r.err()->msg);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

  if (wifi_ssid.empty())
  {
    INFO("Set default SSID : %s", STRINGIZE(WIFI_SSID));
    wifi_ssid = STRINGIZE(WIFI_SSID);
  }
  else
  {
    INFO("Set strongest SSID : %s", wifi_ssid.c_str());
  }
  auto r = connect();
  if (r.is_err())
  {
    INFO("Failed to connect to WiFi %d:%s", r.err()->rc, r.err()->msg);
    return;
  }
}

void WifiActor::receive(const ActorMessage &msg)
{
  msg.handle<TimerMsg>([&](const TimerMsg &msg)
                       { handle_timer(msg.timer_id); });
}

void WifiActor::handle_timer(int timer_id)
{
  if (timer_id == _timer_publish)
  {
    if (_reconnect_pending && current_time() >= _reconnect_at_ms)
    {
      _reconnect_pending = false;
      s_retry_count++;
      esp_wifi_connect();
    }
    if (_wifi_connected)
    {
      publish_info(esp_netif);
    }
  }
  else if (timer_id == _timer_publish_props)
  {
    INFO("Timer 2 : Publishing WiFi properties info");
    if (_wifi_connected)
    {
      INFO("Publishing WiFi properties info");
      //      publish_props_info();
    }
  }
  else
  {
    INFO("Unknown timer id: %d", timer_id);
  }
}

/*Res WifiActor::publish_props_info()
{
  if (!_wifi_connected)
  {
    return Res(ENOTCONN, "Not connected to WiFi");
  }
  emit(WifiEvent{.prop_info = PublishSerdes(info_props_wifi[_prop_counter])});
  _prop_counter = (_prop_counter + 1) % (sizeof(info_props_wifi) / sizeof(InfoProp));
  return ResOk;
}*/

WifiActor::~WifiActor()
{
  INFO("Stopping WiFi actor");
}

void WifiActor::event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data)
{
  WifiActor *actor = (WifiActor *)arg;
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
  {
    INFO("WiFi STA started");
    esp_wifi_connect();
  }
  else if (event_base == WIFI_EVENT &&
           event_id == WIFI_EVENT_STA_DISCONNECTED)
  {
    wifi_event_sta_disconnected_t *dis =
        (wifi_event_sta_disconnected_t *)event_data;
    WARN("WiFi STA disconnected, reason=%d",
         dis ? (int)dis->reason : -1);
    actor->send(actor->_wifi_disconnected_msg);
    actor->_wifi_connected = false;
    if (s_retry_count < ESP_MAXIMUM_RETRY)
    {
      // Reconnect on the periodic timer (delayed) instead of immediately:
      // calling esp_wifi_connect() here can race the driver and keep the
      // connect/disconnect loop going.
      actor->_reconnect_pending = true;
      actor->_reconnect_at_ms = current_time() + 1000;
    }
  }
  else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
  {
    INFO("WiFi STA got IP address ");
    actor->emit(actor->_wifi_connected_msg);
    actor->_wifi_connected = true;
    s_retry_count = 0;
  }
}

std::string ip4addr_to_str(esp_ip4_addr_t *ip)
{
  char buf[17];
  uint8_t *ip_parts = (uint8_t *)ip;
  snprintf(buf, sizeof(buf), "%u.%u.%u.%u",
           ip_parts[0], ip_parts[1],
           ip_parts[2], ip_parts[3]);
  return buf;
}

#define DEFAULT_SCAN_LIST_SIZE 10

#ifdef CONFIG_EXAMPLE_USE_SCAN_CHANNEL_BITMAP
#define USE_CHANNEL_BITMAP 1
#define CHANNEL_LIST_SIZE 3
static uint8_t channel_list[CHANNEL_LIST_SIZE] = {1, 6, 11};
#endif /*CONFIG_EXAMPLE_USE_SCAN_CHANNEL_BITMAP*/

static const char *authmode_to_str(int authmode)
{
  switch (authmode)
  {
  case WIFI_AUTH_OPEN:
    return ("Authmode WIFI_AUTH_OPEN");
    break;
  case WIFI_AUTH_OWE:
    return ("Authmode WIFI_AUTH_OWE");
    break;
  case WIFI_AUTH_WEP:
    return ("Authmode WIFI_AUTH_WEP");
    break;
  case WIFI_AUTH_WPA_PSK:
    return ("Authmode WIFI_AUTH_WPA_PSK");
    break;
  case WIFI_AUTH_WPA2_PSK:
    return ("Authmode WIFI_AUTH_WPA2_PSK");
    break;
  case WIFI_AUTH_WPA_WPA2_PSK:
    return ("Authmode WIFI_AUTH_WPA_WPA2_PSK");
    break;
  case WIFI_AUTH_ENTERPRISE:
    return ("Authmode WIFI_AUTH_ENTERPRISE");
    break;
  case WIFI_AUTH_WPA3_PSK:
    return ("Authmode WIFI_AUTH_WPA3_PSK");
    break;
  case WIFI_AUTH_WPA2_WPA3_PSK:
    return ("Authmode WIFI_AUTH_WPA2_WPA3_PSK");
    break;
  case WIFI_AUTH_WPA3_ENT_192:
    return ("Authmode WIFI_AUTH_WPA3_ENT_192");
    break;
  default:
    return ("Authmode WIFI_AUTH_UNKNOWN");
    break;
  }
}

static const char *cipher_type_to_str(int pairwise_cipher)
{
  switch (pairwise_cipher)
  {
  case WIFI_CIPHER_TYPE_NONE:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_NONE");
    break;
  case WIFI_CIPHER_TYPE_WEP40:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_WEP40");
    break;
  case WIFI_CIPHER_TYPE_WEP104:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_WEP104");
    break;
  case WIFI_CIPHER_TYPE_TKIP:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_TKIP");
    break;
  case WIFI_CIPHER_TYPE_CCMP:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_CCMP");
    break;
  case WIFI_CIPHER_TYPE_TKIP_CCMP:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_TKIP_CCMP");
    break;
  case WIFI_CIPHER_TYPE_AES_CMAC128:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_AES_CMAC128");
    break;
  case WIFI_CIPHER_TYPE_SMS4:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_SMS4");
    break;
  case WIFI_CIPHER_TYPE_GCMP:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_GCMP");
    break;
  case WIFI_CIPHER_TYPE_GCMP256:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_GCMP256");
    break;
  default:
    return ("Pairwise Cipher WIFI_CIPHER_TYPE_UNKNOWN");
    break;
  }
}

static const char *group_cipher_to_str(int group_cipher)
{
  switch (group_cipher)
  {
  case WIFI_CIPHER_TYPE_NONE:
    return ("Group Cipher WIFI_(sta_netif);CIPHER_TYPE_NONE");
    break;
  case WIFI_CIPHER_TYPE_WEP40:
    return ("Group Cipher WIFI_CIPHER_TYPE_WEP40");
    break;
  case WIFI_CIPHER_TYPE_WEP104:
    return ("Group Cipher WIFI_CIPHER_TYPE_WEP104");
    break;
  case WIFI_CIPHER_TYPE_TKIP:
    return ("Group Cipher WIFI_CIPHER_TYPE_TKIP");
    break;
  case WIFI_CIPHER_TYPE_CCMP:
    return ("Group Cipher WIFI_CIPHER_TYPE_CCMP");
    break;
  case WIFI_CIPHER_TYPE_TKIP_CCMP:
    return ("Group Cipher WIFI_CIPHER_TYPE_TKIP_CCMP");
    break;
  case WIFI_CIPHER_TYPE_SMS4:
    return ("Group Cipher WIFI_CIPHER_TYPE_SMS4");
    break;
  case WIFI_CIPHER_TYPE_GCMP:
    return ("Group Cipher WIFI_CIPHER_TYPE_GCMP");
    break;
  case WIFI_CIPHER_TYPE_GCMP256:
    return ("Group Cipher WIFI_CIPHER_TYPE_GCMP256");
    break;
  default:
    return ("Group Cipher WIFI_CIPHER_TYPE_UNKNOWN");
    break;
  }
}

/* Initialize Wi-Fi as sta and set scan method */
Res WifiActor::scan()
{

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_netif = esp_netif_create_default_wifi_sta();
  assert(esp_netif);

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());

  uint16_t number = DEFAULT_SCAN_LIST_SIZE;
  wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
  uint16_t ap_count = 0;
  memset(ap_info, 0, sizeof(ap_info));

  wifi_scan_config_t scan_config;
  memset(&scan_config, 0, sizeof(scan_config));
  scan_config.bssid = NULL;
  scan_config.channel = 0;
  scan_config.show_hidden = true;
  scan_config.scan_type = WIFI_SCAN_TYPE_ACTIVE;
  scan_config.scan_time.active.min = 100;
  scan_config.scan_time.active.max = 200;
  scan_config.scan_time.passive = 0;
  scan_config.home_chan_dwell_time = WIFI_SCAN_HOME_CHANNEL_DWELL_DEFAULT_TIME;

  CHECK_ESP(esp_wifi_scan_start(&scan_config, true));

  INFO("Max AP number ap_info can hold = %u", number);
  CHECK_ESP(esp_wifi_scan_get_ap_num(&ap_count));
  CHECK_ESP(esp_wifi_scan_get_ap_records(&number, ap_info));
  INFO("Total APs scanned = %u, actual AP number ap_info holds = %u", ap_count, number);
  int8_t rssi_highest = -128;
  int ap_index = 0;
  for (int i = 0; i < number; i++)
  {
    INFO("Channel %d SSID '%s' RSSI %d %s %s %s %s",
         ap_info[i].primary,
         ap_info[i].ssid,
         ap_info[i].rssi, authmode_to_str(ap_info[i].authmode),
         ap_info[i].authmode != WIFI_AUTH_WEP ? "" : cipher_type_to_str(ap_info[i].pairwise_cipher),
         ap_info[i].authmode != WIFI_AUTH_WEP ? "" : group_cipher_to_str(ap_info[i].group_cipher),
         group_cipher_to_str(ap_info[i].group_cipher));

    std::string ssid = std::string((const char *)ap_info[i].ssid);
    if (ap_info[i].rssi > rssi_highest && ssid.rfind("Merckx", 0) == 0)
    {
      rssi_highest = ap_info[i].rssi;
      ap_index = i;
    }
  }
  wifi_ssid = std::string((const char *)ap_info[ap_index].ssid);
  ESP_ERROR_CHECK(esp_wifi_scan_stop());
  ESP_ERROR_CHECK(esp_wifi_clear_ap_list());
  ESP_ERROR_CHECK(esp_wifi_stop());
  ESP_ERROR_CHECK(esp_wifi_deinit());
  return Res::Ok(true);
}

Res WifiActor::connect()
{
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  CHECK_ESP(esp_wifi_init(&cfg));

  // Disable power save (modem sleep) NOW that WiFi is initialized. Doing it in
  // the constructor was a no-op (WiFi not initialized yet), which left the STA
  // in modem-sleep and caused repeated connect/disconnect cycles.
  esp_wifi_set_ps(WIFI_PS_NONE);

  wifi_config_t wifi_config;
  bzero(&wifi_config, sizeof(wifi_config));
  strcpy((char *)wifi_config.sta.ssid, wifi_ssid.c_str());
  strcpy((char *)wifi_config.sta.password, wifi_password.c_str());
  INFO("Setting WiFi configuration SSID '%s'", wifi_config.sta.ssid);

  // Correct ESP-IDF order: set the mode first, then the config.
  CHECK_ESP(esp_wifi_set_mode(WIFI_MODE_STA));
  CHECK_ESP(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, this, &handler_any_id);
  esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, this, &handler_got_ip);
  CHECK_ESP(esp_wifi_start());

  return Res::Ok(true);
}

void WifiActor::publish_info(esp_netif_t *esp_netif)
{
  
  // get IP address and publish
  esp_netif_ip_info_t ip_info;
  if (esp_netif_get_ip_info(esp_netif, &ip_info) != ESP_OK)
  {
    ERROR("Failed to get IP info");
    return;
  }
  uint8_t primary;
  wifi_second_chan_t second;

  if (esp_wifi_get_channel(&primary, &second) != ESP_OK)
  {
    ERROR("Failed to get WiFi channel");
    return;
  }
  channel = primary;
  _wifi_event->ip = ip4addr_to_str(&ip_info.ip);
  _wifi_event->gateway = ip4addr_to_str(&ip_info.gw);
  _wifi_event->netmask = ip4addr_to_str(&ip_info.netmask);
  _wifi_event->ssid = wifi_ssid;
  _wifi_event->channel = channel;
  // get MAC address
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2],
          mac[3], mac[4], mac[5]);
  _wifi_event->mac = macStr;
  emit(_wifi_event);
}
