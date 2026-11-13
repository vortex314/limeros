#pragma once
// Shared UWB UDP transport for ESP8266 / ESP32.
//
// Implements the protobuf wire format from msgs.proto by hand (no nanopb),
// plus a small UwbProto class that:
//   - opens a UDP socket on a random port
//   - multicasts an Announce every 5 s (and unicasts a copy to the server)
//   - unicasts SysEvent / UwbAnchorEvent / UwbTagEvent to the central server
//   - listens for an incoming SetBroker to switch the server address
//
// Keep this file in sync across tag/, tag_esp32/ and anchor/..anchor4/.

#include <Arduino.h>
#include <string.h>
#include <string>
#include <vector>
#include <msgs.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "uwb_proto.h supports ESP8266 and ESP32 only"
#endif
#include <fnv.h>

namespace uwbproto {

// --- msg_type values (match msgs.proto / protocol.md) -----------------------
enum MsgType : uint32_t {
  MSG_UNKNOWN          = 0,
  MSG_ANNOUNCE         = 1,
  MSG_SET_BROKER       = 2,
  MSG_SYS_EVENT        = 3,
  MSG_UWB_ANCHOR_EVENT = 4,
  MSG_UWB_TAG_EVENT    = 5,
};

// --- network defaults, overridable via platformio.ini build_flags ----------
#ifndef UWB_SERVER_IP
#define UWB_SERVER_IP 192,168,0,148
#endif
#ifndef UWB_MULTICAST_IP
#define UWB_MULTICAST_IP 239,0,0,23
#endif
#ifndef UWB_PORT
#define UWB_PORT 50000
#endif

const IPAddress SERVER_ADDR(UWB_SERVER_IP);
const uint16_t  SERVER_PORT = UWB_PORT;
const IPAddress MULTICAST_ADDR(UWB_MULTICAST_IP);
const uint16_t  MULTICAST_PORT = UWB_PORT;

// --- protobuf wire types ----------------------------------------------------
const uint32_t WIRE_VARINT  = 0;
const uint32_t WIRE_FIXED64 = 1;
const uint32_t WIRE_LEN     = 2;
const uint32_t WIRE_FIXED32 = 5;

// --- FNV-1a 32-bit hash of the device name (the Envelope.src) ---------------
inline uint32_t fnv1a32(const char* s) {
  uint32_t h = 0x811c9dc5UL;
  while (*s) {
    h ^= (uint8_t)*s++;
    h *= 0x01000193UL;
  }
  return h;
}

// --- hex helpers ------------------------------------------------------------
inline uint8_t hexDigit(char c) {
  if (c >= '0' && c <= '9') return (uint8_t)(c - '0');
  if (c >= 'a' && c <= 'f') return (uint8_t)(c - 'a' + 10);
  if (c >= 'A' && c <= 'F') return (uint8_t)(c - 'A' + 10);
  return 0xff;
}

// Parse an 8-byte EUI-64 like "7D:00:22:EA:82:60:3B:9C" into out[8].
inline bool parseMacHex(const char* s, uint8_t out[8]) {
  size_t n = 0;
  while (*s && n < 8) {
    if (*s == ':' || *s == '-' || *s == ' ' || *s == '\t') { s++; continue; }
    uint8_t hi = hexDigit(*s++);
    if (!*s) return false;
    uint8_t lo = hexDigit(*s++);
    if (hi == 0xff || lo == 0xff) return false;
    out[n++] = (uint8_t)((hi << 4) | lo);
  }
  return n == 8;
}

// --- protobuf writer --------------------------------------------------------
struct ByteWriter {
  uint8_t* buf;
  size_t   cap;
  size_t   len;

  ByteWriter(uint8_t* b, size_t c) : buf(b), cap(c), len(0) {}

  void varint(uint64_t v) {
    while (v >= 0x80) {
      if (len < cap) buf[len] = (uint8_t)(v | 0x80);
      len++;
      v >>= 7;
    }
    if (len < cap) buf[len] = (uint8_t)v;
    len++;
  }
  void tag(uint32_t field, uint32_t wire) { varint((uint64_t)((field << 3) | wire)); }
  void raw(const void* data, size_t n) {
    if (len + n > cap) n = (cap > len) ? cap - len : 0;
    memcpy(buf + len, data, n);
    len += n;
  }
  void bytes(uint32_t field, const void* data, size_t n) {
    tag(field, WIRE_LEN);
    varint(n);
    raw(data, n);
  }
  void str(uint32_t field, const char* s) { bytes(field, s, strlen(s)); }
  void u32(uint32_t field, uint32_t v) { tag(field, WIRE_VARINT); varint(v); }
  void u64(uint32_t field, uint64_t v) { tag(field, WIRE_VARINT); varint(v); }
  void i32(uint32_t field, int32_t v) { tag(field, WIRE_VARINT); varint((uint64_t)(int64_t)v); }
  void f32(uint32_t field, float f) {
    uint32_t bits;
    memcpy(&bits, &f, 4);
    tag(field, WIRE_FIXED32);
    uint8_t b[4] = {
      (uint8_t)(bits & 0xff),
      (uint8_t)((bits >> 8) & 0xff),
      (uint8_t)((bits >> 16) & 0xff),
      (uint8_t)((bits >> 24) & 0xff),
    };
    raw(b, 4);
  }
  void packedU32(uint32_t field, const uint32_t* vals, size_t n) {
    uint8_t tmp[64];
    ByteWriter w(tmp, sizeof(tmp));
    for (size_t i = 0; i < n; i++) w.varint(vals[i]);
    bytes(field, tmp, w.len);
  }
};

// --- protobuf reader (for the SetBroker receive path) -----------------------
inline bool readVarint(const uint8_t* buf, size_t len, size_t& i, uint64_t& out) {
  out = 0;
  int shift = 0;
  while (i < len && shift < 64) {
    uint8_t b = buf[i++];
    out |= (uint64_t)(b & 0x7f) << shift;
    if (!(b & 0x80)) return true;
    shift += 7;
  }
  return false;
}

inline bool decodeEnvelope(const uint8_t* buf, size_t len,
                           uint32_t& src, uint32_t& dst, uint32_t& msgType,
                           const uint8_t*& payload, size_t& payloadLen) {
  src = 0; dst = 0; msgType = 0; payload = nullptr; payloadLen = 0;
  size_t i = 0;
  while (i < len) {
    uint64_t tag;
    if (!readVarint(buf, len, i, tag)) return false;
    uint32_t field = (uint32_t)(tag >> 3);
    uint32_t wire  = (uint32_t)(tag & 7);
    uint64_t v;
    if (field == 1 && wire == WIRE_VARINT) {
      if (!readVarint(buf, len, i, v)) return false; src = (uint32_t)v;
    } else if (field == 2 && wire == WIRE_VARINT) {
      if (!readVarint(buf, len, i, v)) return false; dst = (uint32_t)v;
    } else if (field == 3 && wire == WIRE_VARINT) {
      if (!readVarint(buf, len, i, v)) return false; msgType = (uint32_t)v;
    } else if (field == 4 && wire == WIRE_LEN) {
      if (!readVarint(buf, len, i, v)) return false;
      size_t n = (size_t)v;
      if (i + n > len) return false;
      payload = buf + i; payloadLen = n; i += n;
    } else {
      if (wire == WIRE_VARINT) {
        if (!readVarint(buf, len, i, v)) return false;
      } else if (wire == WIRE_LEN) {
        if (!readVarint(buf, len, i, v)) return false;
        i += (size_t)v; if (i > len) return false;
      } else if (wire == WIRE_FIXED64) {
        i += 8;
      } else if (wire == WIRE_FIXED32) {
        i += 4;
      } else {
        return false;
      }
      if (i > len) return false;
    }
  }
  return true;
}

inline bool decodeSetBroker(const uint8_t* buf, size_t len, IPAddress& ip, uint16_t& port) {
  size_t i = 0;
  bool haveIp = false, havePort = false;
  while (i < len) {
    uint64_t tag;
    if (!readVarint(buf, len, i, tag)) return false;
    uint32_t field = (uint32_t)(tag >> 3);
    uint32_t wire  = (uint32_t)(tag & 7);
    uint64_t v;
    if (field == 1 && wire == WIRE_LEN) {
      if (!readVarint(buf, len, i, v)) return false;
      size_t n = (size_t)v;
      if (n != 4 || i + n > len) return false;
      ip = IPAddress(buf[i], buf[i + 1], buf[i + 2], buf[i + 3]);
      i += n;
      haveIp = true;
    } else if (field == 2 && wire == WIRE_VARINT) {
      if (!readVarint(buf, len, i, v)) return false;
      port = (uint16_t)v;
      havePort = true;
    } else {
      if (wire == WIRE_VARINT) {
        if (!readVarint(buf, len, i, v)) return false;
      } else if (wire == WIRE_LEN) {
        if (!readVarint(buf, len, i, v)) return false;
        i += (size_t)v; if (i > len) return false;
      } else if (wire == WIRE_FIXED64) {
        i += 8;
      } else if (wire == WIRE_FIXED32) {
        i += 4;
      } else {
        return false;
      }
    }
  }
  return haveIp && havePort;
}

// --- transport --------------------------------------------------------------
class UwbProto {
public:
  void begin(const char* deviceName, const uint32_t* msgTypes, size_t msgTypeCount) {
    _deviceName = deviceName;
    _src = fnv1a32(deviceName);
    _server = SERVER_ADDR;
    _serverPort = SERVER_PORT;

    size_t n = (msgTypeCount > MAX_MSG_TYPES) ? MAX_MSG_TYPES : msgTypeCount;
    for (size_t i = 0; i < n; i++) _msgTypes[i] = msgTypes[i];
    _msgTypeCount = n;

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

  void setUwbMac(const uint8_t* mac8, size_t len) {
    _uwbMacLen = (len > 8) ? 8 : len;
    memcpy(_uwbMac, mac8, _uwbMacLen);
  }

  void setServer(const IPAddress& ip, uint16_t port) { _server = ip; _serverPort = port; }

  uint16_t localPort() const { return _localPort; }
  uint32_t src() const { return _src; }
  const IPAddress& server() const { return _server; }
  uint16_t serverPort() const { return _serverPort; }

  // Process one incoming packet (currently only SetBroker is handled).
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
          Serial.printf("UDP: broker switched to %s:%u\n", _server.toString().c_str(), (unsigned)_serverPort);
        }
      }
    }
  }

  void sendAnnounce() {
    Envelope envelope;
    DeviceEvent device_event;
    Buffer envelope_buffer(100);
    Buffer payload_buffer(80);

    envelope.src = FNV(_deviceName);
    envelope.msg_type = DeviceEvent::MSG_ID;

    device_event.device_name = _deviceName;
    std::vector<uint32_t> caps(_msgTypes, _msgTypes + _msgTypeCount);
    device_event.request_types = caps;
    device_event.event_types = caps;

    device_event.encode(payload_buffer);
    envelope.payload = payload_buffer.to_vector();
    envelope.encode(envelope_buffer);

    // Multicast per protocol.md, plus a unicast copy so the central server
    // sees it even where multicast is not routed.
    sendTo(MULTICAST_ADDR, MULTICAST_PORT, envelope_buffer.data(),envelope_buffer.size(), true);
    sendTo(_server, _serverPort, envelope_buffer.data(),envelope_buffer.size(), false);
  }

  void sendSysEvent() {
    Envelope envelope;
    SysEvent sys_event;
    Buffer envelope_buffer(100);
    Buffer payload_buffer(80);

    envelope.src = FNV(_deviceName);
    envelope.msg_type = SysEvent::MSG_ID;

    sys_event.uptime = (uint64_t)millis();

    sys_event.encode(payload_buffer);
    envelope.payload = payload_buffer.to_vector();
    envelope.encode(envelope_buffer);


    sendTo(_server, _serverPort, envelope_buffer.data(),envelope_buffer.size(), false);
  }

  void sendUwbAnchorEvent() {
    Envelope envelope;
    DW1000AnchorEvent event;
    Buffer envelope_buffer(100);
    Buffer payload_buffer(80);

    envelope.src = FNV(_deviceName);
    envelope.msg_type = DW1000AnchorEvent::MSG_ID;

    event.uwb_mac_address = std::vector<uint8_t>(_uwbMac, _uwbMac + _uwbMacLen);

    event.encode(payload_buffer);
    envelope.payload = payload_buffer.to_vector();
    envelope.encode(envelope_buffer);
    sendTo(_server, _serverPort, envelope_buffer.data(),envelope_buffer.size(), false);
  }

  void sendUwbTagEvent(const uint8_t anchorShort[2], float distanceM, float rxPowerDb) {
    Envelope envelope;
    DW1000TagEvent event;
    Buffer envelope_buffer(100);
    Buffer payload_buffer(80);

    envelope.src = FNV(_deviceName);
    envelope.msg_type = DW1000TagEvent::MSG_ID;

    event.uwb_mac_address = std::vector<uint8_t>(_uwbMac, _uwbMac + _uwbMacLen);
    event.anchor = (uint32_t)((anchorShort[0] << 8) | anchorShort[1]);
    event.distance_m = distanceM;
    event.rx_power_db = rxPowerDb;

    event.encode(payload_buffer);
    envelope.payload = payload_buffer.to_vector();
    envelope.encode(envelope_buffer);
    sendTo(_server, _serverPort, envelope_buffer.data(),envelope_buffer.size(), false);

  }

private:
  void sendTo(const IPAddress& ip, uint16_t port, const uint8_t* data, size_t len, bool multicast) {
#ifdef ESP32
    if (multicast) return;  // skip multicast on ESP32; unicast to server is the reliable path
    _udp.beginPacket(ip, port);
#else
    if (multicast) {
      _udp.beginPacketMulticast(ip, port, WiFi.localIP());
    } else {
      _udp.beginPacket(ip, port);
    }
#endif
    _udp.write(data, len);
    _udp.endPacket();
  }

  enum { MAX_MSG_TYPES = 4 };

  const char* _deviceName = "";
  uint32_t    _src = 0;
  IPAddress   _server;
  uint16_t    _serverPort = SERVER_PORT;
  uint16_t    _localPort = 0;
  WiFiUDP     _udp;
  uint8_t     _uwbMac[8];
  size_t      _uwbMacLen = 0;
  uint32_t    _msgTypes[MAX_MSG_TYPES];
  size_t      _msgTypeCount = 0;
};

} // namespace uwbproto