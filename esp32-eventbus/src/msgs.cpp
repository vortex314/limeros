#include <string>
#include <vector>
#include <cstdint>
#include <optional>
#include <cbor.h>
#include <functional>
#include <sstream>
#include <msg.h>

#include "msgs.h"








Bytes UdpMessage::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (dst) {
            cbor_encode_int(&mapEncoder, Field::DST_INDEX);
            cbor_encode_int(&mapEncoder, dst.value());
            }
    if (src) {
            cbor_encode_int(&mapEncoder, Field::SRC_INDEX);
            cbor_encode_int(&mapEncoder, src.value());
            }
    if (msg_typ) {
            cbor_encode_int(&mapEncoder, Field::MSG_TYP_INDEX);
            cbor_encode_int(&mapEncoder, msg_typ.value());
            }
    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (payload) {
            cbor_encode_int(&mapEncoder, Field::PAYLOAD_INDEX);
            cbor_encode_byte_string(&mapEncoder, payload.value().data(), payload.value().size());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

UdpMessage* UdpMessage::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    UdpMessage* msg = new UdpMessage();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::DST_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *dst = v;
}
                break;
            }
            
            case Field::SRC_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *src = v;
}
                break;
            }
            
            case Field::MSG_TYP_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *msg_typ = v;
}
                break;
            }
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::PAYLOAD_INDEX:{{
    uint8_t tmpbuf[512];
    size_t tmplen = sizeof(tmpbuf);
    if (cbor_value_is_byte_string(&mapIt)) {
        cbor_value_copy_byte_string(&mapIt, tmpbuf, &tmplen, &mapIt);
        *payload = Bytes(tmpbuf, tmpbuf + tmplen);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string UdpMessage::to_string() const {
    std::ostringstream oss;
    oss << "UdpMessage {\n";
    if (dst) {
        oss << "  dst: ";
        oss << dst.value();
        oss << "\n";
    } else {
        oss << "  dst: None\n";
    }
    if (src) {
        oss << "  src: ";
        oss << src.value();
        oss << "\n";
    } else {
        oss << "  src: None\n";
    }
    if (msg_typ) {
        oss << "  msg_typ: ";
        oss << msg_typ.value();
        oss << "\n";
    } else {
        oss << "  msg_typ: None\n";
    }
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (payload) {
        oss << "  payload: ";
        oss << "Bytes[" << payload.value().size() << "]";
        oss << "\n";
    } else {
        oss << "  payload: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes DeviceAliveEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (device_id) {
            cbor_encode_int(&mapEncoder, Field::DEVICE_ID_INDEX);
            cbor_encode_text_stringz(&mapEncoder, device_id.value().c_str());
            }
    {
            CborEncoder arrayEncoder;
            cbor_encode_int(&mapEncoder, Field::OBJECTS_INDEX );
            cbor_encoder_create_array(&mapEncoder, &arrayEncoder, objects.size());
            for (const auto & item : objects) {
                cbor_encode_text_stringz(&arrayEncoder, item.c_str());
            }
            cbor_encoder_close_container(&mapEncoder, &arrayEncoder);
            }
    {
            CborEncoder arrayEncoder;
            cbor_encode_int(&mapEncoder, Field::SUBSCRIBES_INDEX );
            cbor_encoder_create_array(&mapEncoder, &arrayEncoder, subscribes.size());
            for (const auto & item : subscribes) {
                cbor_encode_text_stringz(&arrayEncoder, item.c_str());
            }
            cbor_encoder_close_container(&mapEncoder, &arrayEncoder);
            }
    {
            CborEncoder arrayEncoder;
            cbor_encode_int(&mapEncoder, Field::PUBLISHES_INDEX );
            cbor_encoder_create_array(&mapEncoder, &arrayEncoder, publishes.size());
            for (const auto & item : publishes) {
                cbor_encode_text_stringz(&arrayEncoder, item.c_str());
            }
            cbor_encoder_close_container(&mapEncoder, &arrayEncoder);
            }
    {
            CborEncoder arrayEncoder;
            cbor_encode_int(&mapEncoder, Field::REQUESTS_INDEX );
            cbor_encoder_create_array(&mapEncoder, &arrayEncoder, requests.size());
            for (const auto & item : requests) {
                cbor_encode_text_stringz(&arrayEncoder, item.c_str());
            }
            cbor_encoder_close_container(&mapEncoder, &arrayEncoder);
            }
    {
            CborEncoder arrayEncoder;
            cbor_encode_int(&mapEncoder, Field::REPLIES_INDEX );
            cbor_encoder_create_array(&mapEncoder, &arrayEncoder, replies.size());
            for (const auto & item : replies) {
                cbor_encode_text_stringz(&arrayEncoder, item.c_str());
            }
            cbor_encoder_close_container(&mapEncoder, &arrayEncoder);
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

DeviceAliveEvent* DeviceAliveEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    DeviceAliveEvent* msg = new DeviceAliveEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::DEVICE_ID_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *device_id = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::OBJECTS_INDEX:{CborValue tmp;
                cbor_value_enter_container(&mapIt,&tmp);
                while (!cbor_value_at_end(&tmp)) {
                    std::string v;
                    {
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&tmp)) {
        cbor_value_copy_text_string(&tmp, valbuf, &vallen, &tmp);
        v = std::string(valbuf, vallen - 1);
    }
}
                    objects.push_back(v);
                }
                cbor_value_leave_container(&mapIt,&tmp);
                break;
            }
            
            case Field::SUBSCRIBES_INDEX:{CborValue tmp;
                cbor_value_enter_container(&mapIt,&tmp);
                while (!cbor_value_at_end(&tmp)) {
                    std::string v;
                    {
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&tmp)) {
        cbor_value_copy_text_string(&tmp, valbuf, &vallen, &tmp);
        v = std::string(valbuf, vallen - 1);
    }
}
                    subscribes.push_back(v);
                }
                cbor_value_leave_container(&mapIt,&tmp);
                break;
            }
            
            case Field::PUBLISHES_INDEX:{CborValue tmp;
                cbor_value_enter_container(&mapIt,&tmp);
                while (!cbor_value_at_end(&tmp)) {
                    std::string v;
                    {
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&tmp)) {
        cbor_value_copy_text_string(&tmp, valbuf, &vallen, &tmp);
        v = std::string(valbuf, vallen - 1);
    }
}
                    publishes.push_back(v);
                }
                cbor_value_leave_container(&mapIt,&tmp);
                break;
            }
            
            case Field::REQUESTS_INDEX:{CborValue tmp;
                cbor_value_enter_container(&mapIt,&tmp);
                while (!cbor_value_at_end(&tmp)) {
                    std::string v;
                    {
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&tmp)) {
        cbor_value_copy_text_string(&tmp, valbuf, &vallen, &tmp);
        v = std::string(valbuf, vallen - 1);
    }
}
                    requests.push_back(v);
                }
                cbor_value_leave_container(&mapIt,&tmp);
                break;
            }
            
            case Field::REPLIES_INDEX:{CborValue tmp;
                cbor_value_enter_container(&mapIt,&tmp);
                while (!cbor_value_at_end(&tmp)) {
                    std::string v;
                    {
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&tmp)) {
        cbor_value_copy_text_string(&tmp, valbuf, &vallen, &tmp);
        v = std::string(valbuf, vallen - 1);
    }
}
                    replies.push_back(v);
                }
                cbor_value_leave_container(&mapIt,&tmp);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string DeviceAliveEvent::to_string() const {
    std::ostringstream oss;
    oss << "DeviceAliveEvent {\n";
    if (device_id) {
        oss << "  device_id: ";
        oss << "\"" << device_id.value() << "\"";
        oss << "\n";
    } else {
        oss << "  device_id: None\n";
    }
    oss << "  objects: [";
    for (size_t i = 0; i < objects.size(); i++) {
        if (i > 0) oss << ", ";
        oss << "\"" << objects[i] << "\"";
    }
    oss << "]\n";
    oss << "  subscribes: [";
    for (size_t i = 0; i < subscribes.size(); i++) {
        if (i > 0) oss << ", ";
        oss << "\"" << subscribes[i] << "\"";
    }
    oss << "]\n";
    oss << "  publishes: [";
    for (size_t i = 0; i < publishes.size(); i++) {
        if (i > 0) oss << ", ";
        oss << "\"" << publishes[i] << "\"";
    }
    oss << "]\n";
    oss << "  requests: [";
    for (size_t i = 0; i < requests.size(); i++) {
        if (i > 0) oss << ", ";
        oss << "\"" << requests[i] << "\"";
    }
    oss << "]\n";
    oss << "  replies: [";
    for (size_t i = 0; i < replies.size(); i++) {
        if (i > 0) oss << ", ";
        oss << "\"" << replies[i] << "\"";
    }
    oss << "]\n";
    oss << "}";
    return oss.str();
}


Bytes LogEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (level) {
            cbor_encode_int(&mapEncoder, Field::LEVEL_INDEX);
            cbor_encode_int(&mapEncoder, level.value());
            }
    if (message) {
            cbor_encode_int(&mapEncoder, Field::MESSAGE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, message.value().c_str());
            }
    if (error_code) {
            cbor_encode_int(&mapEncoder, Field::ERROR_CODE_INDEX);
            cbor_encode_int(&mapEncoder, error_code.value());
            }
    if (file) {
            cbor_encode_int(&mapEncoder, Field::FILE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, file.value().c_str());
            }
    if (line) {
            cbor_encode_int(&mapEncoder, Field::LINE_INDEX);
            cbor_encode_int(&mapEncoder, line.value());
            }
    if (timestamp) {
            cbor_encode_int(&mapEncoder, Field::TIMESTAMP_INDEX);
            cbor_encode_int(&mapEncoder, timestamp.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

LogEvent* LogEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    LogEvent* msg = new LogEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::LEVEL_INDEX:{{
    long long v;
    cbor_value_get_int64(&mapIt, &v);
    *level = static_cast<LogLevel>(v);
}
                break;
            }
            
            case Field::MESSAGE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *message = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::ERROR_CODE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *error_code = v;
}
                break;
            }
            
            case Field::FILE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *file = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::LINE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *line = v;
}
                break;
            }
            
            case Field::TIMESTAMP_INDEX:{cbor_value_get_uint64(&mapIt, &*timestamp);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string LogEvent::to_string() const {
    std::ostringstream oss;
    oss << "LogEvent {\n";
    if (level) {
        oss << "  level: ";
        oss << level.value();
        oss << "\n";
    } else {
        oss << "  level: None\n";
    }
    if (message) {
        oss << "  message: ";
        oss << "\"" << message.value() << "\"";
        oss << "\n";
    } else {
        oss << "  message: None\n";
    }
    if (error_code) {
        oss << "  error_code: ";
        oss << error_code.value();
        oss << "\n";
    } else {
        oss << "  error_code: None\n";
    }
    if (file) {
        oss << "  file: ";
        oss << "\"" << file.value() << "\"";
        oss << "\n";
    } else {
        oss << "  file: None\n";
    }
    if (line) {
        oss << "  line: ";
        oss << line.value();
        oss << "\n";
    } else {
        oss << "  line: None\n";
    }
    if (timestamp) {
        oss << "  timestamp: ";
        oss << timestamp.value();
        oss << "\n";
    } else {
        oss << "  timestamp: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes SysRequest::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (set_time) {
            cbor_encode_int(&mapEncoder, Field::SET_TIME_INDEX);
            cbor_encode_int(&mapEncoder, set_time.value());
            }
    if (reboot) {
            cbor_encode_int(&mapEncoder, Field::REBOOT_INDEX);
            cbor_encode_boolean(&mapEncoder, reboot.value());
            }
    if (console) {
            cbor_encode_int(&mapEncoder, Field::CONSOLE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, console.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

SysRequest* SysRequest::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    SysRequest* msg = new SysRequest();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::SET_TIME_INDEX:{cbor_value_get_uint64(&mapIt, &*set_time);
                break;
            }
            
            case Field::REBOOT_INDEX:{cbor_value_get_boolean(&mapIt, &*reboot);
                break;
            }
            
            case Field::CONSOLE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *console = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string SysRequest::to_string() const {
    std::ostringstream oss;
    oss << "SysRequest {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (set_time) {
        oss << "  set_time: ";
        oss << set_time.value();
        oss << "\n";
    } else {
        oss << "  set_time: None\n";
    }
    if (reboot) {
        oss << "  reboot: ";
        oss << (reboot.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  reboot: None\n";
    }
    if (console) {
        oss << "  console: ";
        oss << "\"" << console.value() << "\"";
        oss << "\n";
    } else {
        oss << "  console: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes SysReply::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (rc) {
            cbor_encode_int(&mapEncoder, Field::RC_INDEX);
            cbor_encode_int(&mapEncoder, rc.value());
            }
    if (message) {
            cbor_encode_int(&mapEncoder, Field::MESSAGE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, message.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

SysReply* SysReply::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    SysReply* msg = new SysReply();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::RC_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *rc = v;
}
                break;
            }
            
            case Field::MESSAGE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *message = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string SysReply::to_string() const {
    std::ostringstream oss;
    oss << "SysReply {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (rc) {
        oss << "  rc: ";
        oss << rc.value();
        oss << "\n";
    } else {
        oss << "  rc: None\n";
    }
    if (message) {
        oss << "  message: ";
        oss << "\"" << message.value() << "\"";
        oss << "\n";
    } else {
        oss << "  message: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes SysEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (utc) {
            cbor_encode_int(&mapEncoder, Field::UTC_INDEX);
            cbor_encode_int(&mapEncoder, utc.value());
            }
    if (uptime) {
            cbor_encode_int(&mapEncoder, Field::UPTIME_INDEX);
            cbor_encode_int(&mapEncoder, uptime.value());
            }
    if (free_heap) {
            cbor_encode_int(&mapEncoder, Field::FREE_HEAP_INDEX);
            cbor_encode_int(&mapEncoder, free_heap.value());
            }
    if (flash) {
            cbor_encode_int(&mapEncoder, Field::FLASH_INDEX);
            cbor_encode_int(&mapEncoder, flash.value());
            }
    if (cpu_board) {
            cbor_encode_int(&mapEncoder, Field::CPU_BOARD_INDEX);
            cbor_encode_text_stringz(&mapEncoder, cpu_board.value().c_str());
            }
    if (build_date) {
            cbor_encode_int(&mapEncoder, Field::BUILD_DATE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, build_date.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

SysEvent* SysEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    SysEvent* msg = new SysEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::UTC_INDEX:{cbor_value_get_uint64(&mapIt, &*utc);
                break;
            }
            
            case Field::UPTIME_INDEX:{cbor_value_get_uint64(&mapIt, &*uptime);
                break;
            }
            
            case Field::FREE_HEAP_INDEX:{cbor_value_get_uint64(&mapIt, &*free_heap);
                break;
            }
            
            case Field::FLASH_INDEX:{cbor_value_get_uint64(&mapIt, &*flash);
                break;
            }
            
            case Field::CPU_BOARD_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *cpu_board = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::BUILD_DATE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *build_date = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string SysEvent::to_string() const {
    std::ostringstream oss;
    oss << "SysEvent {\n";
    if (utc) {
        oss << "  utc: ";
        oss << utc.value();
        oss << "\n";
    } else {
        oss << "  utc: None\n";
    }
    if (uptime) {
        oss << "  uptime: ";
        oss << uptime.value();
        oss << "\n";
    } else {
        oss << "  uptime: None\n";
    }
    if (free_heap) {
        oss << "  free_heap: ";
        oss << free_heap.value();
        oss << "\n";
    } else {
        oss << "  free_heap: None\n";
    }
    if (flash) {
        oss << "  flash: ";
        oss << flash.value();
        oss << "\n";
    } else {
        oss << "  flash: None\n";
    }
    if (cpu_board) {
        oss << "  cpu_board: ";
        oss << "\"" << cpu_board.value() << "\"";
        oss << "\n";
    } else {
        oss << "  cpu_board: None\n";
    }
    if (build_date) {
        oss << "  build_date: ";
        oss << "\"" << build_date.value() << "\"";
        oss << "\n";
    } else {
        oss << "  build_date: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes WifiEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (ssid) {
            cbor_encode_int(&mapEncoder, Field::SSID_INDEX);
            cbor_encode_text_stringz(&mapEncoder, ssid.value().c_str());
            }
    if (bssid) {
            cbor_encode_int(&mapEncoder, Field::BSSID_INDEX);
            cbor_encode_text_stringz(&mapEncoder, bssid.value().c_str());
            }
    if (rssi) {
            cbor_encode_int(&mapEncoder, Field::RSSI_INDEX);
            cbor_encode_int(&mapEncoder, rssi.value());
            }
    if (ip) {
            cbor_encode_int(&mapEncoder, Field::IP_INDEX);
            cbor_encode_text_stringz(&mapEncoder, ip.value().c_str());
            }
    if (mac) {
            cbor_encode_int(&mapEncoder, Field::MAC_INDEX);
            cbor_encode_text_stringz(&mapEncoder, mac.value().c_str());
            }
    if (channel) {
            cbor_encode_int(&mapEncoder, Field::CHANNEL_INDEX);
            cbor_encode_int(&mapEncoder, channel.value());
            }
    if (gateway) {
            cbor_encode_int(&mapEncoder, Field::GATEWAY_INDEX);
            cbor_encode_text_stringz(&mapEncoder, gateway.value().c_str());
            }
    if (netmask) {
            cbor_encode_int(&mapEncoder, Field::NETMASK_INDEX);
            cbor_encode_text_stringz(&mapEncoder, netmask.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

WifiEvent* WifiEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    WifiEvent* msg = new WifiEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::SSID_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *ssid = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::BSSID_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *bssid = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::RSSI_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *rssi = v;
}
                break;
            }
            
            case Field::IP_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *ip = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::MAC_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *mac = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::CHANNEL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *channel = v;
}
                break;
            }
            
            case Field::GATEWAY_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *gateway = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::NETMASK_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *netmask = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string WifiEvent::to_string() const {
    std::ostringstream oss;
    oss << "WifiEvent {\n";
    if (ssid) {
        oss << "  ssid: ";
        oss << "\"" << ssid.value() << "\"";
        oss << "\n";
    } else {
        oss << "  ssid: None\n";
    }
    if (bssid) {
        oss << "  bssid: ";
        oss << "\"" << bssid.value() << "\"";
        oss << "\n";
    } else {
        oss << "  bssid: None\n";
    }
    if (rssi) {
        oss << "  rssi: ";
        oss << rssi.value();
        oss << "\n";
    } else {
        oss << "  rssi: None\n";
    }
    if (ip) {
        oss << "  ip: ";
        oss << "\"" << ip.value() << "\"";
        oss << "\n";
    } else {
        oss << "  ip: None\n";
    }
    if (mac) {
        oss << "  mac: ";
        oss << "\"" << mac.value() << "\"";
        oss << "\n";
    } else {
        oss << "  mac: None\n";
    }
    if (channel) {
        oss << "  channel: ";
        oss << channel.value();
        oss << "\n";
    } else {
        oss << "  channel: None\n";
    }
    if (gateway) {
        oss << "  gateway: ";
        oss << "\"" << gateway.value() << "\"";
        oss << "\n";
    } else {
        oss << "  gateway: None\n";
    }
    if (netmask) {
        oss << "  netmask: ";
        oss << "\"" << netmask.value() << "\"";
        oss << "\n";
    } else {
        oss << "  netmask: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes MulticastEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (group) {
            cbor_encode_int(&mapEncoder, Field::GROUP_INDEX);
            cbor_encode_text_stringz(&mapEncoder, group.value().c_str());
            }
    if (port) {
            cbor_encode_int(&mapEncoder, Field::PORT_INDEX);
            cbor_encode_int(&mapEncoder, port.value());
            }
    if (mtu) {
            cbor_encode_int(&mapEncoder, Field::MTU_INDEX);
            cbor_encode_int(&mapEncoder, mtu.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

MulticastEvent* MulticastEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    MulticastEvent* msg = new MulticastEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::GROUP_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *group = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::PORT_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *port = v;
}
                break;
            }
            
            case Field::MTU_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *mtu = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string MulticastEvent::to_string() const {
    std::ostringstream oss;
    oss << "MulticastEvent {\n";
    if (group) {
        oss << "  group: ";
        oss << "\"" << group.value() << "\"";
        oss << "\n";
    } else {
        oss << "  group: None\n";
    }
    if (port) {
        oss << "  port: ";
        oss << port.value();
        oss << "\n";
    } else {
        oss << "  port: None\n";
    }
    if (mtu) {
        oss << "  mtu: ";
        oss << mtu.value();
        oss << "\n";
    } else {
        oss << "  mtu: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes PingRequest::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (number) {
            cbor_encode_int(&mapEncoder, Field::NUMBER_INDEX);
            cbor_encode_int(&mapEncoder, number.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

PingRequest* PingRequest::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    PingRequest* msg = new PingRequest();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::NUMBER_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *number = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string PingRequest::to_string() const {
    std::ostringstream oss;
    oss << "PingRequest {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (number) {
        oss << "  number: ";
        oss << number.value();
        oss << "\n";
    } else {
        oss << "  number: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes PingReply::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (number) {
            cbor_encode_int(&mapEncoder, Field::NUMBER_INDEX);
            cbor_encode_int(&mapEncoder, number.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

PingReply* PingReply::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    PingReply* msg = new PingReply();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::NUMBER_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *number = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string PingReply::to_string() const {
    std::ostringstream oss;
    oss << "PingReply {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (number) {
        oss << "  number: ";
        oss << number.value();
        oss << "\n";
    } else {
        oss << "  number: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes HoverboardEventRaw::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (ctrl_mod) {
            cbor_encode_int(&mapEncoder, Field::CTRL_MOD_INDEX);
            cbor_encode_int(&mapEncoder, ctrl_mod.value());
            }
    if (ctrl_typ) {
            cbor_encode_int(&mapEncoder, Field::CTRL_TYP_INDEX);
            cbor_encode_int(&mapEncoder, ctrl_typ.value());
            }
    if (cur_mot_max) {
            cbor_encode_int(&mapEncoder, Field::CUR_MOT_MAX_INDEX);
            cbor_encode_int(&mapEncoder, cur_mot_max.value());
            }
    if (rpm_mot_max) {
            cbor_encode_int(&mapEncoder, Field::RPM_MOT_MAX_INDEX);
            cbor_encode_int(&mapEncoder, rpm_mot_max.value());
            }
    if (fi_weak_ena) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_ENA_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_ena.value());
            }
    if (fi_weak_hi) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_HI_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_hi.value());
            }
    if (fi_weak_lo) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_LO_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_lo.value());
            }
    if (fi_weak_max) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_MAX_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_max.value());
            }
    if (phase_adv_max_deg) {
            cbor_encode_int(&mapEncoder, Field::PHASE_ADV_MAX_DEG_INDEX);
            cbor_encode_int(&mapEncoder, phase_adv_max_deg.value());
            }
    if (input1_raw) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_RAW_INDEX);
            cbor_encode_int(&mapEncoder, input1_raw.value());
            }
    if (input1_typ) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_TYP_INDEX);
            cbor_encode_int(&mapEncoder, input1_typ.value());
            }
    if (input1_min) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_MIN_INDEX);
            cbor_encode_int(&mapEncoder, input1_min.value());
            }
    if (input1_mid) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_MID_INDEX);
            cbor_encode_int(&mapEncoder, input1_mid.value());
            }
    if (input1_max) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_MAX_INDEX);
            cbor_encode_int(&mapEncoder, input1_max.value());
            }
    if (input1_cmd) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_CMD_INDEX);
            cbor_encode_int(&mapEncoder, input1_cmd.value());
            }
    if (input2_raw) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_RAW_INDEX);
            cbor_encode_int(&mapEncoder, input2_raw.value());
            }
    if (input2_typ) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_TYP_INDEX);
            cbor_encode_int(&mapEncoder, input2_typ.value());
            }
    if (input2_min) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_MIN_INDEX);
            cbor_encode_int(&mapEncoder, input2_min.value());
            }
    if (input2_mid) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_MID_INDEX);
            cbor_encode_int(&mapEncoder, input2_mid.value());
            }
    if (input2_max) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_MAX_INDEX);
            cbor_encode_int(&mapEncoder, input2_max.value());
            }
    if (input2_cmd) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_CMD_INDEX);
            cbor_encode_int(&mapEncoder, input2_cmd.value());
            }
    if (aux_input1_raw) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_RAW_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_raw.value());
            }
    if (aux_input1_typ) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_TYP_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_typ.value());
            }
    if (aux_input1_min) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_MIN_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_min.value());
            }
    if (aux_input1_mid) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_MID_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_mid.value());
            }
    if (aux_input1_max) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_MAX_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_max.value());
            }
    if (aux_input1_cmd) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_CMD_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_cmd.value());
            }
    if (aux_input2_raw) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_RAW_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_raw.value());
            }
    if (aux_input2_typ) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_TYP_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_typ.value());
            }
    if (aux_input2_min) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_MIN_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_min.value());
            }
    if (aux_input2_mid) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_MID_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_mid.value());
            }
    if (aux_input2_max) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_MAX_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_max.value());
            }
    if (aux_input2_cmd) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_CMD_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_cmd.value());
            }
    if (dc_curr) {
            cbor_encode_int(&mapEncoder, Field::DC_CURR_INDEX);
            cbor_encode_int(&mapEncoder, dc_curr.value());
            }
    if (rdc_curr) {
            cbor_encode_int(&mapEncoder, Field::RDC_CURR_INDEX);
            cbor_encode_int(&mapEncoder, rdc_curr.value());
            }
    if (ldc_curr) {
            cbor_encode_int(&mapEncoder, Field::LDC_CURR_INDEX);
            cbor_encode_int(&mapEncoder, ldc_curr.value());
            }
    if (cmdl) {
            cbor_encode_int(&mapEncoder, Field::CMDL_INDEX);
            cbor_encode_int(&mapEncoder, cmdl.value());
            }
    if (cmdr) {
            cbor_encode_int(&mapEncoder, Field::CMDR_INDEX);
            cbor_encode_int(&mapEncoder, cmdr.value());
            }
    if (spd_avg) {
            cbor_encode_int(&mapEncoder, Field::SPD_AVG_INDEX);
            cbor_encode_int(&mapEncoder, spd_avg.value());
            }
    if (spdl) {
            cbor_encode_int(&mapEncoder, Field::SPDL_INDEX);
            cbor_encode_int(&mapEncoder, spdl.value());
            }
    if (spdr) {
            cbor_encode_int(&mapEncoder, Field::SPDR_INDEX);
            cbor_encode_int(&mapEncoder, spdr.value());
            }
    if (filter_rate) {
            cbor_encode_int(&mapEncoder, Field::FILTER_RATE_INDEX);
            cbor_encode_int(&mapEncoder, filter_rate.value());
            }
    if (spd_coef) {
            cbor_encode_int(&mapEncoder, Field::SPD_COEF_INDEX);
            cbor_encode_int(&mapEncoder, spd_coef.value());
            }
    if (str_coef) {
            cbor_encode_int(&mapEncoder, Field::STR_COEF_INDEX);
            cbor_encode_int(&mapEncoder, str_coef.value());
            }
    if (batv) {
            cbor_encode_int(&mapEncoder, Field::BATV_INDEX);
            cbor_encode_int(&mapEncoder, batv.value());
            }
    if (temp) {
            cbor_encode_int(&mapEncoder, Field::TEMP_INDEX);
            cbor_encode_int(&mapEncoder, temp.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

HoverboardEventRaw* HoverboardEventRaw::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    HoverboardEventRaw* msg = new HoverboardEventRaw();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::CTRL_MOD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *ctrl_mod = v;
}
                break;
            }
            
            case Field::CTRL_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *ctrl_typ = v;
}
                break;
            }
            
            case Field::CUR_MOT_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *cur_mot_max = v;
}
                break;
            }
            
            case Field::RPM_MOT_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *rpm_mot_max = v;
}
                break;
            }
            
            case Field::FI_WEAK_ENA_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_ena = v;
}
                break;
            }
            
            case Field::FI_WEAK_HI_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_hi = v;
}
                break;
            }
            
            case Field::FI_WEAK_LO_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_lo = v;
}
                break;
            }
            
            case Field::FI_WEAK_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_max = v;
}
                break;
            }
            
            case Field::PHASE_ADV_MAX_DEG_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *phase_adv_max_deg = v;
}
                break;
            }
            
            case Field::INPUT1_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_raw = v;
}
                break;
            }
            
            case Field::INPUT1_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_typ = v;
}
                break;
            }
            
            case Field::INPUT1_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_min = v;
}
                break;
            }
            
            case Field::INPUT1_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_mid = v;
}
                break;
            }
            
            case Field::INPUT1_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_max = v;
}
                break;
            }
            
            case Field::INPUT1_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_cmd = v;
}
                break;
            }
            
            case Field::INPUT2_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_raw = v;
}
                break;
            }
            
            case Field::INPUT2_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_typ = v;
}
                break;
            }
            
            case Field::INPUT2_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_min = v;
}
                break;
            }
            
            case Field::INPUT2_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_mid = v;
}
                break;
            }
            
            case Field::INPUT2_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_max = v;
}
                break;
            }
            
            case Field::INPUT2_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_cmd = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_raw = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_typ = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_min = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_mid = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_max = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_cmd = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_raw = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_typ = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_min = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_mid = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_max = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_cmd = v;
}
                break;
            }
            
            case Field::DC_CURR_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *dc_curr = v;
}
                break;
            }
            
            case Field::RDC_CURR_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *rdc_curr = v;
}
                break;
            }
            
            case Field::LDC_CURR_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *ldc_curr = v;
}
                break;
            }
            
            case Field::CMDL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *cmdl = v;
}
                break;
            }
            
            case Field::CMDR_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *cmdr = v;
}
                break;
            }
            
            case Field::SPD_AVG_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spd_avg = v;
}
                break;
            }
            
            case Field::SPDL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spdl = v;
}
                break;
            }
            
            case Field::SPDR_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spdr = v;
}
                break;
            }
            
            case Field::FILTER_RATE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *filter_rate = v;
}
                break;
            }
            
            case Field::SPD_COEF_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spd_coef = v;
}
                break;
            }
            
            case Field::STR_COEF_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *str_coef = v;
}
                break;
            }
            
            case Field::BATV_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *batv = v;
}
                break;
            }
            
            case Field::TEMP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *temp = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string HoverboardEventRaw::to_string() const {
    std::ostringstream oss;
    oss << "HoverboardEventRaw {\n";
    if (ctrl_mod) {
        oss << "  ctrl_mod: ";
        oss << ctrl_mod.value();
        oss << "\n";
    } else {
        oss << "  ctrl_mod: None\n";
    }
    if (ctrl_typ) {
        oss << "  ctrl_typ: ";
        oss << ctrl_typ.value();
        oss << "\n";
    } else {
        oss << "  ctrl_typ: None\n";
    }
    if (cur_mot_max) {
        oss << "  cur_mot_max: ";
        oss << cur_mot_max.value();
        oss << "\n";
    } else {
        oss << "  cur_mot_max: None\n";
    }
    if (rpm_mot_max) {
        oss << "  rpm_mot_max: ";
        oss << rpm_mot_max.value();
        oss << "\n";
    } else {
        oss << "  rpm_mot_max: None\n";
    }
    if (fi_weak_ena) {
        oss << "  fi_weak_ena: ";
        oss << fi_weak_ena.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_ena: None\n";
    }
    if (fi_weak_hi) {
        oss << "  fi_weak_hi: ";
        oss << fi_weak_hi.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_hi: None\n";
    }
    if (fi_weak_lo) {
        oss << "  fi_weak_lo: ";
        oss << fi_weak_lo.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_lo: None\n";
    }
    if (fi_weak_max) {
        oss << "  fi_weak_max: ";
        oss << fi_weak_max.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_max: None\n";
    }
    if (phase_adv_max_deg) {
        oss << "  phase_adv_max_deg: ";
        oss << phase_adv_max_deg.value();
        oss << "\n";
    } else {
        oss << "  phase_adv_max_deg: None\n";
    }
    if (input1_raw) {
        oss << "  input1_raw: ";
        oss << input1_raw.value();
        oss << "\n";
    } else {
        oss << "  input1_raw: None\n";
    }
    if (input1_typ) {
        oss << "  input1_typ: ";
        oss << input1_typ.value();
        oss << "\n";
    } else {
        oss << "  input1_typ: None\n";
    }
    if (input1_min) {
        oss << "  input1_min: ";
        oss << input1_min.value();
        oss << "\n";
    } else {
        oss << "  input1_min: None\n";
    }
    if (input1_mid) {
        oss << "  input1_mid: ";
        oss << input1_mid.value();
        oss << "\n";
    } else {
        oss << "  input1_mid: None\n";
    }
    if (input1_max) {
        oss << "  input1_max: ";
        oss << input1_max.value();
        oss << "\n";
    } else {
        oss << "  input1_max: None\n";
    }
    if (input1_cmd) {
        oss << "  input1_cmd: ";
        oss << input1_cmd.value();
        oss << "\n";
    } else {
        oss << "  input1_cmd: None\n";
    }
    if (input2_raw) {
        oss << "  input2_raw: ";
        oss << input2_raw.value();
        oss << "\n";
    } else {
        oss << "  input2_raw: None\n";
    }
    if (input2_typ) {
        oss << "  input2_typ: ";
        oss << input2_typ.value();
        oss << "\n";
    } else {
        oss << "  input2_typ: None\n";
    }
    if (input2_min) {
        oss << "  input2_min: ";
        oss << input2_min.value();
        oss << "\n";
    } else {
        oss << "  input2_min: None\n";
    }
    if (input2_mid) {
        oss << "  input2_mid: ";
        oss << input2_mid.value();
        oss << "\n";
    } else {
        oss << "  input2_mid: None\n";
    }
    if (input2_max) {
        oss << "  input2_max: ";
        oss << input2_max.value();
        oss << "\n";
    } else {
        oss << "  input2_max: None\n";
    }
    if (input2_cmd) {
        oss << "  input2_cmd: ";
        oss << input2_cmd.value();
        oss << "\n";
    } else {
        oss << "  input2_cmd: None\n";
    }
    if (aux_input1_raw) {
        oss << "  aux_input1_raw: ";
        oss << aux_input1_raw.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_raw: None\n";
    }
    if (aux_input1_typ) {
        oss << "  aux_input1_typ: ";
        oss << aux_input1_typ.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_typ: None\n";
    }
    if (aux_input1_min) {
        oss << "  aux_input1_min: ";
        oss << aux_input1_min.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_min: None\n";
    }
    if (aux_input1_mid) {
        oss << "  aux_input1_mid: ";
        oss << aux_input1_mid.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_mid: None\n";
    }
    if (aux_input1_max) {
        oss << "  aux_input1_max: ";
        oss << aux_input1_max.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_max: None\n";
    }
    if (aux_input1_cmd) {
        oss << "  aux_input1_cmd: ";
        oss << aux_input1_cmd.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_cmd: None\n";
    }
    if (aux_input2_raw) {
        oss << "  aux_input2_raw: ";
        oss << aux_input2_raw.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_raw: None\n";
    }
    if (aux_input2_typ) {
        oss << "  aux_input2_typ: ";
        oss << aux_input2_typ.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_typ: None\n";
    }
    if (aux_input2_min) {
        oss << "  aux_input2_min: ";
        oss << aux_input2_min.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_min: None\n";
    }
    if (aux_input2_mid) {
        oss << "  aux_input2_mid: ";
        oss << aux_input2_mid.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_mid: None\n";
    }
    if (aux_input2_max) {
        oss << "  aux_input2_max: ";
        oss << aux_input2_max.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_max: None\n";
    }
    if (aux_input2_cmd) {
        oss << "  aux_input2_cmd: ";
        oss << aux_input2_cmd.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_cmd: None\n";
    }
    if (dc_curr) {
        oss << "  dc_curr: ";
        oss << dc_curr.value();
        oss << "\n";
    } else {
        oss << "  dc_curr: None\n";
    }
    if (rdc_curr) {
        oss << "  rdc_curr: ";
        oss << rdc_curr.value();
        oss << "\n";
    } else {
        oss << "  rdc_curr: None\n";
    }
    if (ldc_curr) {
        oss << "  ldc_curr: ";
        oss << ldc_curr.value();
        oss << "\n";
    } else {
        oss << "  ldc_curr: None\n";
    }
    if (cmdl) {
        oss << "  cmdl: ";
        oss << cmdl.value();
        oss << "\n";
    } else {
        oss << "  cmdl: None\n";
    }
    if (cmdr) {
        oss << "  cmdr: ";
        oss << cmdr.value();
        oss << "\n";
    } else {
        oss << "  cmdr: None\n";
    }
    if (spd_avg) {
        oss << "  spd_avg: ";
        oss << spd_avg.value();
        oss << "\n";
    } else {
        oss << "  spd_avg: None\n";
    }
    if (spdl) {
        oss << "  spdl: ";
        oss << spdl.value();
        oss << "\n";
    } else {
        oss << "  spdl: None\n";
    }
    if (spdr) {
        oss << "  spdr: ";
        oss << spdr.value();
        oss << "\n";
    } else {
        oss << "  spdr: None\n";
    }
    if (filter_rate) {
        oss << "  filter_rate: ";
        oss << filter_rate.value();
        oss << "\n";
    } else {
        oss << "  filter_rate: None\n";
    }
    if (spd_coef) {
        oss << "  spd_coef: ";
        oss << spd_coef.value();
        oss << "\n";
    } else {
        oss << "  spd_coef: None\n";
    }
    if (str_coef) {
        oss << "  str_coef: ";
        oss << str_coef.value();
        oss << "\n";
    } else {
        oss << "  str_coef: None\n";
    }
    if (batv) {
        oss << "  batv: ";
        oss << batv.value();
        oss << "\n";
    } else {
        oss << "  batv: None\n";
    }
    if (temp) {
        oss << "  temp: ";
        oss << temp.value();
        oss << "\n";
    } else {
        oss << "  temp: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes HoverboardEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (ctrl_mod) {
            cbor_encode_int(&mapEncoder, Field::CTRL_MOD_INDEX);
            cbor_encode_int(&mapEncoder, ctrl_mod.value());
            }
    if (ctrl_typ) {
            cbor_encode_int(&mapEncoder, Field::CTRL_TYP_INDEX);
            cbor_encode_int(&mapEncoder, ctrl_typ.value());
            }
    if (cur_mot_max) {
            cbor_encode_int(&mapEncoder, Field::CUR_MOT_MAX_INDEX);
            cbor_encode_int(&mapEncoder, cur_mot_max.value());
            }
    if (rpm_mot_max) {
            cbor_encode_int(&mapEncoder, Field::RPM_MOT_MAX_INDEX);
            cbor_encode_int(&mapEncoder, rpm_mot_max.value());
            }
    if (fi_weak_ena) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_ENA_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_ena.value());
            }
    if (fi_weak_hi) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_HI_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_hi.value());
            }
    if (fi_weak_lo) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_LO_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_lo.value());
            }
    if (fi_weak_max) {
            cbor_encode_int(&mapEncoder, Field::FI_WEAK_MAX_INDEX);
            cbor_encode_int(&mapEncoder, fi_weak_max.value());
            }
    if (phase_adv_max_deg) {
            cbor_encode_int(&mapEncoder, Field::PHASE_ADV_MAX_DEG_INDEX);
            cbor_encode_int(&mapEncoder, phase_adv_max_deg.value());
            }
    if (input1_raw) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_RAW_INDEX);
            cbor_encode_int(&mapEncoder, input1_raw.value());
            }
    if (input1_typ) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_TYP_INDEX);
            cbor_encode_int(&mapEncoder, input1_typ.value());
            }
    if (input1_min) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_MIN_INDEX);
            cbor_encode_int(&mapEncoder, input1_min.value());
            }
    if (input1_mid) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_MID_INDEX);
            cbor_encode_int(&mapEncoder, input1_mid.value());
            }
    if (input1_max) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_MAX_INDEX);
            cbor_encode_int(&mapEncoder, input1_max.value());
            }
    if (input1_cmd) {
            cbor_encode_int(&mapEncoder, Field::INPUT1_CMD_INDEX);
            cbor_encode_int(&mapEncoder, input1_cmd.value());
            }
    if (input2_raw) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_RAW_INDEX);
            cbor_encode_int(&mapEncoder, input2_raw.value());
            }
    if (input2_typ) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_TYP_INDEX);
            cbor_encode_int(&mapEncoder, input2_typ.value());
            }
    if (input2_min) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_MIN_INDEX);
            cbor_encode_int(&mapEncoder, input2_min.value());
            }
    if (input2_mid) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_MID_INDEX);
            cbor_encode_int(&mapEncoder, input2_mid.value());
            }
    if (input2_max) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_MAX_INDEX);
            cbor_encode_int(&mapEncoder, input2_max.value());
            }
    if (input2_cmd) {
            cbor_encode_int(&mapEncoder, Field::INPUT2_CMD_INDEX);
            cbor_encode_int(&mapEncoder, input2_cmd.value());
            }
    if (aux_input1_raw) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_RAW_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_raw.value());
            }
    if (aux_input1_typ) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_TYP_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_typ.value());
            }
    if (aux_input1_min) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_MIN_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_min.value());
            }
    if (aux_input1_mid) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_MID_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_mid.value());
            }
    if (aux_input1_max) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_MAX_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_max.value());
            }
    if (aux_input1_cmd) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT1_CMD_INDEX);
            cbor_encode_int(&mapEncoder, aux_input1_cmd.value());
            }
    if (aux_input2_raw) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_RAW_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_raw.value());
            }
    if (aux_input2_typ) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_TYP_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_typ.value());
            }
    if (aux_input2_min) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_MIN_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_min.value());
            }
    if (aux_input2_mid) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_MID_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_mid.value());
            }
    if (aux_input2_max) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_MAX_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_max.value());
            }
    if (aux_input2_cmd) {
            cbor_encode_int(&mapEncoder, Field::AUX_INPUT2_CMD_INDEX);
            cbor_encode_int(&mapEncoder, aux_input2_cmd.value());
            }
    if (dc_curr) {
            cbor_encode_int(&mapEncoder, Field::DC_CURR_INDEX);
            cbor_encode_float(&mapEncoder, dc_curr.value());
            }
    if (rdc_curr) {
            cbor_encode_int(&mapEncoder, Field::RDC_CURR_INDEX);
            cbor_encode_float(&mapEncoder, rdc_curr.value());
            }
    if (ldc_curr) {
            cbor_encode_int(&mapEncoder, Field::LDC_CURR_INDEX);
            cbor_encode_float(&mapEncoder, ldc_curr.value());
            }
    if (cmdl) {
            cbor_encode_int(&mapEncoder, Field::CMDL_INDEX);
            cbor_encode_int(&mapEncoder, cmdl.value());
            }
    if (cmdr) {
            cbor_encode_int(&mapEncoder, Field::CMDR_INDEX);
            cbor_encode_int(&mapEncoder, cmdr.value());
            }
    if (spd_avg) {
            cbor_encode_int(&mapEncoder, Field::SPD_AVG_INDEX);
            cbor_encode_int(&mapEncoder, spd_avg.value());
            }
    if (spdl) {
            cbor_encode_int(&mapEncoder, Field::SPDL_INDEX);
            cbor_encode_int(&mapEncoder, spdl.value());
            }
    if (spdr) {
            cbor_encode_int(&mapEncoder, Field::SPDR_INDEX);
            cbor_encode_int(&mapEncoder, spdr.value());
            }
    if (filter_rate) {
            cbor_encode_int(&mapEncoder, Field::FILTER_RATE_INDEX);
            cbor_encode_int(&mapEncoder, filter_rate.value());
            }
    if (spd_coef) {
            cbor_encode_int(&mapEncoder, Field::SPD_COEF_INDEX);
            cbor_encode_int(&mapEncoder, spd_coef.value());
            }
    if (str_coef) {
            cbor_encode_int(&mapEncoder, Field::STR_COEF_INDEX);
            cbor_encode_int(&mapEncoder, str_coef.value());
            }
    if (batv) {
            cbor_encode_int(&mapEncoder, Field::BATV_INDEX);
            cbor_encode_float(&mapEncoder, batv.value());
            }
    if (temp) {
            cbor_encode_int(&mapEncoder, Field::TEMP_INDEX);
            cbor_encode_float(&mapEncoder, temp.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

HoverboardEvent* HoverboardEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    HoverboardEvent* msg = new HoverboardEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::CTRL_MOD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *ctrl_mod = v;
}
                break;
            }
            
            case Field::CTRL_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *ctrl_typ = v;
}
                break;
            }
            
            case Field::CUR_MOT_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *cur_mot_max = v;
}
                break;
            }
            
            case Field::RPM_MOT_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *rpm_mot_max = v;
}
                break;
            }
            
            case Field::FI_WEAK_ENA_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_ena = v;
}
                break;
            }
            
            case Field::FI_WEAK_HI_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_hi = v;
}
                break;
            }
            
            case Field::FI_WEAK_LO_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_lo = v;
}
                break;
            }
            
            case Field::FI_WEAK_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *fi_weak_max = v;
}
                break;
            }
            
            case Field::PHASE_ADV_MAX_DEG_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *phase_adv_max_deg = v;
}
                break;
            }
            
            case Field::INPUT1_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_raw = v;
}
                break;
            }
            
            case Field::INPUT1_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_typ = v;
}
                break;
            }
            
            case Field::INPUT1_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_min = v;
}
                break;
            }
            
            case Field::INPUT1_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_mid = v;
}
                break;
            }
            
            case Field::INPUT1_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_max = v;
}
                break;
            }
            
            case Field::INPUT1_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input1_cmd = v;
}
                break;
            }
            
            case Field::INPUT2_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_raw = v;
}
                break;
            }
            
            case Field::INPUT2_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_typ = v;
}
                break;
            }
            
            case Field::INPUT2_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_min = v;
}
                break;
            }
            
            case Field::INPUT2_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_mid = v;
}
                break;
            }
            
            case Field::INPUT2_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_max = v;
}
                break;
            }
            
            case Field::INPUT2_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *input2_cmd = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_raw = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_typ = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_min = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_mid = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_max = v;
}
                break;
            }
            
            case Field::AUX_INPUT1_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input1_cmd = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_RAW_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_raw = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_TYP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_typ = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_MIN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_min = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_MID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_mid = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_MAX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_max = v;
}
                break;
            }
            
            case Field::AUX_INPUT2_CMD_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *aux_input2_cmd = v;
}
                break;
            }
            
            case Field::DC_CURR_INDEX:{cbor_value_get_float(&mapIt, &*dc_curr);
                break;
            }
            
            case Field::RDC_CURR_INDEX:{cbor_value_get_float(&mapIt, &*rdc_curr);
                break;
            }
            
            case Field::LDC_CURR_INDEX:{cbor_value_get_float(&mapIt, &*ldc_curr);
                break;
            }
            
            case Field::CMDL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *cmdl = v;
}
                break;
            }
            
            case Field::CMDR_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *cmdr = v;
}
                break;
            }
            
            case Field::SPD_AVG_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spd_avg = v;
}
                break;
            }
            
            case Field::SPDL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spdl = v;
}
                break;
            }
            
            case Field::SPDR_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spdr = v;
}
                break;
            }
            
            case Field::FILTER_RATE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *filter_rate = v;
}
                break;
            }
            
            case Field::SPD_COEF_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *spd_coef = v;
}
                break;
            }
            
            case Field::STR_COEF_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *str_coef = v;
}
                break;
            }
            
            case Field::BATV_INDEX:{cbor_value_get_float(&mapIt, &*batv);
                break;
            }
            
            case Field::TEMP_INDEX:{cbor_value_get_float(&mapIt, &*temp);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string HoverboardEvent::to_string() const {
    std::ostringstream oss;
    oss << "HoverboardEvent {\n";
    if (ctrl_mod) {
        oss << "  ctrl_mod: ";
        oss << ctrl_mod.value();
        oss << "\n";
    } else {
        oss << "  ctrl_mod: None\n";
    }
    if (ctrl_typ) {
        oss << "  ctrl_typ: ";
        oss << ctrl_typ.value();
        oss << "\n";
    } else {
        oss << "  ctrl_typ: None\n";
    }
    if (cur_mot_max) {
        oss << "  cur_mot_max: ";
        oss << cur_mot_max.value();
        oss << "\n";
    } else {
        oss << "  cur_mot_max: None\n";
    }
    if (rpm_mot_max) {
        oss << "  rpm_mot_max: ";
        oss << rpm_mot_max.value();
        oss << "\n";
    } else {
        oss << "  rpm_mot_max: None\n";
    }
    if (fi_weak_ena) {
        oss << "  fi_weak_ena: ";
        oss << fi_weak_ena.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_ena: None\n";
    }
    if (fi_weak_hi) {
        oss << "  fi_weak_hi: ";
        oss << fi_weak_hi.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_hi: None\n";
    }
    if (fi_weak_lo) {
        oss << "  fi_weak_lo: ";
        oss << fi_weak_lo.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_lo: None\n";
    }
    if (fi_weak_max) {
        oss << "  fi_weak_max: ";
        oss << fi_weak_max.value();
        oss << "\n";
    } else {
        oss << "  fi_weak_max: None\n";
    }
    if (phase_adv_max_deg) {
        oss << "  phase_adv_max_deg: ";
        oss << phase_adv_max_deg.value();
        oss << "\n";
    } else {
        oss << "  phase_adv_max_deg: None\n";
    }
    if (input1_raw) {
        oss << "  input1_raw: ";
        oss << input1_raw.value();
        oss << "\n";
    } else {
        oss << "  input1_raw: None\n";
    }
    if (input1_typ) {
        oss << "  input1_typ: ";
        oss << input1_typ.value();
        oss << "\n";
    } else {
        oss << "  input1_typ: None\n";
    }
    if (input1_min) {
        oss << "  input1_min: ";
        oss << input1_min.value();
        oss << "\n";
    } else {
        oss << "  input1_min: None\n";
    }
    if (input1_mid) {
        oss << "  input1_mid: ";
        oss << input1_mid.value();
        oss << "\n";
    } else {
        oss << "  input1_mid: None\n";
    }
    if (input1_max) {
        oss << "  input1_max: ";
        oss << input1_max.value();
        oss << "\n";
    } else {
        oss << "  input1_max: None\n";
    }
    if (input1_cmd) {
        oss << "  input1_cmd: ";
        oss << input1_cmd.value();
        oss << "\n";
    } else {
        oss << "  input1_cmd: None\n";
    }
    if (input2_raw) {
        oss << "  input2_raw: ";
        oss << input2_raw.value();
        oss << "\n";
    } else {
        oss << "  input2_raw: None\n";
    }
    if (input2_typ) {
        oss << "  input2_typ: ";
        oss << input2_typ.value();
        oss << "\n";
    } else {
        oss << "  input2_typ: None\n";
    }
    if (input2_min) {
        oss << "  input2_min: ";
        oss << input2_min.value();
        oss << "\n";
    } else {
        oss << "  input2_min: None\n";
    }
    if (input2_mid) {
        oss << "  input2_mid: ";
        oss << input2_mid.value();
        oss << "\n";
    } else {
        oss << "  input2_mid: None\n";
    }
    if (input2_max) {
        oss << "  input2_max: ";
        oss << input2_max.value();
        oss << "\n";
    } else {
        oss << "  input2_max: None\n";
    }
    if (input2_cmd) {
        oss << "  input2_cmd: ";
        oss << input2_cmd.value();
        oss << "\n";
    } else {
        oss << "  input2_cmd: None\n";
    }
    if (aux_input1_raw) {
        oss << "  aux_input1_raw: ";
        oss << aux_input1_raw.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_raw: None\n";
    }
    if (aux_input1_typ) {
        oss << "  aux_input1_typ: ";
        oss << aux_input1_typ.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_typ: None\n";
    }
    if (aux_input1_min) {
        oss << "  aux_input1_min: ";
        oss << aux_input1_min.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_min: None\n";
    }
    if (aux_input1_mid) {
        oss << "  aux_input1_mid: ";
        oss << aux_input1_mid.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_mid: None\n";
    }
    if (aux_input1_max) {
        oss << "  aux_input1_max: ";
        oss << aux_input1_max.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_max: None\n";
    }
    if (aux_input1_cmd) {
        oss << "  aux_input1_cmd: ";
        oss << aux_input1_cmd.value();
        oss << "\n";
    } else {
        oss << "  aux_input1_cmd: None\n";
    }
    if (aux_input2_raw) {
        oss << "  aux_input2_raw: ";
        oss << aux_input2_raw.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_raw: None\n";
    }
    if (aux_input2_typ) {
        oss << "  aux_input2_typ: ";
        oss << aux_input2_typ.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_typ: None\n";
    }
    if (aux_input2_min) {
        oss << "  aux_input2_min: ";
        oss << aux_input2_min.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_min: None\n";
    }
    if (aux_input2_mid) {
        oss << "  aux_input2_mid: ";
        oss << aux_input2_mid.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_mid: None\n";
    }
    if (aux_input2_max) {
        oss << "  aux_input2_max: ";
        oss << aux_input2_max.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_max: None\n";
    }
    if (aux_input2_cmd) {
        oss << "  aux_input2_cmd: ";
        oss << aux_input2_cmd.value();
        oss << "\n";
    } else {
        oss << "  aux_input2_cmd: None\n";
    }
    if (dc_curr) {
        oss << "  dc_curr: ";
        oss << dc_curr.value();
        oss << "\n";
    } else {
        oss << "  dc_curr: None\n";
    }
    if (rdc_curr) {
        oss << "  rdc_curr: ";
        oss << rdc_curr.value();
        oss << "\n";
    } else {
        oss << "  rdc_curr: None\n";
    }
    if (ldc_curr) {
        oss << "  ldc_curr: ";
        oss << ldc_curr.value();
        oss << "\n";
    } else {
        oss << "  ldc_curr: None\n";
    }
    if (cmdl) {
        oss << "  cmdl: ";
        oss << cmdl.value();
        oss << "\n";
    } else {
        oss << "  cmdl: None\n";
    }
    if (cmdr) {
        oss << "  cmdr: ";
        oss << cmdr.value();
        oss << "\n";
    } else {
        oss << "  cmdr: None\n";
    }
    if (spd_avg) {
        oss << "  spd_avg: ";
        oss << spd_avg.value();
        oss << "\n";
    } else {
        oss << "  spd_avg: None\n";
    }
    if (spdl) {
        oss << "  spdl: ";
        oss << spdl.value();
        oss << "\n";
    } else {
        oss << "  spdl: None\n";
    }
    if (spdr) {
        oss << "  spdr: ";
        oss << spdr.value();
        oss << "\n";
    } else {
        oss << "  spdr: None\n";
    }
    if (filter_rate) {
        oss << "  filter_rate: ";
        oss << filter_rate.value();
        oss << "\n";
    } else {
        oss << "  filter_rate: None\n";
    }
    if (spd_coef) {
        oss << "  spd_coef: ";
        oss << spd_coef.value();
        oss << "\n";
    } else {
        oss << "  spd_coef: None\n";
    }
    if (str_coef) {
        oss << "  str_coef: ";
        oss << str_coef.value();
        oss << "\n";
    } else {
        oss << "  str_coef: None\n";
    }
    if (batv) {
        oss << "  batv: ";
        oss << batv.value();
        oss << "\n";
    } else {
        oss << "  batv: None\n";
    }
    if (temp) {
        oss << "  temp: ";
        oss << temp.value();
        oss << "\n";
    } else {
        oss << "  temp: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes HoverboardRequest::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (speed) {
            cbor_encode_int(&mapEncoder, Field::SPEED_INDEX);
            cbor_encode_int(&mapEncoder, speed.value());
            }
    if (steer) {
            cbor_encode_int(&mapEncoder, Field::STEER_INDEX);
            cbor_encode_int(&mapEncoder, steer.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

HoverboardRequest* HoverboardRequest::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    HoverboardRequest* msg = new HoverboardRequest();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::SPEED_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *speed = v;
}
                break;
            }
            
            case Field::STEER_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *steer = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string HoverboardRequest::to_string() const {
    std::ostringstream oss;
    oss << "HoverboardRequest {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (speed) {
        oss << "  speed: ";
        oss << speed.value();
        oss << "\n";
    } else {
        oss << "  speed: None\n";
    }
    if (steer) {
        oss << "  steer: ";
        oss << steer.value();
        oss << "\n";
    } else {
        oss << "  steer: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes HoverboardReply::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (error_code) {
            cbor_encode_int(&mapEncoder, Field::ERROR_CODE_INDEX);
            cbor_encode_int(&mapEncoder, error_code.value());
            }
    if (message) {
            cbor_encode_int(&mapEncoder, Field::MESSAGE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, message.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

HoverboardReply* HoverboardReply::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    HoverboardReply* msg = new HoverboardReply();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::ERROR_CODE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *error_code = v;
}
                break;
            }
            
            case Field::MESSAGE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *message = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string HoverboardReply::to_string() const {
    std::ostringstream oss;
    oss << "HoverboardReply {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (error_code) {
        oss << "  error_code: ";
        oss << error_code.value();
        oss << "\n";
    } else {
        oss << "  error_code: None\n";
    }
    if (message) {
        oss << "  message: ";
        oss << "\"" << message.value() << "\"";
        oss << "\n";
    } else {
        oss << "  message: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes TouchPoint::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (active) {
            cbor_encode_int(&mapEncoder, Field::ACTIVE_INDEX);
            cbor_encode_boolean(&mapEncoder, active.value());
            }
    if (id) {
            cbor_encode_int(&mapEncoder, Field::ID_INDEX);
            cbor_encode_int(&mapEncoder, id.value());
            }
    if (x) {
            cbor_encode_int(&mapEncoder, Field::X_INDEX);
            cbor_encode_int(&mapEncoder, x.value());
            }
    if (y) {
            cbor_encode_int(&mapEncoder, Field::Y_INDEX);
            cbor_encode_int(&mapEncoder, y.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

TouchPoint* TouchPoint::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    TouchPoint* msg = new TouchPoint();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::ACTIVE_INDEX:{cbor_value_get_boolean(&mapIt, &*active);
                break;
            }
            
            case Field::ID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *id = v;
}
                break;
            }
            
            case Field::X_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *x = v;
}
                break;
            }
            
            case Field::Y_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *y = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string TouchPoint::to_string() const {
    std::ostringstream oss;
    oss << "TouchPoint {\n";
    if (active) {
        oss << "  active: ";
        oss << (active.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  active: None\n";
    }
    if (id) {
        oss << "  id: ";
        oss << id.value();
        oss << "\n";
    } else {
        oss << "  id: None\n";
    }
    if (x) {
        oss << "  x: ";
        oss << x.value();
        oss << "\n";
    } else {
        oss << "  x: None\n";
    }
    if (y) {
        oss << "  y: ";
        oss << y.value();
        oss << "\n";
    } else {
        oss << "  y: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes Ps4Event::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (button_left) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_LEFT_INDEX);
            cbor_encode_boolean(&mapEncoder, button_left.value());
            }
    if (button_right) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_RIGHT_INDEX);
            cbor_encode_boolean(&mapEncoder, button_right.value());
            }
    if (button_up) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_UP_INDEX);
            cbor_encode_boolean(&mapEncoder, button_up.value());
            }
    if (button_down) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_DOWN_INDEX);
            cbor_encode_boolean(&mapEncoder, button_down.value());
            }
    if (button_square) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_SQUARE_INDEX);
            cbor_encode_boolean(&mapEncoder, button_square.value());
            }
    if (button_cross) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_CROSS_INDEX);
            cbor_encode_boolean(&mapEncoder, button_cross.value());
            }
    if (button_circle) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_CIRCLE_INDEX);
            cbor_encode_boolean(&mapEncoder, button_circle.value());
            }
    if (button_triangle) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_TRIANGLE_INDEX);
            cbor_encode_boolean(&mapEncoder, button_triangle.value());
            }
    if (button_left_shoulder) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_LEFT_SHOULDER_INDEX);
            cbor_encode_boolean(&mapEncoder, button_left_shoulder.value());
            }
    if (button_right_shoulder) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_RIGHT_SHOULDER_INDEX);
            cbor_encode_boolean(&mapEncoder, button_right_shoulder.value());
            }
    if (button_left_trigger) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_LEFT_TRIGGER_INDEX);
            cbor_encode_boolean(&mapEncoder, button_left_trigger.value());
            }
    if (button_right_trigger) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_RIGHT_TRIGGER_INDEX);
            cbor_encode_boolean(&mapEncoder, button_right_trigger.value());
            }
    if (button_left_joystick) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_LEFT_JOYSTICK_INDEX);
            cbor_encode_boolean(&mapEncoder, button_left_joystick.value());
            }
    if (button_right_joystick) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_RIGHT_JOYSTICK_INDEX);
            cbor_encode_boolean(&mapEncoder, button_right_joystick.value());
            }
    if (button_share) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_SHARE_INDEX);
            cbor_encode_boolean(&mapEncoder, button_share.value());
            }
    if (button_options) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_OPTIONS_INDEX);
            cbor_encode_boolean(&mapEncoder, button_options.value());
            }
    if (button_touchpad) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_TOUCHPAD_INDEX);
            cbor_encode_boolean(&mapEncoder, button_touchpad.value());
            }
    if (button_ps) {
            cbor_encode_int(&mapEncoder, Field::BUTTON_PS_INDEX);
            cbor_encode_boolean(&mapEncoder, button_ps.value());
            }
    if (axis_lx) {
            cbor_encode_int(&mapEncoder, Field::AXIS_LX_INDEX);
            cbor_encode_int(&mapEncoder, axis_lx.value());
            }
    if (axis_ly) {
            cbor_encode_int(&mapEncoder, Field::AXIS_LY_INDEX);
            cbor_encode_int(&mapEncoder, axis_ly.value());
            }
    if (axis_rx) {
            cbor_encode_int(&mapEncoder, Field::AXIS_RX_INDEX);
            cbor_encode_int(&mapEncoder, axis_rx.value());
            }
    if (axis_ry) {
            cbor_encode_int(&mapEncoder, Field::AXIS_RY_INDEX);
            cbor_encode_int(&mapEncoder, axis_ry.value());
            }
    if (gyro_x) {
            cbor_encode_int(&mapEncoder, Field::GYRO_X_INDEX);
            cbor_encode_int(&mapEncoder, gyro_x.value());
            }
    if (gyro_y) {
            cbor_encode_int(&mapEncoder, Field::GYRO_Y_INDEX);
            cbor_encode_int(&mapEncoder, gyro_y.value());
            }
    if (gyro_z) {
            cbor_encode_int(&mapEncoder, Field::GYRO_Z_INDEX);
            cbor_encode_int(&mapEncoder, gyro_z.value());
            }
    if (accel_x) {
            cbor_encode_int(&mapEncoder, Field::ACCEL_X_INDEX);
            cbor_encode_int(&mapEncoder, accel_x.value());
            }
    if (accel_y) {
            cbor_encode_int(&mapEncoder, Field::ACCEL_Y_INDEX);
            cbor_encode_int(&mapEncoder, accel_y.value());
            }
    if (accel_z) {
            cbor_encode_int(&mapEncoder, Field::ACCEL_Z_INDEX);
            cbor_encode_int(&mapEncoder, accel_z.value());
            }
    if (connected) {
            cbor_encode_int(&mapEncoder, Field::CONNECTED_INDEX);
            cbor_encode_boolean(&mapEncoder, connected.value());
            }
    if (battery_level) {
            cbor_encode_int(&mapEncoder, Field::BATTERY_LEVEL_INDEX);
            cbor_encode_int(&mapEncoder, battery_level.value());
            }
    if (bluetooth) {
            cbor_encode_int(&mapEncoder, Field::BLUETOOTH_INDEX);
            cbor_encode_boolean(&mapEncoder, bluetooth.value());
            }
    if (debug) {
            cbor_encode_int(&mapEncoder, Field::DEBUG_INDEX);
            cbor_encode_text_stringz(&mapEncoder, debug.value().c_str());
            }
    if (temp) {
            cbor_encode_int(&mapEncoder, Field::TEMP_INDEX);
            cbor_encode_int(&mapEncoder, temp.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

Ps4Event* Ps4Event::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    Ps4Event* msg = new Ps4Event();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::BUTTON_LEFT_INDEX:{cbor_value_get_boolean(&mapIt, &*button_left);
                break;
            }
            
            case Field::BUTTON_RIGHT_INDEX:{cbor_value_get_boolean(&mapIt, &*button_right);
                break;
            }
            
            case Field::BUTTON_UP_INDEX:{cbor_value_get_boolean(&mapIt, &*button_up);
                break;
            }
            
            case Field::BUTTON_DOWN_INDEX:{cbor_value_get_boolean(&mapIt, &*button_down);
                break;
            }
            
            case Field::BUTTON_SQUARE_INDEX:{cbor_value_get_boolean(&mapIt, &*button_square);
                break;
            }
            
            case Field::BUTTON_CROSS_INDEX:{cbor_value_get_boolean(&mapIt, &*button_cross);
                break;
            }
            
            case Field::BUTTON_CIRCLE_INDEX:{cbor_value_get_boolean(&mapIt, &*button_circle);
                break;
            }
            
            case Field::BUTTON_TRIANGLE_INDEX:{cbor_value_get_boolean(&mapIt, &*button_triangle);
                break;
            }
            
            case Field::BUTTON_LEFT_SHOULDER_INDEX:{cbor_value_get_boolean(&mapIt, &*button_left_shoulder);
                break;
            }
            
            case Field::BUTTON_RIGHT_SHOULDER_INDEX:{cbor_value_get_boolean(&mapIt, &*button_right_shoulder);
                break;
            }
            
            case Field::BUTTON_LEFT_TRIGGER_INDEX:{cbor_value_get_boolean(&mapIt, &*button_left_trigger);
                break;
            }
            
            case Field::BUTTON_RIGHT_TRIGGER_INDEX:{cbor_value_get_boolean(&mapIt, &*button_right_trigger);
                break;
            }
            
            case Field::BUTTON_LEFT_JOYSTICK_INDEX:{cbor_value_get_boolean(&mapIt, &*button_left_joystick);
                break;
            }
            
            case Field::BUTTON_RIGHT_JOYSTICK_INDEX:{cbor_value_get_boolean(&mapIt, &*button_right_joystick);
                break;
            }
            
            case Field::BUTTON_SHARE_INDEX:{cbor_value_get_boolean(&mapIt, &*button_share);
                break;
            }
            
            case Field::BUTTON_OPTIONS_INDEX:{cbor_value_get_boolean(&mapIt, &*button_options);
                break;
            }
            
            case Field::BUTTON_TOUCHPAD_INDEX:{cbor_value_get_boolean(&mapIt, &*button_touchpad);
                break;
            }
            
            case Field::BUTTON_PS_INDEX:{cbor_value_get_boolean(&mapIt, &*button_ps);
                break;
            }
            
            case Field::AXIS_LX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *axis_lx = v;
}
                break;
            }
            
            case Field::AXIS_LY_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *axis_ly = v;
}
                break;
            }
            
            case Field::AXIS_RX_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *axis_rx = v;
}
                break;
            }
            
            case Field::AXIS_RY_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *axis_ry = v;
}
                break;
            }
            
            case Field::GYRO_X_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *gyro_x = v;
}
                break;
            }
            
            case Field::GYRO_Y_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *gyro_y = v;
}
                break;
            }
            
            case Field::GYRO_Z_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *gyro_z = v;
}
                break;
            }
            
            case Field::ACCEL_X_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *accel_x = v;
}
                break;
            }
            
            case Field::ACCEL_Y_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *accel_y = v;
}
                break;
            }
            
            case Field::ACCEL_Z_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *accel_z = v;
}
                break;
            }
            
            case Field::CONNECTED_INDEX:{cbor_value_get_boolean(&mapIt, &*connected);
                break;
            }
            
            case Field::BATTERY_LEVEL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *battery_level = v;
}
                break;
            }
            
            case Field::BLUETOOTH_INDEX:{cbor_value_get_boolean(&mapIt, &*bluetooth);
                break;
            }
            
            case Field::DEBUG_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *debug = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::TEMP_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *temp = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string Ps4Event::to_string() const {
    std::ostringstream oss;
    oss << "Ps4Event {\n";
    if (button_left) {
        oss << "  button_left: ";
        oss << (button_left.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_left: None\n";
    }
    if (button_right) {
        oss << "  button_right: ";
        oss << (button_right.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_right: None\n";
    }
    if (button_up) {
        oss << "  button_up: ";
        oss << (button_up.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_up: None\n";
    }
    if (button_down) {
        oss << "  button_down: ";
        oss << (button_down.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_down: None\n";
    }
    if (button_square) {
        oss << "  button_square: ";
        oss << (button_square.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_square: None\n";
    }
    if (button_cross) {
        oss << "  button_cross: ";
        oss << (button_cross.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_cross: None\n";
    }
    if (button_circle) {
        oss << "  button_circle: ";
        oss << (button_circle.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_circle: None\n";
    }
    if (button_triangle) {
        oss << "  button_triangle: ";
        oss << (button_triangle.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_triangle: None\n";
    }
    if (button_left_shoulder) {
        oss << "  button_left_shoulder: ";
        oss << (button_left_shoulder.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_left_shoulder: None\n";
    }
    if (button_right_shoulder) {
        oss << "  button_right_shoulder: ";
        oss << (button_right_shoulder.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_right_shoulder: None\n";
    }
    if (button_left_trigger) {
        oss << "  button_left_trigger: ";
        oss << (button_left_trigger.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_left_trigger: None\n";
    }
    if (button_right_trigger) {
        oss << "  button_right_trigger: ";
        oss << (button_right_trigger.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_right_trigger: None\n";
    }
    if (button_left_joystick) {
        oss << "  button_left_joystick: ";
        oss << (button_left_joystick.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_left_joystick: None\n";
    }
    if (button_right_joystick) {
        oss << "  button_right_joystick: ";
        oss << (button_right_joystick.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_right_joystick: None\n";
    }
    if (button_share) {
        oss << "  button_share: ";
        oss << (button_share.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_share: None\n";
    }
    if (button_options) {
        oss << "  button_options: ";
        oss << (button_options.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_options: None\n";
    }
    if (button_touchpad) {
        oss << "  button_touchpad: ";
        oss << (button_touchpad.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_touchpad: None\n";
    }
    if (button_ps) {
        oss << "  button_ps: ";
        oss << (button_ps.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  button_ps: None\n";
    }
    if (axis_lx) {
        oss << "  axis_lx: ";
        oss << axis_lx.value();
        oss << "\n";
    } else {
        oss << "  axis_lx: None\n";
    }
    if (axis_ly) {
        oss << "  axis_ly: ";
        oss << axis_ly.value();
        oss << "\n";
    } else {
        oss << "  axis_ly: None\n";
    }
    if (axis_rx) {
        oss << "  axis_rx: ";
        oss << axis_rx.value();
        oss << "\n";
    } else {
        oss << "  axis_rx: None\n";
    }
    if (axis_ry) {
        oss << "  axis_ry: ";
        oss << axis_ry.value();
        oss << "\n";
    } else {
        oss << "  axis_ry: None\n";
    }
    if (gyro_x) {
        oss << "  gyro_x: ";
        oss << gyro_x.value();
        oss << "\n";
    } else {
        oss << "  gyro_x: None\n";
    }
    if (gyro_y) {
        oss << "  gyro_y: ";
        oss << gyro_y.value();
        oss << "\n";
    } else {
        oss << "  gyro_y: None\n";
    }
    if (gyro_z) {
        oss << "  gyro_z: ";
        oss << gyro_z.value();
        oss << "\n";
    } else {
        oss << "  gyro_z: None\n";
    }
    if (accel_x) {
        oss << "  accel_x: ";
        oss << accel_x.value();
        oss << "\n";
    } else {
        oss << "  accel_x: None\n";
    }
    if (accel_y) {
        oss << "  accel_y: ";
        oss << accel_y.value();
        oss << "\n";
    } else {
        oss << "  accel_y: None\n";
    }
    if (accel_z) {
        oss << "  accel_z: ";
        oss << accel_z.value();
        oss << "\n";
    } else {
        oss << "  accel_z: None\n";
    }
    if (connected) {
        oss << "  connected: ";
        oss << (connected.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  connected: None\n";
    }
    if (battery_level) {
        oss << "  battery_level: ";
        oss << battery_level.value();
        oss << "\n";
    } else {
        oss << "  battery_level: None\n";
    }
    if (bluetooth) {
        oss << "  bluetooth: ";
        oss << (bluetooth.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  bluetooth: None\n";
    }
    if (debug) {
        oss << "  debug: ";
        oss << "\"" << debug.value() << "\"";
        oss << "\n";
    } else {
        oss << "  debug: None\n";
    }
    if (temp) {
        oss << "  temp: ";
        oss << temp.value();
        oss << "\n";
    } else {
        oss << "  temp: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes Ps4Request::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (rumble_small) {
            cbor_encode_int(&mapEncoder, Field::RUMBLE_SMALL_INDEX);
            cbor_encode_int(&mapEncoder, rumble_small.value());
            }
    if (rumble_large) {
            cbor_encode_int(&mapEncoder, Field::RUMBLE_LARGE_INDEX);
            cbor_encode_int(&mapEncoder, rumble_large.value());
            }
    if (led_red) {
            cbor_encode_int(&mapEncoder, Field::LED_RED_INDEX);
            cbor_encode_int(&mapEncoder, led_red.value());
            }
    if (led_green) {
            cbor_encode_int(&mapEncoder, Field::LED_GREEN_INDEX);
            cbor_encode_int(&mapEncoder, led_green.value());
            }
    if (led_blue) {
            cbor_encode_int(&mapEncoder, Field::LED_BLUE_INDEX);
            cbor_encode_int(&mapEncoder, led_blue.value());
            }
    if (led_flash_on) {
            cbor_encode_int(&mapEncoder, Field::LED_FLASH_ON_INDEX);
            cbor_encode_int(&mapEncoder, led_flash_on.value());
            }
    if (led_flash_off) {
            cbor_encode_int(&mapEncoder, Field::LED_FLASH_OFF_INDEX);
            cbor_encode_int(&mapEncoder, led_flash_off.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

Ps4Request* Ps4Request::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    Ps4Request* msg = new Ps4Request();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::RUMBLE_SMALL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *rumble_small = v;
}
                break;
            }
            
            case Field::RUMBLE_LARGE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *rumble_large = v;
}
                break;
            }
            
            case Field::LED_RED_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *led_red = v;
}
                break;
            }
            
            case Field::LED_GREEN_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *led_green = v;
}
                break;
            }
            
            case Field::LED_BLUE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *led_blue = v;
}
                break;
            }
            
            case Field::LED_FLASH_ON_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *led_flash_on = v;
}
                break;
            }
            
            case Field::LED_FLASH_OFF_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *led_flash_off = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string Ps4Request::to_string() const {
    std::ostringstream oss;
    oss << "Ps4Request {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (rumble_small) {
        oss << "  rumble_small: ";
        oss << rumble_small.value();
        oss << "\n";
    } else {
        oss << "  rumble_small: None\n";
    }
    if (rumble_large) {
        oss << "  rumble_large: ";
        oss << rumble_large.value();
        oss << "\n";
    } else {
        oss << "  rumble_large: None\n";
    }
    if (led_red) {
        oss << "  led_red: ";
        oss << led_red.value();
        oss << "\n";
    } else {
        oss << "  led_red: None\n";
    }
    if (led_green) {
        oss << "  led_green: ";
        oss << led_green.value();
        oss << "\n";
    } else {
        oss << "  led_green: None\n";
    }
    if (led_blue) {
        oss << "  led_blue: ";
        oss << led_blue.value();
        oss << "\n";
    } else {
        oss << "  led_blue: None\n";
    }
    if (led_flash_on) {
        oss << "  led_flash_on: ";
        oss << led_flash_on.value();
        oss << "\n";
    } else {
        oss << "  led_flash_on: None\n";
    }
    if (led_flash_off) {
        oss << "  led_flash_off: ";
        oss << led_flash_off.value();
        oss << "\n";
    } else {
        oss << "  led_flash_off: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes CameraEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (width) {
            cbor_encode_int(&mapEncoder, Field::WIDTH_INDEX);
            cbor_encode_int(&mapEncoder, width.value());
            }
    if (height) {
            cbor_encode_int(&mapEncoder, Field::HEIGHT_INDEX);
            cbor_encode_int(&mapEncoder, height.value());
            }
    if (format) {
            cbor_encode_int(&mapEncoder, Field::FORMAT_INDEX);
            cbor_encode_text_stringz(&mapEncoder, format.value().c_str());
            }
    if (data) {
            cbor_encode_int(&mapEncoder, Field::DATA_INDEX);
            cbor_encode_byte_string(&mapEncoder, data.value().data(), data.value().size());
            }
    if (led) {
            cbor_encode_int(&mapEncoder, Field::LED_INDEX);
            cbor_encode_boolean(&mapEncoder, led.value());
            }
    if (quality) {
            cbor_encode_int(&mapEncoder, Field::QUALITY_INDEX);
            cbor_encode_int(&mapEncoder, quality.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

CameraEvent* CameraEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    CameraEvent* msg = new CameraEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::WIDTH_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *width = v;
}
                break;
            }
            
            case Field::HEIGHT_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *height = v;
}
                break;
            }
            
            case Field::FORMAT_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *format = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::DATA_INDEX:{{
    uint8_t tmpbuf[512];
    size_t tmplen = sizeof(tmpbuf);
    if (cbor_value_is_byte_string(&mapIt)) {
        cbor_value_copy_byte_string(&mapIt, tmpbuf, &tmplen, &mapIt);
        *data = Bytes(tmpbuf, tmpbuf + tmplen);
    }
}
                break;
            }
            
            case Field::LED_INDEX:{cbor_value_get_boolean(&mapIt, &*led);
                break;
            }
            
            case Field::QUALITY_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *quality = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string CameraEvent::to_string() const {
    std::ostringstream oss;
    oss << "CameraEvent {\n";
    if (width) {
        oss << "  width: ";
        oss << width.value();
        oss << "\n";
    } else {
        oss << "  width: None\n";
    }
    if (height) {
        oss << "  height: ";
        oss << height.value();
        oss << "\n";
    } else {
        oss << "  height: None\n";
    }
    if (format) {
        oss << "  format: ";
        oss << "\"" << format.value() << "\"";
        oss << "\n";
    } else {
        oss << "  format: None\n";
    }
    if (data) {
        oss << "  data: ";
        oss << "Bytes[" << data.value().size() << "]";
        oss << "\n";
    } else {
        oss << "  data: None\n";
    }
    if (led) {
        oss << "  led: ";
        oss << (led.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  led: None\n";
    }
    if (quality) {
        oss << "  quality: ";
        oss << quality.value();
        oss << "\n";
    } else {
        oss << "  quality: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes CameraRequest::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (led) {
            cbor_encode_int(&mapEncoder, Field::LED_INDEX);
            cbor_encode_boolean(&mapEncoder, led.value());
            }
    if (capture_tcp_destination) {
            cbor_encode_int(&mapEncoder, Field::CAPTURE_TCP_DESTINATION_INDEX);
            cbor_encode_text_stringz(&mapEncoder, capture_tcp_destination.value().c_str());
            }
    if (quality) {
            cbor_encode_int(&mapEncoder, Field::QUALITY_INDEX);
            cbor_encode_int(&mapEncoder, quality.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

CameraRequest* CameraRequest::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    CameraRequest* msg = new CameraRequest();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::LED_INDEX:{cbor_value_get_boolean(&mapIt, &*led);
                break;
            }
            
            case Field::CAPTURE_TCP_DESTINATION_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *capture_tcp_destination = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::QUALITY_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *quality = v;
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string CameraRequest::to_string() const {
    std::ostringstream oss;
    oss << "CameraRequest {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (led) {
        oss << "  led: ";
        oss << (led.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  led: None\n";
    }
    if (capture_tcp_destination) {
        oss << "  capture_tcp_destination: ";
        oss << "\"" << capture_tcp_destination.value() << "\"";
        oss << "\n";
    } else {
        oss << "  capture_tcp_destination: None\n";
    }
    if (quality) {
        oss << "  quality: ";
        oss << quality.value();
        oss << "\n";
    } else {
        oss << "  quality: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes CameraReply::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (error_code) {
            cbor_encode_int(&mapEncoder, Field::ERROR_CODE_INDEX);
            cbor_encode_int(&mapEncoder, error_code.value());
            }
    if (message) {
            cbor_encode_int(&mapEncoder, Field::MESSAGE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, message.value().c_str());
            }
    if (data) {
            cbor_encode_int(&mapEncoder, Field::DATA_INDEX);
            cbor_encode_byte_string(&mapEncoder, data.value().data(), data.value().size());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

CameraReply* CameraReply::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    CameraReply* msg = new CameraReply();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::ERROR_CODE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *error_code = v;
}
                break;
            }
            
            case Field::MESSAGE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *message = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::DATA_INDEX:{{
    uint8_t tmpbuf[512];
    size_t tmplen = sizeof(tmpbuf);
    if (cbor_value_is_byte_string(&mapIt)) {
        cbor_value_copy_byte_string(&mapIt, tmpbuf, &tmplen, &mapIt);
        *data = Bytes(tmpbuf, tmpbuf + tmplen);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string CameraReply::to_string() const {
    std::ostringstream oss;
    oss << "CameraReply {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (error_code) {
        oss << "  error_code: ";
        oss << error_code.value();
        oss << "\n";
    } else {
        oss << "  error_code: None\n";
    }
    if (message) {
        oss << "  message: ";
        oss << "\"" << message.value() << "\"";
        oss << "\n";
    } else {
        oss << "  message: None\n";
    }
    if (data) {
        oss << "  data: ";
        oss << "Bytes[" << data.value().size() << "]";
        oss << "\n";
    } else {
        oss << "  data: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes LawnmowerManualEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (speed) {
            cbor_encode_int(&mapEncoder, Field::SPEED_INDEX);
            cbor_encode_int(&mapEncoder, speed.value());
            }
    if (steering) {
            cbor_encode_int(&mapEncoder, Field::STEERING_INDEX);
            cbor_encode_int(&mapEncoder, steering.value());
            }
    if (blade) {
            cbor_encode_int(&mapEncoder, Field::BLADE_INDEX);
            cbor_encode_boolean(&mapEncoder, blade.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

LawnmowerManualEvent* LawnmowerManualEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    LawnmowerManualEvent* msg = new LawnmowerManualEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::SPEED_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *speed = v;
}
                break;
            }
            
            case Field::STEERING_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *steering = v;
}
                break;
            }
            
            case Field::BLADE_INDEX:{cbor_value_get_boolean(&mapIt, &*blade);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string LawnmowerManualEvent::to_string() const {
    std::ostringstream oss;
    oss << "LawnmowerManualEvent {\n";
    if (speed) {
        oss << "  speed: ";
        oss << speed.value();
        oss << "\n";
    } else {
        oss << "  speed: None\n";
    }
    if (steering) {
        oss << "  steering: ";
        oss << steering.value();
        oss << "\n";
    } else {
        oss << "  steering: None\n";
    }
    if (blade) {
        oss << "  blade: ";
        oss << (blade.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  blade: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes LawnmowerManualRequest::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (speed) {
            cbor_encode_int(&mapEncoder, Field::SPEED_INDEX);
            cbor_encode_float(&mapEncoder, speed.value());
            }
    if (steer) {
            cbor_encode_int(&mapEncoder, Field::STEER_INDEX);
            cbor_encode_float(&mapEncoder, steer.value());
            }
    if (blade) {
            cbor_encode_int(&mapEncoder, Field::BLADE_INDEX);
            cbor_encode_boolean(&mapEncoder, blade.value());
            }
    if (start_manual_control) {
            cbor_encode_int(&mapEncoder, Field::START_MANUAL_CONTROL_INDEX);
            cbor_encode_boolean(&mapEncoder, start_manual_control.value());
            }
    if (stop_manual_control) {
            cbor_encode_int(&mapEncoder, Field::STOP_MANUAL_CONTROL_INDEX);
            cbor_encode_boolean(&mapEncoder, stop_manual_control.value());
            }
    if (emergency_stop) {
            cbor_encode_int(&mapEncoder, Field::EMERGENCY_STOP_INDEX);
            cbor_encode_boolean(&mapEncoder, emergency_stop.value());
            }
    if (start_auto_mode) {
            cbor_encode_int(&mapEncoder, Field::START_AUTO_MODE_INDEX);
            cbor_encode_boolean(&mapEncoder, start_auto_mode.value());
            }
    if (stop_auto_mode) {
            cbor_encode_int(&mapEncoder, Field::STOP_AUTO_MODE_INDEX);
            cbor_encode_boolean(&mapEncoder, stop_auto_mode.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

LawnmowerManualRequest* LawnmowerManualRequest::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    LawnmowerManualRequest* msg = new LawnmowerManualRequest();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::SPEED_INDEX:{cbor_value_get_float(&mapIt, &*speed);
                break;
            }
            
            case Field::STEER_INDEX:{cbor_value_get_float(&mapIt, &*steer);
                break;
            }
            
            case Field::BLADE_INDEX:{cbor_value_get_boolean(&mapIt, &*blade);
                break;
            }
            
            case Field::START_MANUAL_CONTROL_INDEX:{cbor_value_get_boolean(&mapIt, &*start_manual_control);
                break;
            }
            
            case Field::STOP_MANUAL_CONTROL_INDEX:{cbor_value_get_boolean(&mapIt, &*stop_manual_control);
                break;
            }
            
            case Field::EMERGENCY_STOP_INDEX:{cbor_value_get_boolean(&mapIt, &*emergency_stop);
                break;
            }
            
            case Field::START_AUTO_MODE_INDEX:{cbor_value_get_boolean(&mapIt, &*start_auto_mode);
                break;
            }
            
            case Field::STOP_AUTO_MODE_INDEX:{cbor_value_get_boolean(&mapIt, &*stop_auto_mode);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string LawnmowerManualRequest::to_string() const {
    std::ostringstream oss;
    oss << "LawnmowerManualRequest {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (speed) {
        oss << "  speed: ";
        oss << speed.value();
        oss << "\n";
    } else {
        oss << "  speed: None\n";
    }
    if (steer) {
        oss << "  steer: ";
        oss << steer.value();
        oss << "\n";
    } else {
        oss << "  steer: None\n";
    }
    if (blade) {
        oss << "  blade: ";
        oss << (blade.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  blade: None\n";
    }
    if (start_manual_control) {
        oss << "  start_manual_control: ";
        oss << (start_manual_control.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  start_manual_control: None\n";
    }
    if (stop_manual_control) {
        oss << "  stop_manual_control: ";
        oss << (stop_manual_control.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  stop_manual_control: None\n";
    }
    if (emergency_stop) {
        oss << "  emergency_stop: ";
        oss << (emergency_stop.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  emergency_stop: None\n";
    }
    if (start_auto_mode) {
        oss << "  start_auto_mode: ";
        oss << (start_auto_mode.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  start_auto_mode: None\n";
    }
    if (stop_auto_mode) {
        oss << "  stop_auto_mode: ";
        oss << (stop_auto_mode.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  stop_auto_mode: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes LawnmowerManualReply::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (error_code) {
            cbor_encode_int(&mapEncoder, Field::ERROR_CODE_INDEX);
            cbor_encode_int(&mapEncoder, error_code.value());
            }
    if (message) {
            cbor_encode_int(&mapEncoder, Field::MESSAGE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, message.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

LawnmowerManualReply* LawnmowerManualReply::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    LawnmowerManualReply* msg = new LawnmowerManualReply();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::ERROR_CODE_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *error_code = v;
}
                break;
            }
            
            case Field::MESSAGE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *message = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string LawnmowerManualReply::to_string() const {
    std::ostringstream oss;
    oss << "LawnmowerManualReply {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (error_code) {
        oss << "  error_code: ";
        oss << error_code.value();
        oss << "\n";
    } else {
        oss << "  error_code: None\n";
    }
    if (message) {
        oss << "  message: ";
        oss << "\"" << message.value() << "\"";
        oss << "\n";
    } else {
        oss << "  message: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes LawnmowerAutoEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (started) {
            cbor_encode_int(&mapEncoder, Field::STARTED_INDEX);
            cbor_encode_boolean(&mapEncoder, started.value());
            }
    if (stopped) {
            cbor_encode_int(&mapEncoder, Field::STOPPED_INDEX);
            cbor_encode_boolean(&mapEncoder, stopped.value());
            }
    if (paused) {
            cbor_encode_int(&mapEncoder, Field::PAUSED_INDEX);
            cbor_encode_boolean(&mapEncoder, paused.value());
            }
    if (resumed) {
            cbor_encode_int(&mapEncoder, Field::RESUMED_INDEX);
            cbor_encode_boolean(&mapEncoder, resumed.value());
            }
    if (mode) {
            cbor_encode_int(&mapEncoder, Field::MODE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, mode.value().c_str());
            }
    if (path) {
            cbor_encode_int(&mapEncoder, Field::PATH_INDEX);
            cbor_encode_text_stringz(&mapEncoder, path.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

LawnmowerAutoEvent* LawnmowerAutoEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    LawnmowerAutoEvent* msg = new LawnmowerAutoEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::STARTED_INDEX:{cbor_value_get_boolean(&mapIt, &*started);
                break;
            }
            
            case Field::STOPPED_INDEX:{cbor_value_get_boolean(&mapIt, &*stopped);
                break;
            }
            
            case Field::PAUSED_INDEX:{cbor_value_get_boolean(&mapIt, &*paused);
                break;
            }
            
            case Field::RESUMED_INDEX:{cbor_value_get_boolean(&mapIt, &*resumed);
                break;
            }
            
            case Field::MODE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *mode = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::PATH_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *path = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string LawnmowerAutoEvent::to_string() const {
    std::ostringstream oss;
    oss << "LawnmowerAutoEvent {\n";
    if (started) {
        oss << "  started: ";
        oss << (started.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  started: None\n";
    }
    if (stopped) {
        oss << "  stopped: ";
        oss << (stopped.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  stopped: None\n";
    }
    if (paused) {
        oss << "  paused: ";
        oss << (paused.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  paused: None\n";
    }
    if (resumed) {
        oss << "  resumed: ";
        oss << (resumed.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  resumed: None\n";
    }
    if (mode) {
        oss << "  mode: ";
        oss << "\"" << mode.value() << "\"";
        oss << "\n";
    } else {
        oss << "  mode: None\n";
    }
    if (path) {
        oss << "  path: ";
        oss << "\"" << path.value() << "\"";
        oss << "\n";
    } else {
        oss << "  path: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes LawnmowerAutoRequest::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (start) {
            cbor_encode_int(&mapEncoder, Field::START_INDEX);
            cbor_encode_boolean(&mapEncoder, start.value());
            }
    if (stop) {
            cbor_encode_int(&mapEncoder, Field::STOP_INDEX);
            cbor_encode_boolean(&mapEncoder, stop.value());
            }
    if (pause) {
            cbor_encode_int(&mapEncoder, Field::PAUSE_INDEX);
            cbor_encode_boolean(&mapEncoder, pause.value());
            }
    if (resume) {
            cbor_encode_int(&mapEncoder, Field::RESUME_INDEX);
            cbor_encode_boolean(&mapEncoder, resume.value());
            }
    if (mode) {
            cbor_encode_int(&mapEncoder, Field::MODE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, mode.value().c_str());
            }
    if (path) {
            cbor_encode_int(&mapEncoder, Field::PATH_INDEX);
            cbor_encode_text_stringz(&mapEncoder, path.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

LawnmowerAutoRequest* LawnmowerAutoRequest::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    LawnmowerAutoRequest* msg = new LawnmowerAutoRequest();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::START_INDEX:{cbor_value_get_boolean(&mapIt, &*start);
                break;
            }
            
            case Field::STOP_INDEX:{cbor_value_get_boolean(&mapIt, &*stop);
                break;
            }
            
            case Field::PAUSE_INDEX:{cbor_value_get_boolean(&mapIt, &*pause);
                break;
            }
            
            case Field::RESUME_INDEX:{cbor_value_get_boolean(&mapIt, &*resume);
                break;
            }
            
            case Field::MODE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *mode = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::PATH_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *path = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string LawnmowerAutoRequest::to_string() const {
    std::ostringstream oss;
    oss << "LawnmowerAutoRequest {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (start) {
        oss << "  start: ";
        oss << (start.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  start: None\n";
    }
    if (stop) {
        oss << "  stop: ";
        oss << (stop.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  stop: None\n";
    }
    if (pause) {
        oss << "  pause: ";
        oss << (pause.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  pause: None\n";
    }
    if (resume) {
        oss << "  resume: ";
        oss << (resume.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  resume: None\n";
    }
    if (mode) {
        oss << "  mode: ";
        oss << "\"" << mode.value() << "\"";
        oss << "\n";
    } else {
        oss << "  mode: None\n";
    }
    if (path) {
        oss << "  path: ";
        oss << "\"" << path.value() << "\"";
        oss << "\n";
    } else {
        oss << "  path: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes LawnmowerStatus::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (battery_level) {
            cbor_encode_int(&mapEncoder, Field::BATTERY_LEVEL_INDEX);
            cbor_encode_int(&mapEncoder, battery_level.value());
            }
    if (blade_status) {
            cbor_encode_int(&mapEncoder, Field::BLADE_STATUS_INDEX);
            cbor_encode_boolean(&mapEncoder, blade_status.value());
            }
    if (current_mode) {
            cbor_encode_int(&mapEncoder, Field::CURRENT_MODE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, current_mode.value().c_str());
            }
    if (error_message) {
            cbor_encode_int(&mapEncoder, Field::ERROR_MESSAGE_INDEX);
            cbor_encode_text_stringz(&mapEncoder, error_message.value().c_str());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

LawnmowerStatus* LawnmowerStatus::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    LawnmowerStatus* msg = new LawnmowerStatus();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::BATTERY_LEVEL_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *battery_level = v;
}
                break;
            }
            
            case Field::BLADE_STATUS_INDEX:{cbor_value_get_boolean(&mapIt, &*blade_status);
                break;
            }
            
            case Field::CURRENT_MODE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *current_mode = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            case Field::ERROR_MESSAGE_INDEX:{{
    char valbuf[256];
    size_t vallen = sizeof(valbuf);
    if (cbor_value_is_text_string(&mapIt)) {
        cbor_value_copy_text_string(&mapIt, valbuf, &vallen, &mapIt);
        *error_message = std::string(valbuf, vallen - 1);
    }
}
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string LawnmowerStatus::to_string() const {
    std::ostringstream oss;
    oss << "LawnmowerStatus {\n";
    if (battery_level) {
        oss << "  battery_level: ";
        oss << battery_level.value();
        oss << "\n";
    } else {
        oss << "  battery_level: None\n";
    }
    if (blade_status) {
        oss << "  blade_status: ";
        oss << (blade_status.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  blade_status: None\n";
    }
    if (current_mode) {
        oss << "  current_mode: ";
        oss << "\"" << current_mode.value() << "\"";
        oss << "\n";
    } else {
        oss << "  current_mode: None\n";
    }
    if (error_message) {
        oss << "  error_message: ";
        oss << "\"" << error_message.value() << "\"";
        oss << "\n";
    } else {
        oss << "  error_message: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes MotorEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (motor_id) {
            cbor_encode_int(&mapEncoder, Field::MOTOR_ID_INDEX);
            cbor_encode_int(&mapEncoder, motor_id.value());
            }
    if (temperature) {
            cbor_encode_int(&mapEncoder, Field::TEMPERATURE_INDEX);
            cbor_encode_float(&mapEncoder, temperature.value());
            }
    if (voltage) {
            cbor_encode_int(&mapEncoder, Field::VOLTAGE_INDEX);
            cbor_encode_float(&mapEncoder, voltage.value());
            }
    if (current) {
            cbor_encode_int(&mapEncoder, Field::CURRENT_INDEX);
            cbor_encode_float(&mapEncoder, current.value());
            }
    if (speed) {
            cbor_encode_int(&mapEncoder, Field::SPEED_INDEX);
            cbor_encode_float(&mapEncoder, speed.value());
            }
    if (position) {
            cbor_encode_int(&mapEncoder, Field::POSITION_INDEX);
            cbor_encode_float(&mapEncoder, position.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

MotorEvent* MotorEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    MotorEvent* msg = new MotorEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::MOTOR_ID_INDEX:{{
    int64_t v;
    cbor_value_get_int64(&mapIt, &v);
    *motor_id = v;
}
                break;
            }
            
            case Field::TEMPERATURE_INDEX:{cbor_value_get_float(&mapIt, &*temperature);
                break;
            }
            
            case Field::VOLTAGE_INDEX:{cbor_value_get_float(&mapIt, &*voltage);
                break;
            }
            
            case Field::CURRENT_INDEX:{cbor_value_get_float(&mapIt, &*current);
                break;
            }
            
            case Field::SPEED_INDEX:{cbor_value_get_float(&mapIt, &*speed);
                break;
            }
            
            case Field::POSITION_INDEX:{cbor_value_get_float(&mapIt, &*position);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string MotorEvent::to_string() const {
    std::ostringstream oss;
    oss << "MotorEvent {\n";
    if (motor_id) {
        oss << "  motor_id: ";
        oss << motor_id.value();
        oss << "\n";
    } else {
        oss << "  motor_id: None\n";
    }
    if (temperature) {
        oss << "  temperature: ";
        oss << temperature.value();
        oss << "\n";
    } else {
        oss << "  temperature: None\n";
    }
    if (voltage) {
        oss << "  voltage: ";
        oss << voltage.value();
        oss << "\n";
    } else {
        oss << "  voltage: None\n";
    }
    if (current) {
        oss << "  current: ";
        oss << current.value();
        oss << "\n";
    } else {
        oss << "  current: None\n";
    }
    if (speed) {
        oss << "  speed: ";
        oss << speed.value();
        oss << "\n";
    } else {
        oss << "  speed: None\n";
    }
    if (position) {
        oss << "  position: ";
        oss << position.value();
        oss << "\n";
    } else {
        oss << "  position: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes Max31855Event::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (thermocouple_c) {
            cbor_encode_int(&mapEncoder, Field::THERMOCOUPLE_C_INDEX);
            cbor_encode_float(&mapEncoder, thermocouple_c.value());
            }
    if (internal_c) {
            cbor_encode_int(&mapEncoder, Field::INTERNAL_C_INDEX);
            cbor_encode_float(&mapEncoder, internal_c.value());
            }
    if (fault) {
            cbor_encode_int(&mapEncoder, Field::FAULT_INDEX);
            cbor_encode_boolean(&mapEncoder, fault.value());
            }
    if (open_circuit) {
            cbor_encode_int(&mapEncoder, Field::OPEN_CIRCUIT_INDEX);
            cbor_encode_boolean(&mapEncoder, open_circuit.value());
            }
    if (short_to_gnd) {
            cbor_encode_int(&mapEncoder, Field::SHORT_TO_GND_INDEX);
            cbor_encode_boolean(&mapEncoder, short_to_gnd.value());
            }
    if (short_to_vcc) {
            cbor_encode_int(&mapEncoder, Field::SHORT_TO_VCC_INDEX);
            cbor_encode_boolean(&mapEncoder, short_to_vcc.value());
            }
    if (timestamp_ms) {
            cbor_encode_int(&mapEncoder, Field::TIMESTAMP_MS_INDEX);
            cbor_encode_int(&mapEncoder, timestamp_ms.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

Max31855Event* Max31855Event::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    Max31855Event* msg = new Max31855Event();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::THERMOCOUPLE_C_INDEX:{cbor_value_get_float(&mapIt, &*thermocouple_c);
                break;
            }
            
            case Field::INTERNAL_C_INDEX:{cbor_value_get_float(&mapIt, &*internal_c);
                break;
            }
            
            case Field::FAULT_INDEX:{cbor_value_get_boolean(&mapIt, &*fault);
                break;
            }
            
            case Field::OPEN_CIRCUIT_INDEX:{cbor_value_get_boolean(&mapIt, &*open_circuit);
                break;
            }
            
            case Field::SHORT_TO_GND_INDEX:{cbor_value_get_boolean(&mapIt, &*short_to_gnd);
                break;
            }
            
            case Field::SHORT_TO_VCC_INDEX:{cbor_value_get_boolean(&mapIt, &*short_to_vcc);
                break;
            }
            
            case Field::TIMESTAMP_MS_INDEX:{cbor_value_get_uint64(&mapIt, &*timestamp_ms);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string Max31855Event::to_string() const {
    std::ostringstream oss;
    oss << "Max31855Event {\n";
    if (thermocouple_c) {
        oss << "  thermocouple_c: ";
        oss << thermocouple_c.value();
        oss << "\n";
    } else {
        oss << "  thermocouple_c: None\n";
    }
    if (internal_c) {
        oss << "  internal_c: ";
        oss << internal_c.value();
        oss << "\n";
    } else {
        oss << "  internal_c: None\n";
    }
    if (fault) {
        oss << "  fault: ";
        oss << (fault.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  fault: None\n";
    }
    if (open_circuit) {
        oss << "  open_circuit: ";
        oss << (open_circuit.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  open_circuit: None\n";
    }
    if (short_to_gnd) {
        oss << "  short_to_gnd: ";
        oss << (short_to_gnd.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  short_to_gnd: None\n";
    }
    if (short_to_vcc) {
        oss << "  short_to_vcc: ";
        oss << (short_to_vcc.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  short_to_vcc: None\n";
    }
    if (timestamp_ms) {
        oss << "  timestamp_ms: ";
        oss << timestamp_ms.value();
        oss << "\n";
    } else {
        oss << "  timestamp_ms: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes HeatingRequest::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (req_id) {
            cbor_encode_int(&mapEncoder, Field::REQ_ID_INDEX);
            cbor_encode_int(&mapEncoder, req_id.value());
            }
    if (setpoint_c) {
            cbor_encode_int(&mapEncoder, Field::SETPOINT_C_INDEX);
            cbor_encode_float(&mapEncoder, setpoint_c.value());
            }
    if (enabled) {
            cbor_encode_int(&mapEncoder, Field::ENABLED_INDEX);
            cbor_encode_boolean(&mapEncoder, enabled.value());
            }
    if (kp) {
            cbor_encode_int(&mapEncoder, Field::KP_INDEX);
            cbor_encode_float(&mapEncoder, kp.value());
            }
    if (ki) {
            cbor_encode_int(&mapEncoder, Field::KI_INDEX);
            cbor_encode_float(&mapEncoder, ki.value());
            }
    if (kd) {
            cbor_encode_int(&mapEncoder, Field::KD_INDEX);
            cbor_encode_float(&mapEncoder, kd.value());
            }
    if (reset_integral) {
            cbor_encode_int(&mapEncoder, Field::RESET_INTEGRAL_INDEX);
            cbor_encode_boolean(&mapEncoder, reset_integral.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

HeatingRequest* HeatingRequest::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    HeatingRequest* msg = new HeatingRequest();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::REQ_ID_INDEX:{{
    uint64_t v;
    cbor_value_get_uint64(&mapIt, &v);
    *req_id = v;
}
                break;
            }
            
            case Field::SETPOINT_C_INDEX:{cbor_value_get_float(&mapIt, &*setpoint_c);
                break;
            }
            
            case Field::ENABLED_INDEX:{cbor_value_get_boolean(&mapIt, &*enabled);
                break;
            }
            
            case Field::KP_INDEX:{cbor_value_get_float(&mapIt, &*kp);
                break;
            }
            
            case Field::KI_INDEX:{cbor_value_get_float(&mapIt, &*ki);
                break;
            }
            
            case Field::KD_INDEX:{cbor_value_get_float(&mapIt, &*kd);
                break;
            }
            
            case Field::RESET_INTEGRAL_INDEX:{cbor_value_get_boolean(&mapIt, &*reset_integral);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string HeatingRequest::to_string() const {
    std::ostringstream oss;
    oss << "HeatingRequest {\n";
    if (req_id) {
        oss << "  req_id: ";
        oss << req_id.value();
        oss << "\n";
    } else {
        oss << "  req_id: None\n";
    }
    if (setpoint_c) {
        oss << "  setpoint_c: ";
        oss << setpoint_c.value();
        oss << "\n";
    } else {
        oss << "  setpoint_c: None\n";
    }
    if (enabled) {
        oss << "  enabled: ";
        oss << (enabled.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  enabled: None\n";
    }
    if (kp) {
        oss << "  kp: ";
        oss << kp.value();
        oss << "\n";
    } else {
        oss << "  kp: None\n";
    }
    if (ki) {
        oss << "  ki: ";
        oss << ki.value();
        oss << "\n";
    } else {
        oss << "  ki: None\n";
    }
    if (kd) {
        oss << "  kd: ";
        oss << kd.value();
        oss << "\n";
    } else {
        oss << "  kd: None\n";
    }
    if (reset_integral) {
        oss << "  reset_integral: ";
        oss << (reset_integral.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  reset_integral: None\n";
    }
    oss << "}";
    return oss.str();
}


Bytes HeatingEvent::serialize() const {
    // buffer: grow if needed by changing initial size
    std::vector<uint8_t> buffer(512);
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);

    // Start top-level map
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    if (temperature_c) {
            cbor_encode_int(&mapEncoder, Field::TEMPERATURE_C_INDEX);
            cbor_encode_float(&mapEncoder, temperature_c.value());
            }
    if (setpoint_c) {
            cbor_encode_int(&mapEncoder, Field::SETPOINT_C_INDEX);
            cbor_encode_float(&mapEncoder, setpoint_c.value());
            }
    if (output_pct) {
            cbor_encode_int(&mapEncoder, Field::OUTPUT_PCT_INDEX);
            cbor_encode_float(&mapEncoder, output_pct.value());
            }
    if (enabled) {
            cbor_encode_int(&mapEncoder, Field::ENABLED_INDEX);
            cbor_encode_boolean(&mapEncoder, enabled.value());
            }
    if (heater_on) {
            cbor_encode_int(&mapEncoder, Field::HEATER_ON_INDEX);
            cbor_encode_boolean(&mapEncoder, heater_on.value());
            }
    if (fault) {
            cbor_encode_int(&mapEncoder, Field::FAULT_INDEX);
            cbor_encode_boolean(&mapEncoder, fault.value());
            }
    if (timestamp_ms) {
            cbor_encode_int(&mapEncoder, Field::TIMESTAMP_MS_INDEX);
            cbor_encode_int(&mapEncoder, timestamp_ms.value());
            }
    cbor_encoder_close_container(&encoder, &mapEncoder);
    // get used size
    size_t used = cbor_encoder_get_buffer_size(&encoder, buffer.data());
    return Bytes(buffer.begin(), buffer.begin() + used);
}

HeatingEvent* HeatingEvent::deserialize(const Bytes& bytes) {
    CborParser parser;
    CborValue it, mapIt;
    HeatingEvent* msg = new HeatingEvent();

    CborError err = cbor_parser_init(bytes.data(), bytes.size(), 0, &parser, &it);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    if (!cbor_value_is_map(&it)) {
        delete msg;
        return nullptr;
    }

    // enter map
    err = cbor_value_enter_container(&it, &mapIt);
    if (err != CborNoError) {
        delete msg;
        return nullptr;
    }

    // iterate key/value pairs
    while (!cbor_value_at_end(&mapIt)) {
        uint64_t key = 0;
        if (cbor_value_is_unsigned_integer(&mapIt)) {
            cbor_value_get_uint64(&mapIt, &key);
            cbor_value_advance(&mapIt);
        } else {
            // invalid key type
            delete msg;
            return nullptr;
        }
        switch (key) {
            
            case Field::TEMPERATURE_C_INDEX:{cbor_value_get_float(&mapIt, &*temperature_c);
                break;
            }
            
            case Field::SETPOINT_C_INDEX:{cbor_value_get_float(&mapIt, &*setpoint_c);
                break;
            }
            
            case Field::OUTPUT_PCT_INDEX:{cbor_value_get_float(&mapIt, &*output_pct);
                break;
            }
            
            case Field::ENABLED_INDEX:{cbor_value_get_boolean(&mapIt, &*enabled);
                break;
            }
            
            case Field::HEATER_ON_INDEX:{cbor_value_get_boolean(&mapIt, &*heater_on);
                break;
            }
            
            case Field::FAULT_INDEX:{cbor_value_get_boolean(&mapIt, &*fault);
                break;
            }
            
            case Field::TIMESTAMP_MS_INDEX:{cbor_value_get_uint64(&mapIt, &*timestamp_ms);
                break;
            }
            
            default:
                // skip unknown key
                cbor_value_advance(&mapIt);
                break;
        }

    }

    // leave container
    cbor_value_leave_container(&it, &mapIt);

    return msg;
}

std::string HeatingEvent::to_string() const {
    std::ostringstream oss;
    oss << "HeatingEvent {\n";
    if (temperature_c) {
        oss << "  temperature_c: ";
        oss << temperature_c.value();
        oss << "\n";
    } else {
        oss << "  temperature_c: None\n";
    }
    if (setpoint_c) {
        oss << "  setpoint_c: ";
        oss << setpoint_c.value();
        oss << "\n";
    } else {
        oss << "  setpoint_c: None\n";
    }
    if (output_pct) {
        oss << "  output_pct: ";
        oss << output_pct.value();
        oss << "\n";
    } else {
        oss << "  output_pct: None\n";
    }
    if (enabled) {
        oss << "  enabled: ";
        oss << (enabled.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  enabled: None\n";
    }
    if (heater_on) {
        oss << "  heater_on: ";
        oss << (heater_on.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  heater_on: None\n";
    }
    if (fault) {
        oss << "  fault: ";
        oss << (fault.value() ? "true" : "false");
        oss << "\n";
    } else {
        oss << "  fault: None\n";
    }
    if (timestamp_ms) {
        oss << "  timestamp_ms: ";
        oss << timestamp_ms.value();
        oss << "\n";
    } else {
        oss << "  timestamp_ms: None\n";
    }
    oss << "}";
    return oss.str();
}

