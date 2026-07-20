// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `Option<T>`; missing fields are omitted from encoded CBOR.
// Messages are encoded as CBOR maps keyed by field id.
// Uses TinyCBOR for CBOR encoding/decoding.

#pragma once

#include <cbor.h>
#include <errno.h>
#include <msg.h>



// ── TinyCBOR helper ────────────────────────────────────────────────────────

#define cbor_check(X) { \
    CborError err = X; \
    if (err != CborNoError) { \
        WARN("failed cbor : %s",#X); \
         return EINVAL; \
    }   \
    }


// ── Encode macro ───────────────────────────────────────────────────────────// ── Decode macros ──────────────────────────────────────────────────────────// ── Messages ───────────────────────────────────────────────────────────────



class BrokerSubscribeRequest : public Msg {
public:
    static uint32_t msg_id() { return FNV("BrokerSubscribeRequest"); }
    static const char* msg_name() { return "BrokerSubscribeRequest"; }

    typedef enum FieldId {
        SRC = 0,
        MSG_TYPE = 1,
    } FieldId;
    Option<uint32_t> src;
    Option<uint32_t> msg_type;

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        src.for_each([&](const auto&) { fieldCount++; });
        msg_type.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        src.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SRC));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        msg_type.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MSG_TYPE));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a BrokerSubscribeRequest from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case BrokerSubscribeRequest::FieldId::SRC:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    src = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    src = ((uint32_t)val);
                }
                    break;
                case BrokerSubscribeRequest::FieldId::MSG_TYPE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    msg_type = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    msg_type = ((uint32_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class CompassEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("CompassEvent"); }
    static const char* msg_name() { return "CompassEvent"; }

    typedef enum FieldId {
        HEADING = 0,
        PITCH = 1,
        ROLL = 2,
        MAG_X = 3,
        MAG_Y = 4,
        MAG_Z = 5,
        ACCEL_X = 6,
        ACCEL_Y = 7,
        ACCEL_Z = 8,
    } FieldId;
    Option<float> heading;// Heading in degrees
    Option<float> pitch;// Pitch in degrees
    Option<float> roll;// Roll in degrees
    Option<float> mag_x;// Magnetometer X axis in uT
    Option<float> mag_y;// Magnetometer Y axis in uT
    Option<float> mag_z;// Magnetometer Z axis in uT
    Option<float> accel_x;// Accelerometer X axis in m/s^2
    Option<float> accel_y;// Accelerometer Y axis in m/s^2
    Option<float> accel_z;// Accelerometer Z axis in m/s^2

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        heading.for_each([&](const auto&) { fieldCount++; });
        pitch.for_each([&](const auto&) { fieldCount++; });
        roll.for_each([&](const auto&) { fieldCount++; });
        mag_x.for_each([&](const auto&) { fieldCount++; });
        mag_y.for_each([&](const auto&) { fieldCount++; });
        mag_z.for_each([&](const auto&) { fieldCount++; });
        accel_x.for_each([&](const auto&) { fieldCount++; });
        accel_y.for_each([&](const auto&) { fieldCount++; });
        accel_z.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        heading.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::HEADING));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        pitch.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::PITCH));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        roll.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ROLL));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        mag_x.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MAG_X));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        mag_y.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MAG_Y));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        mag_z.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MAG_Z));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        accel_x.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_X));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        accel_y.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_Y));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        accel_z.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_Z));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a CompassEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case CompassEvent::FieldId::HEADING:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    heading = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    heading = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    heading = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::PITCH:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    pitch = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    pitch = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    pitch = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::ROLL:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    roll = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    roll = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    roll = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::MAG_X:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    mag_x = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    mag_x = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    mag_x = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::MAG_Y:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    mag_y = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    mag_y = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    mag_y = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::MAG_Z:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    mag_z = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    mag_z = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    mag_z = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::ACCEL_X:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    accel_x = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_x = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_x = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::ACCEL_Y:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    accel_y = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_y = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_y = ((float)val);
                }
                    break;
                case CompassEvent::FieldId::ACCEL_Z:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    accel_z = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_z = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_z = ((float)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class DeviceAliveEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("DeviceAliveEvent"); }
    static const char* msg_name() { return "DeviceAliveEvent"; }

    typedef enum FieldId {
        DEVICE = 0,
        ENDPOINT = 1,
        TIMESTAMP = 2,
    } FieldId;
    Option<std::string> device;
    Option<std::string> endpoint;
    Option<uint64_t> timestamp;// Timestamp in milliseconds since epoch

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        device.for_each([&](const auto&) { fieldCount++; });
        endpoint.for_each([&](const auto&) { fieldCount++; });
        timestamp.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        device.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::DEVICE));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        endpoint.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ENDPOINT));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        timestamp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TIMESTAMP));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a DeviceAliveEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case DeviceAliveEvent::FieldId::DEVICE:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    device = (val);
                }
                    break;
                case DeviceAliveEvent::FieldId::ENDPOINT:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    endpoint = (val);
                }
                    break;
                case DeviceAliveEvent::FieldId::TIMESTAMP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    timestamp = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    timestamp = ((uint64_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class EndpointAnnounce : public Msg {
public:
    static uint32_t msg_id() { return FNV("EndpointAnnounce"); }
    static const char* msg_name() { return "EndpointAnnounce"; }

    typedef enum FieldId {
        ID = 0,
        NAME = 1,
        DESCRIPTION = 6,
        SERVICES = 2,
        EVENTS = 3,
        REPLIES = 4,
        SUBSCRIBES = 5,
    } FieldId;
    Option<uint32_t> id;// Unique identifier for the announcing endpoint
    Option<std::string> name;// Name of the announcing endpoint
    Option<std::string> description;// Description of the announcing endpoint
    Option<std::vector<uint32_t>> services;// List of services provided by the endpoint
    Option<std::vector<uint32_t>> events;// List of events emitted by the endpoint
    Option<std::vector<uint32_t>> replies;// List of replies supported by the endpoint
    Option<std::vector<uint32_t>> subscribes;// List of subscriptions for the endpoint

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        id.for_each([&](const auto&) { fieldCount++; });
        name.for_each([&](const auto&) { fieldCount++; });
        description.for_each([&](const auto&) { fieldCount++; });
        services.for_each([&](const auto&) { fieldCount++; });
        events.for_each([&](const auto&) { fieldCount++; });
        replies.for_each([&](const auto&) { fieldCount++; });
        subscribes.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        name.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::NAME));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        description.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::DESCRIPTION));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        services.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SERVICES));
            {
            CborEncoder arrEncoder;
            cbor_check(cbor_encoder_create_array(&mapEncoder, &arrEncoder, value.size()));
            for (const auto& item : value) {
                cbor_check(cbor_encode_uint(&arrEncoder, item));
            }
            cbor_check(cbor_encoder_close_container(&mapEncoder, &arrEncoder));
        }
        });
        events.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::EVENTS));
            {
            CborEncoder arrEncoder;
            cbor_check(cbor_encoder_create_array(&mapEncoder, &arrEncoder, value.size()));
            for (const auto& item : value) {
                cbor_check(cbor_encode_uint(&arrEncoder, item));
            }
            cbor_check(cbor_encoder_close_container(&mapEncoder, &arrEncoder));
        }
        });
        replies.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REPLIES));
            {
            CborEncoder arrEncoder;
            cbor_check(cbor_encoder_create_array(&mapEncoder, &arrEncoder, value.size()));
            for (const auto& item : value) {
                cbor_check(cbor_encode_uint(&arrEncoder, item));
            }
            cbor_check(cbor_encoder_close_container(&mapEncoder, &arrEncoder));
        }
        });
        subscribes.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SUBSCRIBES));
            {
            CborEncoder arrEncoder;
            cbor_check(cbor_encoder_create_array(&mapEncoder, &arrEncoder, value.size()));
            for (const auto& item : value) {
                cbor_check(cbor_encode_uint(&arrEncoder, item));
            }
            cbor_check(cbor_encoder_close_container(&mapEncoder, &arrEncoder));
        }
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a EndpointAnnounce from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case EndpointAnnounce::FieldId::ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    id = ((uint32_t)val);
                }
                    break;
                case EndpointAnnounce::FieldId::NAME:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    name = (val);
                }
                    break;
                case EndpointAnnounce::FieldId::DESCRIPTION:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    description = (val);
                }
                    break;
                case EndpointAnnounce::FieldId::SERVICES:
                    if (cbor_value_is_array(&mapValue)) {
                    CborValue arrValue;
                    cbor_value_enter_container(&mapValue, &arrValue);
                    std::vector<uint32_t> val;
                    while (!cbor_value_at_end(&arrValue)) {
                        if (cbor_value_is_unsigned_integer(&arrValue)) {
                            uint64_t v;
                            cbor_value_get_uint64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        } else if (cbor_value_is_negative_integer(&arrValue)) {
                            int64_t v;
                            cbor_value_get_int64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        }
                        cbor_value_advance(&arrValue);
                    }
                    cbor_value_leave_container(&mapValue, &arrValue);
                    services = (val);
                }
                    break;
                case EndpointAnnounce::FieldId::EVENTS:
                    if (cbor_value_is_array(&mapValue)) {
                    CborValue arrValue;
                    cbor_value_enter_container(&mapValue, &arrValue);
                    std::vector<uint32_t> val;
                    while (!cbor_value_at_end(&arrValue)) {
                        if (cbor_value_is_unsigned_integer(&arrValue)) {
                            uint64_t v;
                            cbor_value_get_uint64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        } else if (cbor_value_is_negative_integer(&arrValue)) {
                            int64_t v;
                            cbor_value_get_int64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        }
                        cbor_value_advance(&arrValue);
                    }
                    cbor_value_leave_container(&mapValue, &arrValue);
                    events = (val);
                }
                    break;
                case EndpointAnnounce::FieldId::REPLIES:
                    if (cbor_value_is_array(&mapValue)) {
                    CborValue arrValue;
                    cbor_value_enter_container(&mapValue, &arrValue);
                    std::vector<uint32_t> val;
                    while (!cbor_value_at_end(&arrValue)) {
                        if (cbor_value_is_unsigned_integer(&arrValue)) {
                            uint64_t v;
                            cbor_value_get_uint64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        } else if (cbor_value_is_negative_integer(&arrValue)) {
                            int64_t v;
                            cbor_value_get_int64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        }
                        cbor_value_advance(&arrValue);
                    }
                    cbor_value_leave_container(&mapValue, &arrValue);
                    replies = (val);
                }
                    break;
                case EndpointAnnounce::FieldId::SUBSCRIBES:
                    if (cbor_value_is_array(&mapValue)) {
                    CborValue arrValue;
                    cbor_value_enter_container(&mapValue, &arrValue);
                    std::vector<uint32_t> val;
                    while (!cbor_value_at_end(&arrValue)) {
                        if (cbor_value_is_unsigned_integer(&arrValue)) {
                            uint64_t v;
                            cbor_value_get_uint64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        } else if (cbor_value_is_negative_integer(&arrValue)) {
                            int64_t v;
                            cbor_value_get_int64(&arrValue, &v);
                            val.push_back((uint32_t)v);
                        }
                        cbor_value_advance(&arrValue);
                    }
                    cbor_value_leave_container(&mapValue, &arrValue);
                    subscribes = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class EndpointAnnounceReply : public Msg {
public:
    static uint32_t msg_id() { return FNV("EndpointAnnounceReply"); }
    static const char* msg_name() { return "EndpointAnnounceReply"; }

    typedef enum FieldId {
        UTC = 0,
    } FieldId;
    Option<uint64_t> utc;// Timestamp in milliseconds since epoch

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        utc.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        utc.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::UTC));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a EndpointAnnounceReply from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case EndpointAnnounceReply::FieldId::UTC:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    utc = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    utc = ((uint64_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class Envelope : public Msg {
public:
    static uint32_t msg_id() { return FNV("Envelope"); }
    static const char* msg_name() { return "Envelope"; }

    typedef enum FieldId {
        SRC = 0,
        DST = 1,
        MSG_TYPE = 2,
        REQUEST_ID = 3,
        INSTANCE_ID = 4,
        PAYLOAD = 5,
    } FieldId;
    Option<uint32_t> src;// Source endpoint name
    Option<uint32_t> dst;// Destination endpoint name
    Option<uint32_t> msg_type;// Message type name
    Option<uint32_t> request_id;// Request ID for matching request/reply
    Option<uint32_t> instance_id;// Instance ID for matching request/reply
    Option<std::vector<uint8_t>> payload;// Serialized payload of the message

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        src.for_each([&](const auto&) { fieldCount++; });
        dst.for_each([&](const auto&) { fieldCount++; });
        msg_type.for_each([&](const auto&) { fieldCount++; });
        request_id.for_each([&](const auto&) { fieldCount++; });
        instance_id.for_each([&](const auto&) { fieldCount++; });
        payload.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        src.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SRC));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        dst.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::DST));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        msg_type.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MSG_TYPE));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        request_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQUEST_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        instance_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INSTANCE_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        payload.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::PAYLOAD));
            cbor_check(cbor_encode_byte_string(&mapEncoder, value.data(), value.size()));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a Envelope from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case Envelope::FieldId::SRC:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    src = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    src = ((uint32_t)val);
                }
                    break;
                case Envelope::FieldId::DST:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    dst = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    dst = ((uint32_t)val);
                }
                    break;
                case Envelope::FieldId::MSG_TYPE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    msg_type = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    msg_type = ((uint32_t)val);
                }
                    break;
                case Envelope::FieldId::REQUEST_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    request_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    request_id = ((uint32_t)val);
                }
                    break;
                case Envelope::FieldId::INSTANCE_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    instance_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    instance_id = ((uint32_t)val);
                }
                    break;
                case Envelope::FieldId::PAYLOAD:
                    if (cbor_value_is_byte_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::vector<uint8_t> val(len);
                    cbor_value_copy_byte_string(&mapValue, val.data(), &len, NULL);
                    payload = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class GenericReply : public Msg {
public:
    static uint32_t msg_id() { return FNV("GenericReply"); }
    static const char* msg_name() { return "GenericReply"; }

    typedef enum FieldId {
        REQ_ID = 0,
        ERROR_CODE = 1,
        MESSAGE = 2,
        MSG_TYPE = 3,
    } FieldId;
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<uint32_t> error_code;// Error code, 0 if no error
    Option<std::string> message;// Error message or additional information
    Option<uint32_t> msg_type;// Message type identifier , the original request

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        req_id.for_each([&](const auto&) { fieldCount++; });
        error_code.for_each([&](const auto&) { fieldCount++; });
        message.for_each([&](const auto&) { fieldCount++; });
        msg_type.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        req_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQ_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        error_code.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ERROR_CODE));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        message.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MESSAGE));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        msg_type.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MSG_TYPE));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a GenericReply from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case GenericReply::FieldId::REQ_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                }
                    break;
                case GenericReply::FieldId::ERROR_CODE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    error_code = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    error_code = ((uint32_t)val);
                }
                    break;
                case GenericReply::FieldId::MESSAGE:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    message = (val);
                }
                    break;
                case GenericReply::FieldId::MSG_TYPE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    msg_type = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    msg_type = ((uint32_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class HeatingEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("HeatingEvent"); }
    static const char* msg_name() { return "HeatingEvent"; }

    typedef enum FieldId {
        TEMPERATURE_C = 0,
        SETPOINT_C = 1,
        ENABLED = 2,
        OUTPUT_PCT = 3,
        HEATER_ON = 4,
        FAULT = 5,
        TIMESTAMP_MS = 6,
    } FieldId;
    Option<float> temperature_c;// Current temperature in Celsius
    Option<float> setpoint_c;// Setpoint temperature in Celsius
    Option<bool> enabled;// Heating enabled or disabled
    Option<float> output_pct;// Output percentage of the heating element
    Option<bool> heater_on;// Heater is currently on or off
    Option<bool> fault;// Fault detected in the heating system
    Option<uint64_t> timestamp_ms;// Timestamp in milliseconds since epoch

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        temperature_c.for_each([&](const auto&) { fieldCount++; });
        setpoint_c.for_each([&](const auto&) { fieldCount++; });
        enabled.for_each([&](const auto&) { fieldCount++; });
        output_pct.for_each([&](const auto&) { fieldCount++; });
        heater_on.for_each([&](const auto&) { fieldCount++; });
        fault.for_each([&](const auto&) { fieldCount++; });
        timestamp_ms.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        temperature_c.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TEMPERATURE_C));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        setpoint_c.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SETPOINT_C));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        enabled.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ENABLED));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        output_pct.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::OUTPUT_PCT));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        heater_on.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::HEATER_ON));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        fault.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FAULT));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        timestamp_ms.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TIMESTAMP_MS));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a HeatingEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case HeatingEvent::FieldId::TEMPERATURE_C:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    temperature_c = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    temperature_c = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    temperature_c = ((float)val);
                }
                    break;
                case HeatingEvent::FieldId::SETPOINT_C:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    setpoint_c = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    setpoint_c = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    setpoint_c = ((float)val);
                }
                    break;
                case HeatingEvent::FieldId::ENABLED:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    enabled = (val);
                }
                    break;
                case HeatingEvent::FieldId::OUTPUT_PCT:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    output_pct = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    output_pct = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    output_pct = ((float)val);
                }
                    break;
                case HeatingEvent::FieldId::HEATER_ON:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    heater_on = (val);
                }
                    break;
                case HeatingEvent::FieldId::FAULT:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    fault = (val);
                }
                    break;
                case HeatingEvent::FieldId::TIMESTAMP_MS:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    timestamp_ms = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    timestamp_ms = ((uint64_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class HeatingRequest : public Msg {
public:
    static uint32_t msg_id() { return FNV("HeatingRequest"); }
    static const char* msg_name() { return "HeatingRequest"; }

    typedef enum FieldId {
        SETPOINT_C = 0,
        ENABLED = 1,
        KP = 2,
        KI = 3,
        KD = 4,
        RESET_INTEGRAL = 5,
    } FieldId;
    Option<float> setpoint_c;// Setpoint temperature in Celsius
    Option<bool> enabled;// Enable or disable heating
    Option<float> kp;// Proportional gain for PID controller
    Option<float> ki;// Integral gain for PID controller
    Option<float> kd;// Derivative gain for PID controller
    Option<bool> reset_integral;// Reset the integral term of the PID controller

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        setpoint_c.for_each([&](const auto&) { fieldCount++; });
        enabled.for_each([&](const auto&) { fieldCount++; });
        kp.for_each([&](const auto&) { fieldCount++; });
        ki.for_each([&](const auto&) { fieldCount++; });
        kd.for_each([&](const auto&) { fieldCount++; });
        reset_integral.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        setpoint_c.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SETPOINT_C));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        enabled.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ENABLED));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        kp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::KP));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        ki.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::KI));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        kd.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::KD));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        reset_integral.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::RESET_INTEGRAL));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a HeatingRequest from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case HeatingRequest::FieldId::SETPOINT_C:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    setpoint_c = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    setpoint_c = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    setpoint_c = ((float)val);
                }
                    break;
                case HeatingRequest::FieldId::ENABLED:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    enabled = (val);
                }
                    break;
                case HeatingRequest::FieldId::KP:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    kp = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    kp = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    kp = ((float)val);
                }
                    break;
                case HeatingRequest::FieldId::KI:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    ki = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    ki = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    ki = ((float)val);
                }
                    break;
                case HeatingRequest::FieldId::KD:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    kd = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    kd = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    kd = ((float)val);
                }
                    break;
                case HeatingRequest::FieldId::RESET_INTEGRAL:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    reset_integral = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class HoverboardEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("HoverboardEvent"); }
    static const char* msg_name() { return "HoverboardEvent"; }

    typedef enum FieldId {
        CTRL_MOD = 0,
        CTRL_TYP = 1,
        CUR_MOT_MAX = 2,
        RPM_MOT_MAX = 3,
        FI_WEAK_ENA = 4,
        FI_WEAK_HI = 5,
        FI_WEAK_LO = 6,
        FI_WEAK_MAX = 7,
        PHASE_ADV_MAX_DEG = 8,
        INPUT1_RAW = 9,
        INPUT1_TYP = 10,
        INPUT1_MIN = 11,
        INPUT1_MID = 12,
        INPUT1_MAX = 13,
        INPUT1_CMD = 14,
        INPUT2_RAW = 15,
        INPUT2_TYP = 16,
        INPUT2_MIN = 17,
        INPUT2_MID = 18,
        INPUT2_MAX = 19,
        INPUT2_CMD = 20,
        AUX_INPUT1_RAW = 21,
        AUX_INPUT1_TYP = 22,
        AUX_INPUT1_MIN = 23,
        AUX_INPUT1_MID = 24,
        AUX_INPUT1_MAX = 25,
        AUX_INPUT1_CMD = 26,
        AUX_INPUT2_RAW = 27,
        AUX_INPUT2_TYP = 28,
        AUX_INPUT2_MIN = 29,
        AUX_INPUT2_MID = 30,
        AUX_INPUT2_MAX = 31,
        AUX_INPUT2_CMD = 32,
        DC_CURR = 33,
        RDC_CURR = 34,
        LDC_CURR = 35,
        CMDL = 36,
        CMDR = 37,
        SPD_AVG = 38,
        SPDL = 39,
        SPDR = 40,
        FILTER_RATE = 41,
        SPD_COEF = 42,
        STR_COEF = 43,
        BATV = 44,
        TEMP = 45,
    } FieldId;
    Option<int32_t> ctrl_mod;// 1:Voltage 2:Speed 3:Torque
    Option<int32_t> ctrl_typ;// 0:Commutation 1:Sinusoidal 2:FOC
    Option<int32_t> cur_mot_max;// Max phase current A
    Option<int32_t> rpm_mot_max;// Max motor RPM
    Option<int32_t> fi_weak_ena;// Enable field weak 0:OFF 1:ON
    Option<int32_t> fi_weak_hi;// Field weak high RPM
    Option<int32_t> fi_weak_lo;// Field weak low RPM
    Option<int32_t> fi_weak_max;// Field weak max current A (FOC only)
    Option<int32_t> phase_adv_max_deg;// Max Phase Adv angle Deg (SIN only)
    Option<int32_t> input1_raw;// Input1 raw value
    Option<int32_t> input1_typ;// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    Option<int32_t> input1_min;// Input1 minimum value
    Option<int32_t> input1_mid;// Input1 middle value
    Option<int32_t> input1_max;// Input1 maximum value
    Option<int32_t> input1_cmd;// Input1 command value
    Option<int32_t> input2_raw;// Input2 raw value
    Option<int32_t> input2_typ;// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    Option<int32_t> input2_min;// Input2 minimum value
    Option<int32_t> input2_mid;// Input2 middle value
    Option<int32_t> input2_max;// Input2 maximum value
    Option<int32_t> input2_cmd;// Input2 command value
    Option<int32_t> aux_input1_raw;// Input1 raw value
    Option<int32_t> aux_input1_typ;// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    Option<int32_t> aux_input1_min;// Input1 minimum value
    Option<int32_t> aux_input1_mid;// Input1 middle value
    Option<int32_t> aux_input1_max;// Input1 maximum value
    Option<int32_t> aux_input1_cmd;// Input1 command value
    Option<int32_t> aux_input2_raw;// Input2 raw value
    Option<int32_t> aux_input2_typ;// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    Option<int32_t> aux_input2_min;// Input2 minimum value
    Option<int32_t> aux_input2_mid;// Input2 middle value
    Option<int32_t> aux_input2_max;// Input2 maximum value
    Option<int32_t> aux_input2_cmd;// Input2 command value
    Option<int32_t> dc_curr;// Total DC Link current A *100
    Option<int32_t> rdc_curr;// Right DC Link current A *100
    Option<int32_t> ldc_curr;// Left DC Link current A *100
    Option<int32_t> cmdl;// Left Motor Command RPM
    Option<int32_t> cmdr;// Right Motor Command RPM
    Option<int32_t> spd_avg;// Motor Measured Avg RPM
    Option<int32_t> spdl;// Left Motor Measured RPM
    Option<int32_t> spdr;// Right Motor Measured RPM
    Option<int32_t> filter_rate;// Rate *10
    Option<int32_t> spd_coef;// Speed Coefficient *10
    Option<int32_t> str_coef;// Steer Coefficient *10
    Option<int32_t> batv;// Calibrated Battery Voltage *100
    Option<int32_t> temp;// Calibrated Temperature C *10

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        ctrl_mod.for_each([&](const auto&) { fieldCount++; });
        ctrl_typ.for_each([&](const auto&) { fieldCount++; });
        cur_mot_max.for_each([&](const auto&) { fieldCount++; });
        rpm_mot_max.for_each([&](const auto&) { fieldCount++; });
        fi_weak_ena.for_each([&](const auto&) { fieldCount++; });
        fi_weak_hi.for_each([&](const auto&) { fieldCount++; });
        fi_weak_lo.for_each([&](const auto&) { fieldCount++; });
        fi_weak_max.for_each([&](const auto&) { fieldCount++; });
        phase_adv_max_deg.for_each([&](const auto&) { fieldCount++; });
        input1_raw.for_each([&](const auto&) { fieldCount++; });
        input1_typ.for_each([&](const auto&) { fieldCount++; });
        input1_min.for_each([&](const auto&) { fieldCount++; });
        input1_mid.for_each([&](const auto&) { fieldCount++; });
        input1_max.for_each([&](const auto&) { fieldCount++; });
        input1_cmd.for_each([&](const auto&) { fieldCount++; });
        input2_raw.for_each([&](const auto&) { fieldCount++; });
        input2_typ.for_each([&](const auto&) { fieldCount++; });
        input2_min.for_each([&](const auto&) { fieldCount++; });
        input2_mid.for_each([&](const auto&) { fieldCount++; });
        input2_max.for_each([&](const auto&) { fieldCount++; });
        input2_cmd.for_each([&](const auto&) { fieldCount++; });
        aux_input1_raw.for_each([&](const auto&) { fieldCount++; });
        aux_input1_typ.for_each([&](const auto&) { fieldCount++; });
        aux_input1_min.for_each([&](const auto&) { fieldCount++; });
        aux_input1_mid.for_each([&](const auto&) { fieldCount++; });
        aux_input1_max.for_each([&](const auto&) { fieldCount++; });
        aux_input1_cmd.for_each([&](const auto&) { fieldCount++; });
        aux_input2_raw.for_each([&](const auto&) { fieldCount++; });
        aux_input2_typ.for_each([&](const auto&) { fieldCount++; });
        aux_input2_min.for_each([&](const auto&) { fieldCount++; });
        aux_input2_mid.for_each([&](const auto&) { fieldCount++; });
        aux_input2_max.for_each([&](const auto&) { fieldCount++; });
        aux_input2_cmd.for_each([&](const auto&) { fieldCount++; });
        dc_curr.for_each([&](const auto&) { fieldCount++; });
        rdc_curr.for_each([&](const auto&) { fieldCount++; });
        ldc_curr.for_each([&](const auto&) { fieldCount++; });
        cmdl.for_each([&](const auto&) { fieldCount++; });
        cmdr.for_each([&](const auto&) { fieldCount++; });
        spd_avg.for_each([&](const auto&) { fieldCount++; });
        spdl.for_each([&](const auto&) { fieldCount++; });
        spdr.for_each([&](const auto&) { fieldCount++; });
        filter_rate.for_each([&](const auto&) { fieldCount++; });
        spd_coef.for_each([&](const auto&) { fieldCount++; });
        str_coef.for_each([&](const auto&) { fieldCount++; });
        batv.for_each([&](const auto&) { fieldCount++; });
        temp.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        ctrl_mod.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CTRL_MOD));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        ctrl_typ.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CTRL_TYP));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        cur_mot_max.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CUR_MOT_MAX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        rpm_mot_max.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::RPM_MOT_MAX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        fi_weak_ena.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FI_WEAK_ENA));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        fi_weak_hi.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FI_WEAK_HI));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        fi_weak_lo.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FI_WEAK_LO));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        fi_weak_max.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FI_WEAK_MAX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        phase_adv_max_deg.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::PHASE_ADV_MAX_DEG));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input1_raw.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT1_RAW));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input1_typ.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT1_TYP));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input1_min.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT1_MIN));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input1_mid.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT1_MID));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input1_max.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT1_MAX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input1_cmd.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT1_CMD));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input2_raw.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT2_RAW));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input2_typ.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT2_TYP));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input2_min.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT2_MIN));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input2_mid.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT2_MID));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input2_max.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT2_MAX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        input2_cmd.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INPUT2_CMD));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input1_raw.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT1_RAW));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input1_typ.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT1_TYP));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input1_min.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT1_MIN));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input1_mid.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT1_MID));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input1_max.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT1_MAX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input1_cmd.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT1_CMD));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input2_raw.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT2_RAW));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input2_typ.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT2_TYP));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input2_min.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT2_MIN));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input2_mid.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT2_MID));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input2_max.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT2_MAX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        aux_input2_cmd.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AUX_INPUT2_CMD));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        dc_curr.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::DC_CURR));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        rdc_curr.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::RDC_CURR));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        ldc_curr.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::LDC_CURR));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        cmdl.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CMDL));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        cmdr.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CMDR));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        spd_avg.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SPD_AVG));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        spdl.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SPDL));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        spdr.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SPDR));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        filter_rate.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FILTER_RATE));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        spd_coef.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SPD_COEF));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        str_coef.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::STR_COEF));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        batv.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BATV));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        temp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TEMP));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a HoverboardEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case HoverboardEvent::FieldId::CTRL_MOD:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    ctrl_mod = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    ctrl_mod = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::CTRL_TYP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    ctrl_typ = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    ctrl_typ = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::CUR_MOT_MAX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    cur_mot_max = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    cur_mot_max = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::RPM_MOT_MAX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    rpm_mot_max = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    rpm_mot_max = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::FI_WEAK_ENA:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    fi_weak_ena = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    fi_weak_ena = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::FI_WEAK_HI:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    fi_weak_hi = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    fi_weak_hi = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::FI_WEAK_LO:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    fi_weak_lo = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    fi_weak_lo = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::FI_WEAK_MAX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    fi_weak_max = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    fi_weak_max = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::PHASE_ADV_MAX_DEG:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    phase_adv_max_deg = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    phase_adv_max_deg = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT1_RAW:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input1_raw = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input1_raw = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT1_TYP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input1_typ = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input1_typ = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT1_MIN:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input1_min = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input1_min = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT1_MID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input1_mid = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input1_mid = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT1_MAX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input1_max = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input1_max = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT1_CMD:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input1_cmd = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input1_cmd = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT2_RAW:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input2_raw = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input2_raw = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT2_TYP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input2_typ = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input2_typ = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT2_MIN:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input2_min = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input2_min = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT2_MID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input2_mid = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input2_mid = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT2_MAX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input2_max = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input2_max = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::INPUT2_CMD:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    input2_cmd = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    input2_cmd = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT1_RAW:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input1_raw = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input1_raw = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT1_TYP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input1_typ = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input1_typ = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT1_MIN:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input1_min = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input1_min = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT1_MID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input1_mid = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input1_mid = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT1_MAX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input1_max = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input1_max = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT1_CMD:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input1_cmd = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input1_cmd = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT2_RAW:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input2_raw = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input2_raw = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT2_TYP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input2_typ = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input2_typ = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT2_MIN:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input2_min = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input2_min = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT2_MID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input2_mid = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input2_mid = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT2_MAX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input2_max = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input2_max = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::AUX_INPUT2_CMD:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    aux_input2_cmd = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    aux_input2_cmd = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::DC_CURR:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    dc_curr = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    dc_curr = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::RDC_CURR:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    rdc_curr = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    rdc_curr = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::LDC_CURR:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    ldc_curr = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    ldc_curr = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::CMDL:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    cmdl = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    cmdl = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::CMDR:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    cmdr = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    cmdr = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::SPD_AVG:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    spd_avg = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    spd_avg = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::SPDL:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    spdl = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    spdl = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::SPDR:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    spdr = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    spdr = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::FILTER_RATE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    filter_rate = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    filter_rate = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::SPD_COEF:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    spd_coef = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    spd_coef = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::STR_COEF:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    str_coef = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    str_coef = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::BATV:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    batv = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    batv = ((int32_t)val);
                }
                    break;
                case HoverboardEvent::FieldId::TEMP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    temp = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    temp = ((int32_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class HoverboardRequest : public Msg {
public:
    static uint32_t msg_id() { return FNV("HoverboardRequest"); }
    static const char* msg_name() { return "HoverboardRequest"; }

    typedef enum FieldId {
        REQ_ID = 0,
        SPEED = 1,
        STEER = 2,
    } FieldId;
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<int32_t> speed;// Speed command for the hoverboard
    Option<int32_t> steer;// Steering command for the hoverboard

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        req_id.for_each([&](const auto&) { fieldCount++; });
        speed.for_each([&](const auto&) { fieldCount++; });
        steer.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        req_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQ_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        speed.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SPEED));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        steer.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::STEER));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a HoverboardRequest from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case HoverboardRequest::FieldId::REQ_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                }
                    break;
                case HoverboardRequest::FieldId::SPEED:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    speed = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    speed = ((int32_t)val);
                }
                    break;
                case HoverboardRequest::FieldId::STEER:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    steer = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    steer = ((int32_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class ImuEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("ImuEvent"); }
    static const char* msg_name() { return "ImuEvent"; }

    typedef enum FieldId {
        GYRO_X = 0,
        GYRO_Y = 1,
        GYRO_Z = 2,
        ACCEL_X = 3,
        ACCEL_Y = 4,
        ACCEL_Z = 5,
    } FieldId;
    Option<float> gyro_x;// Gyroscope X axis in deg/s
    Option<float> gyro_y;// Gyroscope Y axis in deg/s
    Option<float> gyro_z;// Gyroscope Z axis in deg/s
    Option<float> accel_x;// Accelerometer X axis in m/s^2
    Option<float> accel_y;// Accelerometer Y axis in m/s^2
    Option<float> accel_z;// Accelerometer Z axis in m/s^2

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        gyro_x.for_each([&](const auto&) { fieldCount++; });
        gyro_y.for_each([&](const auto&) { fieldCount++; });
        gyro_z.for_each([&](const auto&) { fieldCount++; });
        accel_x.for_each([&](const auto&) { fieldCount++; });
        accel_y.for_each([&](const auto&) { fieldCount++; });
        accel_z.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        gyro_x.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::GYRO_X));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        gyro_y.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::GYRO_Y));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        gyro_z.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::GYRO_Z));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        accel_x.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_X));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        accel_y.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_Y));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        accel_z.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_Z));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a ImuEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case ImuEvent::FieldId::GYRO_X:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    gyro_x = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    gyro_x = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    gyro_x = ((float)val);
                }
                    break;
                case ImuEvent::FieldId::GYRO_Y:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    gyro_y = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    gyro_y = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    gyro_y = ((float)val);
                }
                    break;
                case ImuEvent::FieldId::GYRO_Z:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    gyro_z = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    gyro_z = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    gyro_z = ((float)val);
                }
                    break;
                case ImuEvent::FieldId::ACCEL_X:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    accel_x = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_x = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_x = ((float)val);
                }
                    break;
                case ImuEvent::FieldId::ACCEL_Y:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    accel_y = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_y = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_y = ((float)val);
                }
                    break;
                case ImuEvent::FieldId::ACCEL_Z:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    accel_z = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_z = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_z = ((float)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class Max31855Event : public Msg {
public:
    static uint32_t msg_id() { return FNV("Max31855Event"); }
    static const char* msg_name() { return "Max31855Event"; }

    typedef enum FieldId {
        THERMOCOUPLE_TEMP = 0,
        INTERNAL_TEMP = 1,
        FAULT = 2,
        FAULT_SHORT_VCC = 3,
        FAULT_SHORT_GND = 4,
        FAULT_OPEN_TC = 5,
    } FieldId;
    Option<float> thermocouple_temp;// Thermocouple temperature in Celsius
    Option<float> internal_temp;// Internal temperature in Celsius
    Option<bool> fault;// Fault detected
    Option<bool> fault_short_vcc;// Short to VCC detected
    Option<bool> fault_short_gnd;// Short to GND detected
    Option<bool> fault_open_tc;// Open thermocouple detected

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        thermocouple_temp.for_each([&](const auto&) { fieldCount++; });
        internal_temp.for_each([&](const auto&) { fieldCount++; });
        fault.for_each([&](const auto&) { fieldCount++; });
        fault_short_vcc.for_each([&](const auto&) { fieldCount++; });
        fault_short_gnd.for_each([&](const auto&) { fieldCount++; });
        fault_open_tc.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        thermocouple_temp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::THERMOCOUPLE_TEMP));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        internal_temp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::INTERNAL_TEMP));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        fault.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FAULT));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        fault_short_vcc.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FAULT_SHORT_VCC));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        fault_short_gnd.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FAULT_SHORT_GND));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        fault_open_tc.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FAULT_OPEN_TC));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a Max31855Event from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case Max31855Event::FieldId::THERMOCOUPLE_TEMP:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    thermocouple_temp = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    thermocouple_temp = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    thermocouple_temp = ((float)val);
                }
                    break;
                case Max31855Event::FieldId::INTERNAL_TEMP:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    internal_temp = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    internal_temp = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    internal_temp = ((float)val);
                }
                    break;
                case Max31855Event::FieldId::FAULT:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    fault = (val);
                }
                    break;
                case Max31855Event::FieldId::FAULT_SHORT_VCC:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    fault_short_vcc = (val);
                }
                    break;
                case Max31855Event::FieldId::FAULT_SHORT_GND:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    fault_short_gnd = (val);
                }
                    break;
                case Max31855Event::FieldId::FAULT_OPEN_TC:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    fault_open_tc = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class PingReply : public Msg {
public:
    static uint32_t msg_id() { return FNV("PingReply"); }
    static const char* msg_name() { return "PingReply"; }

    typedef enum FieldId {
        REQ_ID = 0,
        TIMESTAMP = 1,
    } FieldId;
    Option<uint32_t> req_id;
    Option<uint64_t> timestamp;// Timestamp in milliseconds since epoch

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        req_id.for_each([&](const auto&) { fieldCount++; });
        timestamp.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        req_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQ_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        timestamp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TIMESTAMP));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a PingReply from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case PingReply::FieldId::REQ_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                }
                    break;
                case PingReply::FieldId::TIMESTAMP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    timestamp = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    timestamp = ((uint64_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class PingRequest : public Msg {
public:
    static uint32_t msg_id() { return FNV("PingRequest"); }
    static const char* msg_name() { return "PingRequest"; }

    typedef enum FieldId {
        REQ_ID = 0,
        TIMESTAMP = 1,
    } FieldId;
    Option<uint32_t> req_id;
    Option<uint64_t> timestamp;// Timestamp in milliseconds since epoch

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        req_id.for_each([&](const auto&) { fieldCount++; });
        timestamp.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        req_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQ_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        timestamp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TIMESTAMP));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a PingRequest from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case PingRequest::FieldId::REQ_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                }
                    break;
                case PingRequest::FieldId::TIMESTAMP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    timestamp = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    timestamp = ((uint64_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class Ps4Event : public Msg {
public:
    static uint32_t msg_id() { return FNV("Ps4Event"); }
    static const char* msg_name() { return "Ps4Event"; }

    typedef enum FieldId {
        BUTTON_LEFT = 0,
        BUTTON_RIGHT = 1,
        BUTTON_UP = 2,
        BUTTON_DOWN = 3,
        BUTTON_SQUARE = 4,
        BUTTON_CROSS = 5,
        BUTTON_CIRCLE = 6,
        BUTTON_TRIANGLE = 7,
        BUTTON_LEFT_SHOULDER = 8,
        BUTTON_RIGHT_SHOULDER = 9,
        BUTTON_LEFT_TRIGGER = 10,
        BUTTON_RIGHT_TRIGGER = 11,
        BUTTON_LEFT_JOYSTICK = 12,
        BUTTON_RIGHT_JOYSTICK = 13,
        BUTTON_SHARE = 14,
        BUTTON_OPTIONS = 15,
        BUTTON_TOUCHPAD = 16,
        BUTTON_PS = 17,
        AXIS_LX = 18,
        AXIS_LY = 19,
        AXIS_RX = 20,
        AXIS_RY = 21,
        GYRO_X = 22,
        GYRO_Y = 23,
        GYRO_Z = 24,
        ACCEL_X = 25,
        ACCEL_Y = 26,
        ACCEL_Z = 27,
        CONNECTED = 28,
        BATTERY_LEVEL = 29,
        BLUETOOTH = 30,
        DEBUG = 31,
        TEMP = 32,
    } FieldId;
    Option<bool> button_left;
    Option<bool> button_right;
    Option<bool> button_up;
    Option<bool> button_down;
    Option<bool> button_square;
    Option<bool> button_cross;
    Option<bool> button_circle;
    Option<bool> button_triangle;
    Option<bool> button_left_shoulder;
    Option<bool> button_right_shoulder;
    Option<bool> button_left_trigger;
    Option<bool> button_right_trigger;
    Option<bool> button_left_joystick;
    Option<bool> button_right_joystick;
    Option<bool> button_share;
    Option<bool> button_options;
    Option<bool> button_touchpad;
    Option<bool> button_ps;
    Option<int32_t> axis_lx;
    Option<int32_t> axis_ly;
    Option<int32_t> axis_rx;
    Option<int32_t> axis_ry;
    Option<int32_t> gyro_x;
    Option<int32_t> gyro_y;
    Option<int32_t> gyro_z;
    Option<int32_t> accel_x;
    Option<int32_t> accel_y;
    Option<int32_t> accel_z;
    Option<bool> connected;
    Option<int32_t> battery_level;
    Option<bool> bluetooth;
    Option<std::string> debug;
    Option<int32_t> temp;

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        button_left.for_each([&](const auto&) { fieldCount++; });
        button_right.for_each([&](const auto&) { fieldCount++; });
        button_up.for_each([&](const auto&) { fieldCount++; });
        button_down.for_each([&](const auto&) { fieldCount++; });
        button_square.for_each([&](const auto&) { fieldCount++; });
        button_cross.for_each([&](const auto&) { fieldCount++; });
        button_circle.for_each([&](const auto&) { fieldCount++; });
        button_triangle.for_each([&](const auto&) { fieldCount++; });
        button_left_shoulder.for_each([&](const auto&) { fieldCount++; });
        button_right_shoulder.for_each([&](const auto&) { fieldCount++; });
        button_left_trigger.for_each([&](const auto&) { fieldCount++; });
        button_right_trigger.for_each([&](const auto&) { fieldCount++; });
        button_left_joystick.for_each([&](const auto&) { fieldCount++; });
        button_right_joystick.for_each([&](const auto&) { fieldCount++; });
        button_share.for_each([&](const auto&) { fieldCount++; });
        button_options.for_each([&](const auto&) { fieldCount++; });
        button_touchpad.for_each([&](const auto&) { fieldCount++; });
        button_ps.for_each([&](const auto&) { fieldCount++; });
        axis_lx.for_each([&](const auto&) { fieldCount++; });
        axis_ly.for_each([&](const auto&) { fieldCount++; });
        axis_rx.for_each([&](const auto&) { fieldCount++; });
        axis_ry.for_each([&](const auto&) { fieldCount++; });
        gyro_x.for_each([&](const auto&) { fieldCount++; });
        gyro_y.for_each([&](const auto&) { fieldCount++; });
        gyro_z.for_each([&](const auto&) { fieldCount++; });
        accel_x.for_each([&](const auto&) { fieldCount++; });
        accel_y.for_each([&](const auto&) { fieldCount++; });
        accel_z.for_each([&](const auto&) { fieldCount++; });
        connected.for_each([&](const auto&) { fieldCount++; });
        battery_level.for_each([&](const auto&) { fieldCount++; });
        bluetooth.for_each([&](const auto&) { fieldCount++; });
        debug.for_each([&](const auto&) { fieldCount++; });
        temp.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        button_left.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_LEFT));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_right.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_RIGHT));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_up.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_UP));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_down.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_DOWN));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_square.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_SQUARE));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_cross.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_CROSS));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_circle.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_CIRCLE));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_triangle.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_TRIANGLE));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_left_shoulder.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_LEFT_SHOULDER));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_right_shoulder.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_RIGHT_SHOULDER));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_left_trigger.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_LEFT_TRIGGER));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_right_trigger.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_RIGHT_TRIGGER));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_left_joystick.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_LEFT_JOYSTICK));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_right_joystick.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_RIGHT_JOYSTICK));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_share.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_SHARE));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_options.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_OPTIONS));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_touchpad.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_TOUCHPAD));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        button_ps.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUTTON_PS));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        axis_lx.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AXIS_LX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        axis_ly.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AXIS_LY));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        axis_rx.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AXIS_RX));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        axis_ry.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::AXIS_RY));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        gyro_x.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::GYRO_X));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        gyro_y.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::GYRO_Y));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        gyro_z.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::GYRO_Z));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        accel_x.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_X));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        accel_y.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_Y));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        accel_z.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::ACCEL_Z));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        connected.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CONNECTED));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        battery_level.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BATTERY_LEVEL));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        bluetooth.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BLUETOOTH));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        debug.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::DEBUG));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        temp.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TEMP));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a Ps4Event from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case Ps4Event::FieldId::BUTTON_LEFT:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_left = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_RIGHT:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_right = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_UP:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_up = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_DOWN:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_down = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_SQUARE:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_square = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_CROSS:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_cross = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_CIRCLE:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_circle = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_TRIANGLE:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_triangle = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_LEFT_SHOULDER:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_left_shoulder = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_RIGHT_SHOULDER:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_right_shoulder = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_LEFT_TRIGGER:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_left_trigger = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_RIGHT_TRIGGER:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_right_trigger = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_LEFT_JOYSTICK:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_left_joystick = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_RIGHT_JOYSTICK:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_right_joystick = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_SHARE:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_share = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_OPTIONS:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_options = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_TOUCHPAD:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_touchpad = (val);
                }
                    break;
                case Ps4Event::FieldId::BUTTON_PS:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    button_ps = (val);
                }
                    break;
                case Ps4Event::FieldId::AXIS_LX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    axis_lx = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    axis_lx = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::AXIS_LY:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    axis_ly = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    axis_ly = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::AXIS_RX:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    axis_rx = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    axis_rx = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::AXIS_RY:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    axis_ry = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    axis_ry = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::GYRO_X:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    gyro_x = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    gyro_x = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::GYRO_Y:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    gyro_y = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    gyro_y = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::GYRO_Z:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    gyro_z = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    gyro_z = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::ACCEL_X:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_x = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_x = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::ACCEL_Y:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_y = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_y = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::ACCEL_Z:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    accel_z = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    accel_z = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::CONNECTED:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    connected = (val);
                }
                    break;
                case Ps4Event::FieldId::BATTERY_LEVEL:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    battery_level = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    battery_level = ((int32_t)val);
                }
                    break;
                case Ps4Event::FieldId::BLUETOOTH:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    bluetooth = (val);
                }
                    break;
                case Ps4Event::FieldId::DEBUG:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    debug = (val);
                }
                    break;
                case Ps4Event::FieldId::TEMP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    temp = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    temp = ((int32_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class Ps4Request : public Msg {
public:
    static uint32_t msg_id() { return FNV("Ps4Request"); }
    static const char* msg_name() { return "Ps4Request"; }

    typedef enum FieldId {
        REQ_ID = 0,
        RUMBLE_SMALL = 1,
        RUMBLE_LARGE = 2,
        LED_RED = 3,
        LED_GREEN = 4,
        LED_BLUE = 5,
        LED_FLASH_ON = 6,
        LED_FLASH_OFF = 7,
    } FieldId;
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<int32_t> rumble_small;
    Option<int32_t> rumble_large;
    Option<int32_t> led_red;
    Option<int32_t> led_green;
    Option<int32_t> led_blue;
    Option<int32_t> led_flash_on;
    Option<int32_t> led_flash_off;

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        req_id.for_each([&](const auto&) { fieldCount++; });
        rumble_small.for_each([&](const auto&) { fieldCount++; });
        rumble_large.for_each([&](const auto&) { fieldCount++; });
        led_red.for_each([&](const auto&) { fieldCount++; });
        led_green.for_each([&](const auto&) { fieldCount++; });
        led_blue.for_each([&](const auto&) { fieldCount++; });
        led_flash_on.for_each([&](const auto&) { fieldCount++; });
        led_flash_off.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        req_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQ_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        rumble_small.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::RUMBLE_SMALL));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        rumble_large.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::RUMBLE_LARGE));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        led_red.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::LED_RED));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        led_green.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::LED_GREEN));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        led_blue.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::LED_BLUE));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        led_flash_on.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::LED_FLASH_ON));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        led_flash_off.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::LED_FLASH_OFF));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a Ps4Request from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case Ps4Request::FieldId::REQ_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                }
                    break;
                case Ps4Request::FieldId::RUMBLE_SMALL:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    rumble_small = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    rumble_small = ((int32_t)val);
                }
                    break;
                case Ps4Request::FieldId::RUMBLE_LARGE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    rumble_large = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    rumble_large = ((int32_t)val);
                }
                    break;
                case Ps4Request::FieldId::LED_RED:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    led_red = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    led_red = ((int32_t)val);
                }
                    break;
                case Ps4Request::FieldId::LED_GREEN:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    led_green = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    led_green = ((int32_t)val);
                }
                    break;
                case Ps4Request::FieldId::LED_BLUE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    led_blue = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    led_blue = ((int32_t)val);
                }
                    break;
                case Ps4Request::FieldId::LED_FLASH_ON:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    led_flash_on = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    led_flash_on = ((int32_t)val);
                }
                    break;
                case Ps4Request::FieldId::LED_FLASH_OFF:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    led_flash_off = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    led_flash_off = ((int32_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class SysEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("SysEvent"); }
    static const char* msg_name() { return "SysEvent"; }

    typedef enum FieldId {
        UTC = 0,
        UPTIME = 1,
        FREE_HEAP = 2,
        FLASH_SIZE = 3,
        CPU_BOARD_TYPE = 4,
        BUILD_DATE_TIME = 5,
    } FieldId;
    Option<uint64_t> utc;
    Option<uint64_t> uptime;
    Option<uint64_t> free_heap;
    Option<uint64_t> flash_size;
    Option<std::string> cpu_board_type;
    Option<std::string> build_date_time;

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        utc.for_each([&](const auto&) { fieldCount++; });
        uptime.for_each([&](const auto&) { fieldCount++; });
        free_heap.for_each([&](const auto&) { fieldCount++; });
        flash_size.for_each([&](const auto&) { fieldCount++; });
        cpu_board_type.for_each([&](const auto&) { fieldCount++; });
        build_date_time.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        utc.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::UTC));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        uptime.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::UPTIME));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        free_heap.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FREE_HEAP));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        flash_size.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::FLASH_SIZE));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        cpu_board_type.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CPU_BOARD_TYPE));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        build_date_time.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BUILD_DATE_TIME));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a SysEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case SysEvent::FieldId::UTC:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    utc = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    utc = ((uint64_t)val);
                }
                    break;
                case SysEvent::FieldId::UPTIME:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    uptime = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    uptime = ((uint64_t)val);
                }
                    break;
                case SysEvent::FieldId::FREE_HEAP:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    free_heap = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    free_heap = ((uint64_t)val);
                }
                    break;
                case SysEvent::FieldId::FLASH_SIZE:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    flash_size = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    flash_size = ((uint64_t)val);
                }
                    break;
                case SysEvent::FieldId::CPU_BOARD_TYPE:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    cpu_board_type = (val);
                }
                    break;
                case SysEvent::FieldId::BUILD_DATE_TIME:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    build_date_time = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class SysReply : public Msg {
public:
    static uint32_t msg_id() { return FNV("SysReply"); }
    static const char* msg_name() { return "SysReply"; }

    typedef enum FieldId {
        REQ_ID = 0,
        RC = 1,
        MESSAGE = 2,
    } FieldId;
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<int32_t> rc;
    Option<std::string> message;

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        req_id.for_each([&](const auto&) { fieldCount++; });
        rc.for_each([&](const auto&) { fieldCount++; });
        message.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        req_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQ_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        rc.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::RC));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        message.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MESSAGE));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a SysReply from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case SysReply::FieldId::REQ_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                }
                    break;
                case SysReply::FieldId::RC:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    rc = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    rc = ((int32_t)val);
                }
                    break;
                case SysReply::FieldId::MESSAGE:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    message = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class SysRequest : public Msg {
public:
    static uint32_t msg_id() { return FNV("SysRequest"); }
    static const char* msg_name() { return "SysRequest"; }

    typedef enum FieldId {
        REQ_ID = 0,
        SET_TIME = 1,
        REBOOT = 2,
        CONSOLE = 3,
    } FieldId;
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<uint64_t> set_time;
    Option<bool> reboot;
    Option<std::string> console;

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        req_id.for_each([&](const auto&) { fieldCount++; });
        set_time.for_each([&](const auto&) { fieldCount++; });
        reboot.for_each([&](const auto&) { fieldCount++; });
        console.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        req_id.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REQ_ID));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        set_time.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SET_TIME));
            cbor_check(cbor_encode_uint(&mapEncoder, value));
        });
        reboot.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::REBOOT));
            cbor_check(cbor_encode_boolean(&mapEncoder, value));
        });
        console.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CONSOLE));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a SysRequest from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case SysRequest::FieldId::REQ_ID:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    req_id = ((uint32_t)val);
                }
                    break;
                case SysRequest::FieldId::SET_TIME:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    set_time = (val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    set_time = ((uint64_t)val);
                }
                    break;
                case SysRequest::FieldId::REBOOT:
                    if (cbor_value_is_boolean(&mapValue)) {
                    bool val;
                    cbor_value_get_boolean(&mapValue, &val);
                    reboot = (val);
                }
                    break;
                case SysRequest::FieldId::CONSOLE:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    console = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class UsEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("UsEvent"); }
    static const char* msg_name() { return "UsEvent"; }

    typedef enum FieldId {
        DISTANCE = 0,
        TEMPERATURE = 1,
        STATUS = 2,
    } FieldId;
    Option<float> distance;// Distance in meters
    Option<float> temperature;// Temperature in Celsius
    Option<int32_t> status;// Status code, 0 if no error

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        distance.for_each([&](const auto&) { fieldCount++; });
        temperature.for_each([&](const auto&) { fieldCount++; });
        status.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        distance.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::DISTANCE));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        temperature.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::TEMPERATURE));
            cbor_check(cbor_encode_float(&mapEncoder, value));
        });
        status.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::STATUS));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a UsEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case UsEvent::FieldId::DISTANCE:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    distance = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    distance = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    distance = ((float)val);
                }
                    break;
                case UsEvent::FieldId::TEMPERATURE:
                    if (cbor_value_is_float(&mapValue) || cbor_value_is_double(&mapValue)) {
                    float val;
                    cbor_value_get_float(&mapValue, &val);
                    temperature = (val);
                } else if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    temperature = ((float)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    temperature = ((float)val);
                }
                    break;
                case UsEvent::FieldId::STATUS:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    status = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    status = ((int32_t)val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};



class WifiEvent : public Msg {
public:
    static uint32_t msg_id() { return FNV("WifiEvent"); }
    static const char* msg_name() { return "WifiEvent"; }

    typedef enum FieldId {
        IP = 0,
        GATEWAY = 1,
        NETMASK = 2,
        SSID = 3,
        BSSID = 4,
        CHANNEL = 5,
        RSSI = 6,
        MAC = 7,
    } FieldId;
    Option<std::string> ip;
    Option<std::string> gateway;
    Option<std::string> netmask;
    Option<std::string> ssid;
    Option<std::string> bssid;
    Option<int32_t> channel;
    Option<int32_t> rssi;
    Option<std::string> mac;

    /// Serialize this message into a CBOR map keyed by field id.
    /// Writes into the parent encoder as a single map item.
    int encode(Bytes& buffer)  {
        CborEncoder encoder;
        cbor_encoder_init(&encoder,buffer.data(),buffer.capacity(),0);
        // Count how many optional fields are set.
        uint32_t fieldCount = 0;
        ip.for_each([&](const auto&) { fieldCount++; });
        gateway.for_each([&](const auto&) { fieldCount++; });
        netmask.for_each([&](const auto&) { fieldCount++; });
        ssid.for_each([&](const auto&) { fieldCount++; });
        bssid.for_each([&](const auto&) { fieldCount++; });
        channel.for_each([&](const auto&) { fieldCount++; });
        rssi.for_each([&](const auto&) { fieldCount++; });
        mac.for_each([&](const auto&) { fieldCount++; });

        CborEncoder mapEncoder;
        cbor_check(cbor_encoder_create_map(&encoder, &mapEncoder, fieldCount));
        ip.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::IP));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        gateway.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::GATEWAY));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        netmask.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::NETMASK));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        ssid.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::SSID));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        bssid.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::BSSID));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });
        channel.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::CHANNEL));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        rssi.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::RSSI));
            cbor_check(cbor_encode_int(&mapEncoder, value));
        });
        mac.for_each([&](const auto& value) {
            cbor_check(cbor_encode_uint(&mapEncoder, FieldId::MAC));
            cbor_check(cbor_encode_text_string(&mapEncoder, value.c_str(), value.length()));
        });

         cbor_check(cbor_encoder_close_container(&encoder, &mapEncoder));
         return 0;
    }

    /// Deserialize a WifiEvent from a CBOR map value.
    /// `it` must point to the map; after decoding it is advanced past the map.
     int decode(const Bytes& buffer) {
        CborParser parser;
        CborValue it;
        cbor_check(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &it));
        if (!cbor_value_is_map(&it)) {
            WARN("Expected CBOR map ");
            return EINVAL;
        }

        CborValue mapValue;
        cbor_value_enter_container(&it, &mapValue);

        while (!cbor_value_at_end(&mapValue)) {
            // Read the map key (must be an unsigned integer — field id).
            if (!cbor_value_is_unsigned_integer(&mapValue)) {
                // Skip unknown key type and its value.
                cbor_value_advance(&mapValue);  // skip key
                if (!cbor_value_at_end(&mapValue)) {
                    cbor_value_advance(&mapValue);  // skip value
                }
                continue;
            }

            uint64_t keyVal;
            cbor_value_get_uint64(&mapValue, &keyVal);
            cbor_value_advance(&mapValue);  // advance to value

            switch ((uint32_t)keyVal) {
                case WifiEvent::FieldId::IP:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    ip = (val);
                }
                    break;
                case WifiEvent::FieldId::GATEWAY:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    gateway = (val);
                }
                    break;
                case WifiEvent::FieldId::NETMASK:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    netmask = (val);
                }
                    break;
                case WifiEvent::FieldId::SSID:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    ssid = (val);
                }
                    break;
                case WifiEvent::FieldId::BSSID:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    bssid = (val);
                }
                    break;
                case WifiEvent::FieldId::CHANNEL:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    channel = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    channel = ((int32_t)val);
                }
                    break;
                case WifiEvent::FieldId::RSSI:
                    if (cbor_value_is_unsigned_integer(&mapValue)) {
                    uint64_t val;
                    cbor_value_get_uint64(&mapValue, &val);
                    rssi = ((int32_t)val);
                } else if (cbor_value_is_negative_integer(&mapValue)) {
                    int64_t val;
                    cbor_value_get_int64(&mapValue, &val);
                    rssi = ((int32_t)val);
                }
                    break;
                case WifiEvent::FieldId::MAC:
                    if (cbor_value_is_text_string(&mapValue)) {
                    size_t len;
                    cbor_value_get_string_length(&mapValue, &len);
                    std::string val(len, '\0');
                    cbor_value_copy_text_string(&mapValue, &val[0], &len, NULL);
                    val.resize(len);
                    mac = (val);
                }
                    break;
                default:
                    // Unknown field id — skip value.
                    break;
            }

            cbor_value_advance(&mapValue);  // advance past value to next key (or end)
        }

        cbor_value_leave_container(&it, &mapValue);
        return 0;
    }
};


