#pragma once
// Shared compass UDP transport for ESP32 (Arduino).
//
// Same pattern as libs/uwb_proto/src/uwb_proto.h for the dwm1000 tag:
// a CBOR Envelope transport over UDP built on the generated msgs.h types.
//
//   - announces itself every 5 s (DeviceEvent)
//   - unicasts SysEvent / CompassEvent / ImuEvent to the central server
//   - listens for an incoming DeviceRequest to switch the server address

#include <Arduino.h>
#include <string.h>
#include <string>
#include <vector>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <msgs.h>
#include <fnv.h>

namespace compassproto {

#ifndef COMPASS_SERVER_IP
#define COMPASS_SERVER_IP 192,168,0,148
#endif
#ifndef COMPASS_MULTICAST_IP
#define COMPASS_MULTICAST_IP 239,0,0,23
#endif
#ifndef COMPASS_PORT
#define COMPASS_PORT 50000
#endif

const IPAddress SERVER_ADDR(COMPASS_SERVER_IP);
const uint16_t  SERVER_PORT = COMPASS_PORT;
const IPAddress MULTICAST_ADDR(COMPASS_MULTICAST_IP);
const uint16_t  MULTICAST_PORT = COMPASS_PORT;

class CompassProto {
public:
    void begin(const char* deviceName, const uint32_t* caps, size_t capCount) {
        _deviceName = deviceName;
        _src = FNV(deviceName);
        _server = SERVER_ADDR;
        _serverPort = SERVER_PORT;

        size_t n = (capCount > MAX_CAPS) ? MAX_CAPS : capCount;
        for (size_t i = 0; i < n; i++) _caps[i] = caps[i];
        _capCount = n;

        randomSeed(micros());
        uint16_t port = (uint16_t)random(20000, 60000);
        while (!_udp.begin(port)) {
            port++;
            if (port == 0) break;
        }
        _localPort = port;
        Serial.printf("UDP: ready as '%s' src=0x%08lX on port %u\n",
                      _deviceName, (unsigned long)_src, (unsigned)_localPort);
    }

    void setServer(const IPAddress& ip, uint16_t port) { _server = ip; _serverPort = port; }

    uint16_t localPort() const { return _localPort; }
    uint32_t src() const { return _src; }
    const IPAddress& server() const { return _server; }
    uint16_t serverPort() const { return _serverPort; }

    // Process one incoming packet (currently only DeviceRequest is handled).
    void loop() {
        int pktSize = _udp.parsePacket();
        if (pktSize <= 0) return;
        uint8_t buf[256];
        size_t n = (size_t)_udp.read(buf, sizeof(buf));
        Envelope envelope;
        Buffer rbuf(buf, n);
        rbuf.resize(n);  // non-owning Buffer ctor leaves _index=0; expose the data
        if (envelope.decode(rbuf) != 0) return;
        if (envelope.msg_type.has_value() && *envelope.msg_type == DeviceRequest::MSG_ID &&
            envelope.payload.has_value()) {
            DeviceRequest req;
            Buffer pbuf(*envelope.payload);
            if (req.decode(pbuf) == 0 && req.ip_address.has_value() && req.port.has_value()) {
                const std::vector<uint8_t>& ip = *req.ip_address;
                if (ip.size() == 4) {
                    _server = IPAddress(ip[0], ip[1], ip[2], ip[3]);
                    _serverPort = (uint16_t)*req.port;
                    Serial.printf("UDP: broker switched to %s:%u\n",
                                  _server.toString().c_str(), (unsigned)_serverPort);
                }
            }
        }
    }

    void sendAnnounce() {
        Envelope envelope;
        DeviceEvent event;
        Buffer envelope_buffer(128);
        Buffer payload_buffer(128);

        envelope.src = FNV(_deviceName);
        envelope.msg_type = DeviceEvent::MSG_ID;

        event.device_name = _deviceName;
        std::vector<uint32_t> caps(_caps, _caps + _capCount);
        event.request_types = caps;
        event.event_types = caps;

        event.encode(payload_buffer);
        envelope.payload = payload_buffer.to_vector();
        envelope.encode(envelope_buffer);

        sendTo(MULTICAST_ADDR, MULTICAST_PORT, envelope_buffer.data(), envelope_buffer.size(), true);
        sendTo(_server, _serverPort, envelope_buffer.data(), envelope_buffer.size(), false);
    }

    void sendSysEvent() {
        Envelope envelope;
        SysEvent event;
        Buffer envelope_buffer(128);
        Buffer payload_buffer(128);

        envelope.src = FNV(_deviceName);
        envelope.msg_type = SysEvent::MSG_ID;

        event.uptime = (uint64_t)millis();

        event.encode(payload_buffer);
        envelope.payload = payload_buffer.to_vector();
        envelope.encode(envelope_buffer);

        sendTo(_server, _serverPort, envelope_buffer.data(), envelope_buffer.size(), false);
    }

    void sendCompassEvent(float heading, float pitch, float roll,
                          float magX, float magY, float magZ,
                          float accX, float accY, float accZ) {
        Envelope envelope;
        CompassEvent event;
        Buffer envelope_buffer(128);
        Buffer payload_buffer(128);

        envelope.src = FNV(_deviceName);
        envelope.msg_type = CompassEvent::MSG_ID;

        event.heading = heading;
        event.pitch   = pitch;
        event.roll    = roll;
        event.mag_x   = magX;
        event.mag_y   = magY;
        event.mag_z   = magZ;
        event.accel_x = accX;
        event.accel_y = accY;
        event.accel_z = accZ;

        event.encode(payload_buffer);
        envelope.payload = payload_buffer.to_vector();
        envelope.encode(envelope_buffer);

        sendTo(_server, _serverPort, envelope_buffer.data(), envelope_buffer.size(), false);
    }

    void sendImuEvent(float gyroX, float gyroY, float gyroZ,
                      float accX, float accY, float accZ) {
        Envelope envelope;
        ImuEvent event;
        Buffer envelope_buffer(128);
        Buffer payload_buffer(128);

        envelope.src = FNV(_deviceName);
        envelope.msg_type = ImuEvent::MSG_ID;

        event.gyro_x  = gyroX;
        event.gyro_y  = gyroY;
        event.gyro_z  = gyroZ;
        event.accel_x = accX;
        event.accel_y = accY;
        event.accel_z = accZ;

        event.encode(payload_buffer);
        envelope.payload = payload_buffer.to_vector();
        envelope.encode(envelope_buffer);

        sendTo(_server, _serverPort, envelope_buffer.data(), envelope_buffer.size(), false);
    }

private:
    void sendTo(const IPAddress& ip, uint16_t port, const uint8_t* data, size_t len, bool multicast) {
        if (multicast) return;  // skip multicast on ESP32; unicast to server is the reliable path
        _udp.beginPacket(ip, port);
        _udp.write(data, len);
        _udp.endPacket();
    }

    enum { MAX_CAPS = 4 };

    const char* _deviceName = "";
    uint32_t    _src = 0;
    IPAddress   _server;
    uint16_t    _serverPort = SERVER_PORT;
    uint16_t    _localPort = 0;
    WiFiUDP     _udp;
    uint32_t    _caps[MAX_CAPS];
    size_t      _capCount = 0;
};

} // namespace compassproto
