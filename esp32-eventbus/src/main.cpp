// #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
// #pragma GCC diagnostic ignored "-Wunused-variable"

#include <optional>
#include <string>
#include <vector>

#include <esp_wifi.h>
#include <esp_coexist.h>
#include <esp_event.h>
#include "esp_mac.h"
#include <nvs_flash.h>

#include <actor.h>
#include <wifi_actor.h>
#include <sys_actor.h>
#include <led_actor.h>
#include <mc_actor.h>
#include <ota_actor.h>
#include <mdns_actor.h>
#include <hoverboard_actor.h>
#include <max31855_actor.h>
#include <log.h>
#include <cbor.h>

#ifndef DEVICE_NAME
#error "DEVICE_NAME not defined"
#endif

EventBus eventbus(200);
Log logger;
esp_err_t nvs_ota_init();
char hostname[24];
extern std::unordered_map<uint32_t, const char *> id_to_name;

void add_local_msgs() {
  id_to_name[TxdMsg::MSG_ID] = "TxdMsg";
  id_to_name[RxdMsg::MSG_ID] = "RxdMsg";
  id_to_name[WifiConnected::MSG_ID] = "WifiConnected";
  id_to_name[WifiDisconnected::MSG_ID] = "WifiDisconnected";
  id_to_name[LedBlink::MSG_ID] = "LedBlink";
  id_to_name[LedOn::MSG_ID] = "LedOn";
  id_to_name[LedOff::MSG_ID] = "LedOff";
  id_to_name[LedPulse::MSG_ID] = "LedPulse";
  id_to_name[TimerMsg::MSG_ID] = "TimerMsg";
  id_to_name[UartRxd::MSG_ID] = "UartRxd";
  id_to_name[Transmitting::MSG_ID] = "Transmitting";
  id_to_name[Max31855Read::MSG_ID] = "Max31855Read";
}


static CborError print_to_buffer(void *token, const char *fmt, ...) {
    char **out_ptr = (char **)token;
    va_list args;
    va_start(args, fmt);
    int written = vsprintf(*out_ptr, fmt, args);
    va_end(args);
    if (written < 0) return CborErrorIO;
    *out_ptr += written; // Advance the pointer in the buffer
    return CborNoError;
}

void print_cbor_diagnostic(const uint8_t *cbor_buf, size_t buf_len) {
    CborParser parser;
    CborValue value;
    
    if (cbor_parser_init(cbor_buf, buf_len, 0, &parser, &value) != CborNoError) {
        return;
    }

    char string_output[512] = {0};
    char *cursor = string_output;

    // Convert CBOR to diagnostic text
    cbor_value_to_pretty_stream(print_to_buffer, &cursor, &value, CborPrettyDefaultFlags);

    INFO("CBOR : %s", string_output);
}


extern "C" void app_main()
{

  ESP_ERROR_CHECK(nvs_ota_init());

  INFO("Free heap size: %ld ", esp_get_free_heap_size());
  INFO("Stack high water mark: %ld \n", uxTaskGetStackHighWaterMark(NULL));
  // get the mac address for logging and mdns
  uint8_t mac[6];
  esp_err_t ret = esp_read_mac(mac, ESP_MAC_WIFI_STA);
  INFO("Device MAC address: %02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  snprintf(hostname, sizeof(hostname), "%s-%02X%02X%02X", DEVICE_NAME, mac[3], mac[4], mac[5]);

  eventbus.register_actor(new WifiActor("wifi"));           // manage wifi connection, will block on start until connected
  eventbus.register_actor(new SysActor("sys"));             // manage the system
  eventbus.register_actor(new McActor("mc", hostname));     // multicast actor
  eventbus.register_actor(new LedActor("led"));             // blink the led
  eventbus.register_actor(new OtaActor("ota"));             // ota via tftp
  eventbus.register_actor(new MdnsActor("mdns", hostname)); // mdns service
#ifdef ENABLE_DESOLDERING
  eventbus.register_actor(new Max31855Actor(hostname, 500)); // MAX31855 + PID + SSR heating control
#endif
#ifdef ENABLE_HOVERBOARD
  eventbus.register_actor(new HoverboardActor("hb")); // hoverboard interface
#endif
  // debugging handler to log all eventbus traffic, comment for beauty
  eventbus.register_handler([](const Msg &msg) // just log eventbus traffic
                            { 
                              uint32_t msg_id = msg.msg_id() ? msg.msg_id() : 0;
                              INFO("EventBus '%s' (%d) Free heap size: %ld", id_to_string(msg_id), msg_id, esp_get_free_heap_size());
                            });
  eventbus.loop();
}
/*
Initializes the non-volatile storage (NVS) and,
if necessary, erases and re-initializes it when specific errors are detected.
Additionally, it reports the currently running OTA partition and attempts to mark the current application as valid,
canceling any pending rollback if successful.
*/
esp_err_t nvs_ota_init()
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  // 2. Report Current Partition
  const esp_partition_t *running = esp_ota_get_running_partition();
  INFO("Running from partition: %s", running->label);
  if (esp_ota_mark_app_valid_cancel_rollback() == ESP_OK)
  {
    INFO("Current app marked as valid, rollback cancelled");
  }
  else
  {
    WARN("Failed to mark current app as valid");
  }
  return ret;
}
