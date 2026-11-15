/**
 * ESP32 UEXT Compass + IMU (Arduino)
 *
 * Reads HMC5883L (magnetometer) and MPU6050 (accel + gyro) on the UEXT1 I2C
 * bus, computes a tilt-compensated heading (Kalman-fused pitch/roll), and
 * reports CompassEvent + ImuEvent over UDP using the shared CBOR transport
 * (libs/protocol msgs.h), mirroring the dwm1000 tag reporting pattern.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <uext.h>
#include <compass.h>
#include <compass_proto.h>

Log logger;  // global logger required by common/msg logging macros

// ==========================================
// 1. NETWORK SETTINGS (injected via build_flags)
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

// ==========================================
// 2. DEVICE IDENTITY
// ==========================================
const char* DEVICE_NAME = "compass";

// ==========================================
// 3. HARDWARE
// ==========================================
const uint8_t PIN_LED  = 2;    // heartbeat LED (active-low on many boards)
const uint8_t LED_ON   = LOW;
const uint8_t LED_OFF  = HIGH;

Uext uext1(1);                        // HMC5883L + MPU6050 live on UEXT1 I2C
CompassSensor compass(uext1);         // sensors on UEXT1 I2C bus
compassproto::CompassProto proto;

// ==========================================
// 4. TIMING
// ==========================================
const unsigned long ANNOUNCE_INTERVAL_MS = 5000;
const unsigned long SYSEVENT_INTERVAL_MS = 30000;
const unsigned long REPORT_INTERVAL_MS   = 250;   // 4 Hz
unsigned long lastAnnounceMs = 0;
unsigned long lastSysEventMs  = 0;
unsigned long lastReportMs    = 0;
unsigned long lastLedToggleMs = 0;
bool ledState = false;

// ==========================================
// 5. WIFI + OTA
// ==========================================
void setupWiFi()
{
    delay(10);
    Serial.print("WiFi: connecting to ");
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
        Serial.printf("\nWiFi: connected! IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("\nWiFi: failed to connect within timeout");
    }
}

void setupOTA(const char* hostname)
{
    ArduinoOTA.setHostname(hostname);
    if (strlen(OTA_PASS) > 0) ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.onStart([]() { Serial.println("OTA: update started"); });
    ArduinoOTA.onEnd([]() { Serial.println("\nOTA: update finished"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA: error[%u]\n", error);
    });
    ArduinoOTA.begin();
    Serial.printf("OTA: ready at hostname %s\n", hostname);
}

// ==========================================
// 6. SETUP / LOOP
// ==========================================
void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== ESP32 UEXT Compass + IMU (UDP) ===");

    pinMode(PIN_LED, OUTPUT);

    setupWiFi();
    setupOTA("compass");

    uext1.dump();

    if (compass.begin()) {
        Serial.println("Compass: HMC5883L + MPU6050 ready");
    } else {
        Serial.println("Compass: sensor init failed (check UEXT1 wiring)");
    }

    static const uint32_t caps[] = {
        SysEvent::MSG_ID,
        CompassEvent::MSG_ID,
        ImuEvent::MSG_ID,
    };
    proto.begin(DEVICE_NAME, caps, 3);
    proto.sendAnnounce();
}

void loop()
{
    unsigned long now = millis();

    // Heartbeat LED @ 2 Hz.
    if (now - lastLedToggleMs >= 250) {
        lastLedToggleMs = now;
        ledState = !ledState;
        digitalWrite(PIN_LED, ledState ? LED_ON : LED_OFF);
    }

    proto.loop();
    ArduinoOTA.handle();

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
            compass.read();

            proto.sendCompassEvent(
                compass.heading(), compass.pitch(), compass.roll(),
                compass.magX(), compass.magY(), compass.magZ(),
                compass.accelX(), compass.accelY(), compass.accelZ());

            proto.sendImuEvent(
                compass.gyroX(), compass.gyroY(), compass.gyroZ(),
                compass.accelX(), compass.accelY(), compass.accelZ());
        }
    }
}
