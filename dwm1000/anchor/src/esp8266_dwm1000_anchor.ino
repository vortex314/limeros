/**
 * ESP8266 + DWM1000 UWB Ranging Anchor (Responder) — UDP reporter
 *
 * Reports over UDP using the protobuf wire format from msgs.proto
 * (see uwb_proto.h):
 *   - a multicast + unicast Announce every 5 s
 *   - a SysEvent + UwbAnchorEvent every 30 s
 */

#include <SPI.h>
#include <DW1000Ranging.h>
#include <ArduinoOTA.h>
#include "uwb_proto.h"
#include "msgs.h"

// ==========================================
// PIN CONFIGURATION
// ==========================================
// Hardware SPI Pins:
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
// NETWORK SETTINGS
// ==========================================
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
// DEVICE IDENTITY
// ==========================================
// ANCHOR_ID is set per environment in platformio.ini (pio run -e anchorN).
#ifndef ANCHOR_ID
#error "ANCHOR_ID not defined: build with e.g. `pio run -e anchor1`"
#endif
#if ANCHOR_ID < 1 || ANCHOR_ID > 99
#error "ANCHOR_ID must be in 1..99"
#endif
// Antenna delay (DW1000 time units, 15.65 ps). Calibrated per anchor in
// platformio.ini: +1 unit shortens this anchor's measured range by ~4.7 mm.
#ifndef ANTENNA_DELAY
#define ANTENNA_DELAY 16456
#endif
#define ANCHOR_STR_(x) #x
#define ANCHOR_STR(x) ANCHOR_STR_(x)

const char* DEVICE_NAME = "uwb_anchor_" ANCHOR_STR(ANCHOR_ID);
const char* OTA_HOSTNAME = "uwb-anchor" ANCHOR_STR(ANCHOR_ID);

// Unique Anchor UWB Address (8-byte EUI-64); last byte = 0x9B + ANCHOR_ID
// (anchor1 = ...:E2:9C, anchor5 = ...:E2:A0). Filled in at the start of setup().
char ANCHOR_ADDR[] = "82:17:5B:D5:A9:9A:E2:00";

// ==========================================
// REPORTING CADENCE
// ==========================================
const unsigned long ANNOUNCE_INTERVAL_MS = 5000;
const unsigned long SYSEVENT_INTERVAL_MS = 30000;
unsigned long lastAnnounceMs = 0;
unsigned long lastSysEventMs  = 0;
unsigned long lastRangeMs = 0;   // set in newRange(), for serial "ranging" note

Log logger;  // global logger instance required by common/msg logging macros
uwbproto::UwbProto proto;

void newRange() {
  lastUwbPacketUs = micros();   // UWB activity (non-blocking)
  if (millis() - lastRangeMs < 500) return;  // throttle serial to avoid blocking loop()
  lastRangeMs = millis();
  Serial.print("Anchor: Ranging active with Tag 0x");
  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Distance: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getRange());
  Serial.print(" m\t RX Power: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
  Serial.println(" dBm");
}

void newBlink(DW1000Device* device) {
  Serial.print("Anchor: Detected Tag blink 0x");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("Anchor: Tag inactive: 0x");
  Serial.println(device->getShortAddress(), HEX);
}

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("WiFi: Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
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

void setup() {
  Serial.begin(115200);
  delay(1000);
  snprintf(ANCHOR_ADDR + 21, 3, "%02X", 0x9B + ANCHOR_ID);
  Serial.println("\n=== ESP8266 + DWM1000 UWB Anchor Responder (UDP) ===");
  Serial.printf("Anchor %d: %s  addr %s\n", ANCHOR_ID, DEVICE_NAME, ANCHOR_ADDR);

  // LED indicator pins
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  setupWiFi();
  setupOTA(OTA_HOSTNAME);

  // UDP reporter (random port), announce + identify immediately.
  uint8_t mac8[8];
  if (uwbproto::parseMacHex(ANCHOR_ADDR, mac8)) {
    proto.setUwbMac(mac8, 8);
  }
  static const uint32_t caps[] = { SysEvent::MSG_ID, DW1000AnchorEvent::MSG_ID };
  proto.begin(DEVICE_NAME, caps, 2);
  proto.sendAnnounce();
  proto.sendUwbAnchorEvent();

  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);

  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  // Antenna delay calibration
  DW1000.setAntennaDelay(ANTENNA_DELAY);  // per anchor, see platformio.ini

  // Start as Anchor in high-accuracy mode
  DW1000Ranging.startAsAnchor(ANCHOR_ADDR, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
  Serial.println("Anchor started and listening for ranging tags...");
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

  DW1000Ranging.loop();
  ArduinoOTA.handle();
  proto.loop();  // incoming SetBroker

  if (WiFi.status() == WL_CONNECTED) {
    if (now - lastAnnounceMs >= ANNOUNCE_INTERVAL_MS) {
      lastAnnounceMs = now;
      proto.sendAnnounce();
    }
    if (now - lastSysEventMs >= SYSEVENT_INTERVAL_MS) {
      lastSysEventMs = now;
      proto.sendSysEvent();
      proto.sendUwbAnchorEvent();
    }
  }
  if (now - lastSysEventMs == 0 && DW1000Ranging.getIrqRecoveries()) {
    Serial.printf("UWB: stuck IRQ recovered %lu times\n", (unsigned long)DW1000Ranging.getIrqRecoveries());
  }
}