/**
 * ESP8266 + DWM1000 UWB Distance Measuring & UDP Reporter
 * Mode: Tag (Two-Way Ranging Initiator)
 *
 * Reports UWB tag events over UDP using the protobuf wire format from
 * msgs.proto (see uwb_proto.h). The MQTT/JSON reporter was replaced by:
 *   - a multicast + unicast Announce every 5 s
 *   - a SysEvent every 30 s
 *   - a UwbTagEvent per anchor every 250 ms (4 Hz)
 */

#include <SPI.h>
#include <DW1000Ranging.h>
#include <ArduinoOTA.h>
#include "uwb_proto.h"

// ==========================================
// 1. PIN CONFIGURATION
// ==========================================
// Hardware SPI Pins on ESP8266:
//   MOSI -> GPIO13 (D7)
//   MISO -> GPIO12 (D6)
//   SCK  -> GPIO14 (D5)
const uint8_t PIN_RST = 5;    // Reset Pin        -> GPIO5 (D1)   [NOT GPIO1/TX]
const uint8_t PIN_SS  = 15;   // SPI Chip Select  -> GPIO15 (D8)
const uint8_t PIN_IRQ = 4;    // DW1000 Interrupt -> GPIO4 (D2)

// ==========================================
// LED INDICATORS (non-blocking, do not disturb DWM1000 timing)
// ==========================================
const uint8_t PIN_LED1 = 2;    // heartbeat LED: blinks at 2 Hz (HIGH = on)
const uint8_t PIN_LED2 = 16;   // UWB-activity LED: 1 ms pulse on received packet (HIGH = on)
const uint8_t LED_ON  = LOW;    // LEDs are active-low: LOW turns them on
const uint8_t LED_OFF = HIGH;
unsigned long lastLed1ToggleMs = 0;
bool led1State = false;
unsigned long lastUwbPacketUs = 0;

// ==========================================
// 2. NETWORK SETTINGS
// ==========================================
// WiFi credentials are injected at build time from the environment
// (WIFI_SSID / WIFI_PASS) via `build_flags` in platformio.ini.
#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif
#ifndef WIFI_PASS
#define WIFI_PASS ""
#endif
#ifndef OTA_PASS
#define OTA_PASS ""
#endif

void setupOTA(const char* hostname) {
  ArduinoOTA.setHostname(hostname);
  if (strlen(OTA_PASS) > 0) {
    ArduinoOTA.setPassword(OTA_PASS);
  }
  ArduinoOTA.onStart([]() { Serial.println("OTA: update started"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nOTA: update finished"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA: error[%u]\n", error);
  });
  ArduinoOTA.begin();
  Serial.println("OTA: ready at hostname " + String(hostname));
}

// ==========================================
// 3. DEVICE IDENTITY
// ==========================================
const char* DEVICE_NAME = "uwb_tag_1";

// Tag UWB Unique MAC Address (8-byte EUI-64)
char TAG_ADDR[] = "7D:00:22:EA:82:60:3B:9C";

// ==========================================
// 4. RANGING STATE
// ==========================================
// Reporting interval (4 Hz) + announce/sys-event cadence.
const unsigned long ANNOUNCE_INTERVAL_MS = 5000;
const unsigned long SYSEVENT_INTERVAL_MS = 30000;
const unsigned long REPORT_INTERVAL_MS   = 250;
unsigned long lastAnnounceMs = 0;
unsigned long lastSysEventMs  = 0;
unsigned long lastReportMs    = 0;

// Measurement state — track the latest distance to each anchor separately.
#define MAX_ANCHORS 6
// Reject obviously-bogus range readings (occasional ±100 m glitches from
// timestamp errors / multipath). Adjust to your setup.
#define MIN_DISTANCE_M (-1.0f)
#define MAX_DISTANCE_M (120.0f)  // field up to 100 m + margin
// Reject a reading that jumps more than this from the anchor's last valid value.
#define JUMP_THRESHOLD_M (2.0f)
// accept a 'jump' after this many consecutive rejections (the old value was
// the outlier, or the tag really moved while the anchor was out of reach)
#define JUMP_ACCEPT_COUNT 3
struct AnchorDist {
  uint16_t shortAddr;
  float distanceM;
  float rxPowerDbm;
  uint8_t rejected;  // consecutive jump-filter rejections
  bool valid;
};
AnchorDist anchors[MAX_ANCHORS];

Log logger;  // global logger instance required by common/msg logging macros
uwbproto::UwbProto proto;

// ==========================================
// 5. DW1000 UWB CALLBACKS
// ==========================================
void newRange() {
  lastUwbPacketUs = micros();   // UWB activity (non-blocking)

  uint16_t addr = DW1000Ranging.getDistantDevice()->getShortAddress();
  float d = DW1000Ranging.getDistantDevice()->getRange();
  float rx = DW1000Ranging.getDistantDevice()->getRXPower();

  // Reject implausible readings; keep the previous valid value for this anchor.
  if (d < MIN_DISTANCE_M || d > MAX_DISTANCE_M) {
    return;
  }

  int slot = -1;
  for (uint8_t i = 0; i < MAX_ANCHORS; i++) {
    if (anchors[i].valid && anchors[i].shortAddr == addr) { slot = i; break; }
  }
  if (slot < 0) {
    for (uint8_t i = 0; i < MAX_ANCHORS; i++) {
      if (!anchors[i].valid) { slot = i; break; }
    }
  }
  if (slot >= 0) {
    // Jump filter: ignore a reading that deviates wildly from the last valid one.
    float delta = d - anchors[slot].distanceM;
    if (delta < 0) delta = -delta;
    if (anchors[slot].valid && delta > JUMP_THRESHOLD_M &&
        ++anchors[slot].rejected < JUMP_ACCEPT_COUNT) {
      return;  // keep the previous valid value
    }
    anchors[slot].rejected = 0;
    anchors[slot].shortAddr = addr;
    anchors[slot].distanceM = d;
    anchors[slot].rxPowerDbm = rx;
    anchors[slot].valid = true;
  }
}

void newBlink(DW1000Device* device) {
  Serial.print("UWB: Found new anchor short address: 0x");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("UWB: Anchor inactive: 0x");
  Serial.println(device->getShortAddress(), HEX);
}

// ==========================================
// 6. WI-FI
// ==========================================
void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("WiFi: Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi: Connected! IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi: Failed to connect within timeout!");
  }
}

// ==========================================
// 7. MAIN SETUP & LOOP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ESP8266 + DWM1000 UWB Ranging Tag (UDP) ===");

  // LED indicator pins
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  setupWiFi();
  setupOTA("uwb-tag1");

  // UDP reporter (random port), then announce ourselves immediately.
  uint8_t mac8[8];
  if (uwbproto::parseMacHex(TAG_ADDR, mac8)) {
    proto.setUwbMac(mac8, 8);
  }
  static const uint32_t caps[] = { SysEvent::MSG_ID, DW1000TagEvent::MSG_ID };
  proto.begin(DEVICE_NAME, caps, 2);
  proto.sendAnnounce();

  // Initialize DWM1000 SPI & Pins
  Serial.println("UWB: Initializing DWM1000...");
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);

  // Register callbacks
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  // Antenna delay calibration (tune per hardware board)
  DW1000.setAntennaDelay(16422);  // calibrated: 16436 read ~-13cm, 16381 read ~+37cm @10cm

  // Start ranging as Tag (Initiator) in high-accuracy long-data mode
  DW1000Ranging.startAsTag(TAG_ADDR, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
  Serial.println("UWB: Tag initiator active!");
}

void loop() {
  unsigned long now = millis();

  // LED1: 2 Hz heartbeat (non-blocking, 250 ms half-period)
  if (now - lastLed1ToggleMs >= 250) {
    lastLed1ToggleMs = now;
    led1State = !led1State;
    digitalWrite(PIN_LED1, led1State ? LED_ON : LED_OFF);
  }

  // LED2: 1 ms pulse after a UWB packet (non-blocking)
  if (lastUwbPacketUs != 0 && (micros() - lastUwbPacketUs) < 1000) {
    digitalWrite(PIN_LED2, LED_ON);
  } else {
    digitalWrite(PIN_LED2, LED_OFF);
  }

  // 1. Service DWM1000 hardware state machine
  DW1000Ranging.loop();
  ArduinoOTA.handle();

  // 2. Service incoming UDP (SetBroker)
  proto.loop();

  // 3. Periodic UDP reporting
  if (WiFi.status() == WL_CONNECTED) {
    if (now - lastAnnounceMs >= ANNOUNCE_INTERVAL_MS) {
      lastAnnounceMs = now;
      proto.sendAnnounce();
    }
    if (now - lastSysEventMs >= SYSEVENT_INTERVAL_MS) {
      lastSysEventMs = now;
      proto.sendSysEvent();
    }
    if (now - lastReportMs >= REPORT_INTERVAL_MS) {
      lastReportMs = now;
      for (uint8_t i = 0; i < MAX_ANCHORS; i++) {
        if (anchors[i].valid) {
          // responder short address, big-endian (matches "0x%04X" display)
          uint8_t as[2] = {
            (uint8_t)(anchors[i].shortAddr >> 8),
            (uint8_t)(anchors[i].shortAddr & 0xff),
          };
          proto.sendUwbTagEvent(as, anchors[i].distanceM, anchors[i].rxPowerDbm);
        }
      }
    }
  }
}