// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `std::optional<T>`; missing fields serialize as CBOR null.
// Messages are encoded as CBOR maps keyed by field id.
// Uses tinycbor for CBOR encoding/decoding.

#pragma once

#include <cbor.h>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

 namespace msgs_detail {

template <typename>
inline constexpr bool always_false_v = false;

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

inline CborError encodeValue(CborEncoder& encoder, uint32_t value) {
    return cbor_encode_uint(&encoder, static_cast<uint64_t>(value));
}

inline CborError encodeValue(CborEncoder& encoder, int32_t value) {
    return cbor_encode_int(&encoder, static_cast<int64_t>(value));
}

inline CborError encodeValue(CborEncoder& encoder, uint64_t value) {
    return cbor_encode_uint(&encoder, value);
}

inline CborError encodeValue(CborEncoder& encoder, int64_t value) {
    return cbor_encode_int(&encoder, value);
}

inline CborError encodeValue(CborEncoder& encoder, bool value) {
    return cbor_encode_boolean(&encoder, value);
}

inline CborError encodeValue(CborEncoder& encoder, float value) {
    return cbor_encode_float(&encoder, value);
}

inline CborError encodeValue(CborEncoder& encoder, const std::string& value) {
    return cbor_encode_text_string(&encoder, value.c_str(), value.size());
}

template <typename T>
inline CborError encodeValue(CborEncoder& encoder, const std::vector<T>& value) {
    CborEncoder arrayEncoder;
    CborError err = cbor_encoder_create_array(
        &encoder, &arrayEncoder, static_cast<size_t>(value.size()));
    if (err != CborNoError) {
        return err;
    }

    for (const auto& element : value) {
        if constexpr (std::is_same_v<T, bool>) {
            err = encodeValue(arrayEncoder, static_cast<bool>(element));
        } else {
            err = encodeValue(arrayEncoder, element);
        }
        if (err != CborNoError) {
            return err;
        }
    }

    return cbor_encoder_close_container(&encoder, &arrayEncoder);
}

template <typename T>
inline CborError encodeOptional(CborEncoder& encoder,
                                const std::optional<T>& value) {
    if (!value.has_value()) {
        return cbor_encode_null(&encoder);
    }

    if constexpr (std::is_same_v<T, uint32_t> ||
                  std::is_same_v<T, int32_t> ||
                  std::is_same_v<T, uint64_t> ||
                  std::is_same_v<T, int64_t> ||
                  std::is_same_v<T, bool> ||
                  std::is_same_v<T, float> ||
                  std::is_same_v<T, std::string> ||
                  is_vector<T>::value) {
        return encodeValue(encoder, value.value());
    } else {
        static_assert(always_false_v<T>, "Unsupported encodeOptional<T> type");
    }
}

inline CborError encodeOptionalUInt32(CborEncoder& encoder,
                                      const std::optional<uint32_t>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalUInt32(CborValue& value,
                                 std::optional<uint32_t>& out) {
    if (!cbor_value_is_null(&value)) {
        uint64_t v;
        cbor_value_get_uint64(&value, &v);
        out = static_cast<uint32_t>(v);
    }
}

inline CborError encodeOptionalInt32(CborEncoder& encoder,
                                     const std::optional<int32_t>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalInt32(CborValue& value,
                                std::optional<int32_t>& out) {
    if (!cbor_value_is_null(&value)) {
        int64_t v;
        cbor_value_get_int64(&value, &v);
        out = static_cast<int32_t>(v);
    }
}

inline CborError encodeOptionalUInt64(CborEncoder& encoder,
                                      const std::optional<uint64_t>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalUInt64(CborValue& value,
                                 std::optional<uint64_t>& out) {
    if (!cbor_value_is_null(&value)) {
        cbor_value_get_uint64(&value, &out.emplace());
    }
}

inline CborError encodeOptionalInt64(CborEncoder& encoder,
                                     const std::optional<int64_t>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalInt64(CborValue& value,
                                std::optional<int64_t>& out) {
    if (!cbor_value_is_null(&value)) {
        cbor_value_get_int64(&value, &out.emplace());
    }
}

inline CborError encodeOptionalBool(CborEncoder& encoder,
                                    const std::optional<bool>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalBool(CborValue& value,
                               std::optional<bool>& out) {
    if (!cbor_value_is_null(&value)) {
        bool v;
        cbor_value_get_boolean(&value, &v);
        out = v;
    }
}

inline CborError encodeOptionalFloat(CborEncoder& encoder,
                                     const std::optional<float>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalFloat(CborValue& value,
                                std::optional<float>& out) {
    if (!cbor_value_is_null(&value)) {
        cbor_value_get_float(&value, &out.emplace());
    }
}

inline CborError encodeOptionalString(CborEncoder& encoder,
                                      const std::optional<std::string>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalString(CborValue& value,
                                 std::optional<std::string>& out) {
    if (!cbor_value_is_null(&value)) {
        char* buf = nullptr;
        size_t len = 0;
        cbor_value_dup_text_string(&value, &buf, &len, nullptr);
        out = std::string(buf, len);
        free(buf);
    }
}

template <typename T>
inline bool decodeScalar(CborValue& value, T& out) {
    if constexpr (std::is_same_v<T, uint32_t>) {
        if (!cbor_value_is_integer(&value)) return false;
        uint64_t v;
        if (cbor_value_get_uint64(&value, &v) != CborNoError) return false;
        out = static_cast<uint32_t>(v);
        return true;
    } else if constexpr (std::is_same_v<T, int32_t>) {
        if (!cbor_value_is_integer(&value)) return false;
        int64_t v;
        if (cbor_value_get_int64(&value, &v) != CborNoError) return false;
        out = static_cast<int32_t>(v);
        return true;
    } else if constexpr (std::is_same_v<T, uint64_t>) {
        if (!cbor_value_is_integer(&value)) return false;
        return cbor_value_get_uint64(&value, &out) == CborNoError;
    } else if constexpr (std::is_same_v<T, int64_t>) {
        if (!cbor_value_is_integer(&value)) return false;
        return cbor_value_get_int64(&value, &out) == CborNoError;
    } else if constexpr (std::is_same_v<T, bool>) {
        if (!cbor_value_is_boolean(&value)) return false;
        return cbor_value_get_boolean(&value, &out) == CborNoError;
    } else if constexpr (std::is_same_v<T, float>) {
        if (!cbor_value_is_float(&value)) return false;
        return cbor_value_get_float(&value, &out) == CborNoError;
    } else if constexpr (std::is_same_v<T, std::string>) {
        if (!cbor_value_is_text_string(&value)) return false;
        char* buf = nullptr;
        size_t len = 0;
        if (cbor_value_dup_text_string(&value, &buf, &len, nullptr) != CborNoError) {
            return false;
        }
        out = std::string(buf, len);
        free(buf);
        return true;
    } else {
        static_assert(always_false_v<T>, "Unsupported decodeScalar<T> type");
    }
}

template <typename T>
inline void decodeOptionalVector(CborValue& value,
                                 std::optional<std::vector<T>>& out) {
    if (cbor_value_is_null(&value) || !cbor_value_is_array(&value)) {
        return;
    }

    CborValue array;
    if (cbor_value_enter_container(&value, &array) != CborNoError) {
        return;
    }

    std::vector<T> decoded;
    while (!cbor_value_at_end(&array)) {
        T item{};
        if (decodeScalar(array, item)) {
            decoded.push_back(std::move(item));
        }

        if (!cbor_value_at_end(&array)) {
            cbor_value_advance(&array);
        }
    }

    cbor_value_leave_container(&value, &array);
    out = std::move(decoded);
}

inline CborError encodeOptionalUInt32Array(
    CborEncoder& encoder,
    const std::optional<std::vector<uint32_t>>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalUInt32Array(
    CborValue& value,
    std::optional<std::vector<uint32_t>>& out) {
    decodeOptionalVector(value, out);
}

inline CborError encodeOptionalInt32Array(
    CborEncoder& encoder,
    const std::optional<std::vector<int32_t>>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalInt32Array(
    CborValue& value,
    std::optional<std::vector<int32_t>>& out) {
    decodeOptionalVector(value, out);
}

inline CborError encodeOptionalUInt64Array(
    CborEncoder& encoder,
    const std::optional<std::vector<uint64_t>>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalUInt64Array(
    CborValue& value,
    std::optional<std::vector<uint64_t>>& out) {
    decodeOptionalVector(value, out);
}

inline CborError encodeOptionalInt64Array(
    CborEncoder& encoder,
    const std::optional<std::vector<int64_t>>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalInt64Array(
    CborValue& value,
    std::optional<std::vector<int64_t>>& out) {
    decodeOptionalVector(value, out);
}

inline CborError encodeOptionalFloatArray(
    CborEncoder& encoder,
    const std::optional<std::vector<float>>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalFloatArray(
    CborValue& value,
    std::optional<std::vector<float>>& out) {
    decodeOptionalVector(value, out);
}

inline CborError encodeOptionalBoolArray(
    CborEncoder& encoder,
    const std::optional<std::vector<bool>>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalBoolArray(
    CborValue& value,
    std::optional<std::vector<bool>>& out) {
    decodeOptionalVector(value, out);
}

inline CborError encodeOptionalStringArray(
    CborEncoder& encoder,
    const std::optional<std::vector<std::string>>& value) {
    return encodeOptional(encoder, value);
}

inline void decodeOptionalStringArray(
    CborValue& value,
    std::optional<std::vector<std::string>>& out) {
    decodeOptionalVector(value, out);
}

} // namespace msgs_detail






struct BrokerSubscribeRequest {
    std::optional<uint32_t> src;
    std::optional<uint32_t> msg_type;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (src.has_value()) {
            ++mapSize;
        }
        if (msg_type.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (src.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, src);
        }
        if (err != CborNoError) return err;
        if (msg_type.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, msg_type);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a BrokerSubscribeRequest from a CBOR map value.
    static BrokerSubscribeRequest fromCbor(CborValue& it) {
        BrokerSubscribeRequest msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.src);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalUInt32(map, msg.msg_type);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct CompassEvent {
    /// Heading in degrees
    std::optional<float> heading;
    /// Pitch in degrees
    std::optional<float> pitch;
    /// Roll in degrees
    std::optional<float> roll;
    /// Magnetometer X axis in uT
    std::optional<float> mag_x;
    /// Magnetometer Y axis in uT
    std::optional<float> mag_y;
    /// Magnetometer Z axis in uT
    std::optional<float> mag_z;
    /// Accelerometer X axis in m/s^2
    std::optional<float> accel_x;
    /// Accelerometer Y axis in m/s^2
    std::optional<float> accel_y;
    /// Accelerometer Z axis in m/s^2
    std::optional<float> accel_z;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (heading.has_value()) {
            ++mapSize;
        }
        if (pitch.has_value()) {
            ++mapSize;
        }
        if (roll.has_value()) {
            ++mapSize;
        }
        if (mag_x.has_value()) {
            ++mapSize;
        }
        if (mag_y.has_value()) {
            ++mapSize;
        }
        if (mag_z.has_value()) {
            ++mapSize;
        }
        if (accel_x.has_value()) {
            ++mapSize;
        }
        if (accel_y.has_value()) {
            ++mapSize;
        }
        if (accel_z.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (heading.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, heading);
        }
        if (err != CborNoError) return err;
        if (pitch.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, pitch);
        }
        if (err != CborNoError) return err;
        if (roll.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, roll);
        }
        if (err != CborNoError) return err;
        if (mag_x.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, mag_x);
        }
        if (err != CborNoError) return err;
        if (mag_y.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, mag_y);
        }
        if (err != CborNoError) return err;
        if (mag_z.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, mag_z);
        }
        if (err != CborNoError) return err;
        if (accel_x.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(6));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, accel_x);
        }
        if (err != CborNoError) return err;
        if (accel_y.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(7));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, accel_y);
        }
        if (err != CborNoError) return err;
        if (accel_z.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(8));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, accel_z);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a CompassEvent from a CBOR map value.
    static CompassEvent fromCbor(CborValue& it) {
        CompassEvent msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalFloat(map, msg.heading);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalFloat(map, msg.pitch);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalFloat(map, msg.roll);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalFloat(map, msg.mag_x);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalFloat(map, msg.mag_y);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalFloat(map, msg.mag_z);
                        break;
                    case 6:

                    msgs_detail::decodeOptionalFloat(map, msg.accel_x);
                        break;
                    case 7:

                    msgs_detail::decodeOptionalFloat(map, msg.accel_y);
                        break;
                    case 8:

                    msgs_detail::decodeOptionalFloat(map, msg.accel_z);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct DeviceAliveEvent {
    std::optional<std::string> device;
    std::optional<std::string> endpoint;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (device.has_value()) {
            ++mapSize;
        }
        if (endpoint.has_value()) {
            ++mapSize;
        }
        if (timestamp.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (device.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, device);
        }
        if (err != CborNoError) return err;
        if (endpoint.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, endpoint);
        }
        if (err != CborNoError) return err;
        if (timestamp.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, timestamp);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a DeviceAliveEvent from a CBOR map value.
    static DeviceAliveEvent fromCbor(CborValue& it) {
        DeviceAliveEvent msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalString(map, msg.device);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalString(map, msg.endpoint);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalUInt64(map, msg.timestamp);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct EndpointAnnounce {
    /// Unique identifier for the announcing endpoint
    std::optional<uint32_t> id;
    /// Name of the announcing endpoint
    std::optional<std::string> name;
    /// List of services provided by the endpoint
    std::optional<std::vector<uint32_t>> services;
    /// List of events emitted by the endpoint
    std::optional<std::vector<uint32_t>> events;
    /// List of replies supported by the endpoint
    std::optional<std::vector<uint32_t>> replies;
    /// List of subscriptions for the endpoint
    std::optional<std::vector<uint32_t>> subscribes;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (id.has_value()) {
            ++mapSize;
        }
        if (name.has_value()) {
            ++mapSize;
        }
        if (services.has_value()) {
            ++mapSize;
        }
        if (events.has_value()) {
            ++mapSize;
        }
        if (replies.has_value()) {
            ++mapSize;
        }
        if (subscribes.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, id);
        }
        if (err != CborNoError) return err;
        if (name.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, name);
        }
        if (err != CborNoError) return err;
        if (services.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32Array(mapEncoder, services);
        }
        if (err != CborNoError) return err;
        if (events.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32Array(mapEncoder, events);
        }
        if (err != CborNoError) return err;
        if (replies.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32Array(mapEncoder, replies);
        }
        if (err != CborNoError) return err;
        if (subscribes.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32Array(mapEncoder, subscribes);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a EndpointAnnounce from a CBOR map value.
    static EndpointAnnounce fromCbor(CborValue& it) {
        EndpointAnnounce msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalString(map, msg.name);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalUInt32Array(map, msg.services);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalUInt32Array(map, msg.events);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalUInt32Array(map, msg.replies);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalUInt32Array(map, msg.subscribes);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct EndpointAnnounceReply {

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a EndpointAnnounceReply from a CBOR map value.
    static EndpointAnnounceReply fromCbor(CborValue& it) {
        EndpointAnnounceReply msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct Envelope {
    /// Source endpoint name
    std::optional<std::string> src;
    /// Destination endpoint name
    std::optional<std::string> dst;
    /// Message type name
    std::optional<std::string> msg_type;
    /// Request ID for matching request/reply
    std::optional<uint32_t> request_id;
    /// Instance ID for matching request/reply
    std::optional<uint32_t> instance_id;
    /// Serialized payload of the message
    std::optional<std::vector<uint8_t>> payload;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (src.has_value()) {
            ++mapSize;
        }
        if (dst.has_value()) {
            ++mapSize;
        }
        if (msg_type.has_value()) {
            ++mapSize;
        }
        if (request_id.has_value()) {
            ++mapSize;
        }
        if (instance_id.has_value()) {
            ++mapSize;
        }
        if (payload.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (src.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, src);
        }
        if (err != CborNoError) return err;
        if (dst.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, dst);
        }
        if (err != CborNoError) return err;
        if (msg_type.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, msg_type);
        }
        if (err != CborNoError) return err;
        if (request_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, request_id);
        }
        if (err != CborNoError) return err;
        if (instance_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, instance_id);
        }
        if (err != CborNoError) return err;
        if (payload.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a Envelope from a CBOR map value.
    static Envelope fromCbor(CborValue& it) {
        Envelope msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalString(map, msg.src);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalString(map, msg.dst);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalString(map, msg.msg_type);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalUInt32(map, msg.request_id);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalUInt32(map, msg.instance_id);
                        break;
                    case 5:

                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct GenericReply {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    /// Error code, 0 if no error
    std::optional<uint32_t> error_code;
    /// Error message or additional information
    std::optional<std::string> message;
    /// Message type identifier , the original request
    std::optional<uint32_t> msg_type;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (req_id.has_value()) {
            ++mapSize;
        }
        if (error_code.has_value()) {
            ++mapSize;
        }
        if (message.has_value()) {
            ++mapSize;
        }
        if (msg_type.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, req_id);
        }
        if (err != CborNoError) return err;
        if (error_code.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, error_code);
        }
        if (err != CborNoError) return err;
        if (message.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, message);
        }
        if (err != CborNoError) return err;
        if (msg_type.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, msg_type);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a GenericReply from a CBOR map value.
    static GenericReply fromCbor(CborValue& it) {
        GenericReply msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalUInt32(map, msg.error_code);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalString(map, msg.message);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalUInt32(map, msg.msg_type);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct HeatingEvent {
    /// Current temperature in Celsius
    std::optional<float> temperature;
    /// Setpoint temperature in Celsius
    std::optional<float> setpoint;
    /// Heating status
    std::optional<bool> heating;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (temperature.has_value()) {
            ++mapSize;
        }
        if (setpoint.has_value()) {
            ++mapSize;
        }
        if (heating.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (temperature.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, temperature);
        }
        if (err != CborNoError) return err;
        if (setpoint.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, setpoint);
        }
        if (err != CborNoError) return err;
        if (heating.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, heating);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a HeatingEvent from a CBOR map value.
    static HeatingEvent fromCbor(CborValue& it) {
        HeatingEvent msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalFloat(map, msg.temperature);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalFloat(map, msg.setpoint);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalBool(map, msg.heating);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct HoverboardEvent {
    /// 1:Voltage 2:Speed 3:Torque
    std::optional<int32_t> ctrl_mod;
    /// 0:Commutation 1:Sinusoidal 2:FOC
    std::optional<int32_t> ctrl_typ;
    /// Max phase current A
    std::optional<int32_t> cur_mot_max;
    /// Max motor RPM
    std::optional<int32_t> rpm_mot_max;
    /// Enable field weak 0:OFF 1:ON
    std::optional<int32_t> fi_weak_ena;
    /// Field weak high RPM
    std::optional<int32_t> fi_weak_hi;
    /// Field weak low RPM
    std::optional<int32_t> fi_weak_lo;
    /// Field weak max current A (FOC only)
    std::optional<int32_t> fi_weak_max;
    /// Max Phase Adv angle Deg (SIN only)
    std::optional<int32_t> phase_adv_max_deg;
    /// Input1 raw value
    std::optional<int32_t> input1_raw;
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> input1_typ;
    /// Input1 minimum value
    std::optional<int32_t> input1_min;
    /// Input1 middle value
    std::optional<int32_t> input1_mid;
    /// Input1 maximum value
    std::optional<int32_t> input1_max;
    /// Input1 command value
    std::optional<int32_t> input1_cmd;
    /// Input2 raw value
    std::optional<int32_t> input2_raw;
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> input2_typ;
    /// Input2 minimum value
    std::optional<int32_t> input2_min;
    /// Input2 middle value
    std::optional<int32_t> input2_mid;
    /// Input2 maximum value
    std::optional<int32_t> input2_max;
    /// Input2 command value
    std::optional<int32_t> input2_cmd;
    /// Input1 raw value
    std::optional<int32_t> aux_input1_raw;
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> aux_input1_typ;
    /// Input1 minimum value
    std::optional<int32_t> aux_input1_min;
    /// Input1 middle value
    std::optional<int32_t> aux_input1_mid;
    /// Input1 maximum value
    std::optional<int32_t> aux_input1_max;
    /// Input1 command value
    std::optional<int32_t> aux_input1_cmd;
    /// Input2 raw value
    std::optional<int32_t> aux_input2_raw;
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> aux_input2_typ;
    /// Input2 minimum value
    std::optional<int32_t> aux_input2_min;
    /// Input2 middle value
    std::optional<int32_t> aux_input2_mid;
    /// Input2 maximum value
    std::optional<int32_t> aux_input2_max;
    /// Input2 command value
    std::optional<int32_t> aux_input2_cmd;
    /// Total DC Link current A *100
    std::optional<float> dc_curr;
    /// Right DC Link current A *100
    std::optional<float> rdc_curr;
    /// Left DC Link current A *100
    std::optional<float> ldc_curr;
    /// Left Motor Command RPM
    std::optional<int32_t> cmdl;
    /// Right Motor Command RPM
    std::optional<int32_t> cmdr;
    /// Motor Measured Avg RPM
    std::optional<int32_t> spd_avg;
    /// Left Motor Measured RPM
    std::optional<int32_t> spdl;
    /// Right Motor Measured RPM
    std::optional<int32_t> spdr;
    /// Rate *10
    std::optional<int32_t> filter_rate;
    /// Speed Coefficient *10
    std::optional<int32_t> spd_coef;
    /// Steer Coefficient *10
    std::optional<int32_t> str_coef;
    /// Calibrated Battery Voltage *100
    std::optional<float> batv;
    /// Calibrated Temperature C *10
    std::optional<float> temp;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (ctrl_mod.has_value()) {
            ++mapSize;
        }
        if (ctrl_typ.has_value()) {
            ++mapSize;
        }
        if (cur_mot_max.has_value()) {
            ++mapSize;
        }
        if (rpm_mot_max.has_value()) {
            ++mapSize;
        }
        if (fi_weak_ena.has_value()) {
            ++mapSize;
        }
        if (fi_weak_hi.has_value()) {
            ++mapSize;
        }
        if (fi_weak_lo.has_value()) {
            ++mapSize;
        }
        if (fi_weak_max.has_value()) {
            ++mapSize;
        }
        if (phase_adv_max_deg.has_value()) {
            ++mapSize;
        }
        if (input1_raw.has_value()) {
            ++mapSize;
        }
        if (input1_typ.has_value()) {
            ++mapSize;
        }
        if (input1_min.has_value()) {
            ++mapSize;
        }
        if (input1_mid.has_value()) {
            ++mapSize;
        }
        if (input1_max.has_value()) {
            ++mapSize;
        }
        if (input1_cmd.has_value()) {
            ++mapSize;
        }
        if (input2_raw.has_value()) {
            ++mapSize;
        }
        if (input2_typ.has_value()) {
            ++mapSize;
        }
        if (input2_min.has_value()) {
            ++mapSize;
        }
        if (input2_mid.has_value()) {
            ++mapSize;
        }
        if (input2_max.has_value()) {
            ++mapSize;
        }
        if (input2_cmd.has_value()) {
            ++mapSize;
        }
        if (aux_input1_raw.has_value()) {
            ++mapSize;
        }
        if (aux_input1_typ.has_value()) {
            ++mapSize;
        }
        if (aux_input1_min.has_value()) {
            ++mapSize;
        }
        if (aux_input1_mid.has_value()) {
            ++mapSize;
        }
        if (aux_input1_max.has_value()) {
            ++mapSize;
        }
        if (aux_input1_cmd.has_value()) {
            ++mapSize;
        }
        if (aux_input2_raw.has_value()) {
            ++mapSize;
        }
        if (aux_input2_typ.has_value()) {
            ++mapSize;
        }
        if (aux_input2_min.has_value()) {
            ++mapSize;
        }
        if (aux_input2_mid.has_value()) {
            ++mapSize;
        }
        if (aux_input2_max.has_value()) {
            ++mapSize;
        }
        if (aux_input2_cmd.has_value()) {
            ++mapSize;
        }
        if (dc_curr.has_value()) {
            ++mapSize;
        }
        if (rdc_curr.has_value()) {
            ++mapSize;
        }
        if (ldc_curr.has_value()) {
            ++mapSize;
        }
        if (cmdl.has_value()) {
            ++mapSize;
        }
        if (cmdr.has_value()) {
            ++mapSize;
        }
        if (spd_avg.has_value()) {
            ++mapSize;
        }
        if (spdl.has_value()) {
            ++mapSize;
        }
        if (spdr.has_value()) {
            ++mapSize;
        }
        if (filter_rate.has_value()) {
            ++mapSize;
        }
        if (spd_coef.has_value()) {
            ++mapSize;
        }
        if (str_coef.has_value()) {
            ++mapSize;
        }
        if (batv.has_value()) {
            ++mapSize;
        }
        if (temp.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (ctrl_mod.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, ctrl_mod);
        }
        if (err != CborNoError) return err;
        if (ctrl_typ.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, ctrl_typ);
        }
        if (err != CborNoError) return err;
        if (cur_mot_max.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, cur_mot_max);
        }
        if (err != CborNoError) return err;
        if (rpm_mot_max.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, rpm_mot_max);
        }
        if (err != CborNoError) return err;
        if (fi_weak_ena.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, fi_weak_ena);
        }
        if (err != CborNoError) return err;
        if (fi_weak_hi.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, fi_weak_hi);
        }
        if (err != CborNoError) return err;
        if (fi_weak_lo.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(6));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, fi_weak_lo);
        }
        if (err != CborNoError) return err;
        if (fi_weak_max.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(7));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, fi_weak_max);
        }
        if (err != CborNoError) return err;
        if (phase_adv_max_deg.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(8));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, phase_adv_max_deg);
        }
        if (err != CborNoError) return err;
        if (input1_raw.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(9));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input1_raw);
        }
        if (err != CborNoError) return err;
        if (input1_typ.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(10));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input1_typ);
        }
        if (err != CborNoError) return err;
        if (input1_min.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(11));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input1_min);
        }
        if (err != CborNoError) return err;
        if (input1_mid.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(12));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input1_mid);
        }
        if (err != CborNoError) return err;
        if (input1_max.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(13));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input1_max);
        }
        if (err != CborNoError) return err;
        if (input1_cmd.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(14));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input1_cmd);
        }
        if (err != CborNoError) return err;
        if (input2_raw.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(15));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input2_raw);
        }
        if (err != CborNoError) return err;
        if (input2_typ.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(16));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input2_typ);
        }
        if (err != CborNoError) return err;
        if (input2_min.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(17));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input2_min);
        }
        if (err != CborNoError) return err;
        if (input2_mid.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(18));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input2_mid);
        }
        if (err != CborNoError) return err;
        if (input2_max.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(19));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input2_max);
        }
        if (err != CborNoError) return err;
        if (input2_cmd.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(20));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, input2_cmd);
        }
        if (err != CborNoError) return err;
        if (aux_input1_raw.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(21));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input1_raw);
        }
        if (err != CborNoError) return err;
        if (aux_input1_typ.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(22));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input1_typ);
        }
        if (err != CborNoError) return err;
        if (aux_input1_min.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(23));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input1_min);
        }
        if (err != CborNoError) return err;
        if (aux_input1_mid.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(24));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input1_mid);
        }
        if (err != CborNoError) return err;
        if (aux_input1_max.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(25));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input1_max);
        }
        if (err != CborNoError) return err;
        if (aux_input1_cmd.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(26));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input1_cmd);
        }
        if (err != CborNoError) return err;
        if (aux_input2_raw.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(27));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input2_raw);
        }
        if (err != CborNoError) return err;
        if (aux_input2_typ.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(28));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input2_typ);
        }
        if (err != CborNoError) return err;
        if (aux_input2_min.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(29));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input2_min);
        }
        if (err != CborNoError) return err;
        if (aux_input2_mid.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(30));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input2_mid);
        }
        if (err != CborNoError) return err;
        if (aux_input2_max.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(31));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input2_max);
        }
        if (err != CborNoError) return err;
        if (aux_input2_cmd.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(32));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, aux_input2_cmd);
        }
        if (err != CborNoError) return err;
        if (dc_curr.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(33));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, dc_curr);
        }
        if (err != CborNoError) return err;
        if (rdc_curr.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(34));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, rdc_curr);
        }
        if (err != CborNoError) return err;
        if (ldc_curr.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(35));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, ldc_curr);
        }
        if (err != CborNoError) return err;
        if (cmdl.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(36));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, cmdl);
        }
        if (err != CborNoError) return err;
        if (cmdr.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(37));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, cmdr);
        }
        if (err != CborNoError) return err;
        if (spd_avg.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(38));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, spd_avg);
        }
        if (err != CborNoError) return err;
        if (spdl.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(39));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, spdl);
        }
        if (err != CborNoError) return err;
        if (spdr.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(40));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, spdr);
        }
        if (err != CborNoError) return err;
        if (filter_rate.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(41));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, filter_rate);
        }
        if (err != CborNoError) return err;
        if (spd_coef.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(42));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, spd_coef);
        }
        if (err != CborNoError) return err;
        if (str_coef.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(43));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, str_coef);
        }
        if (err != CborNoError) return err;
        if (batv.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(44));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, batv);
        }
        if (err != CborNoError) return err;
        if (temp.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(45));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, temp);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a HoverboardEvent from a CBOR map value.
    static HoverboardEvent fromCbor(CborValue& it) {
        HoverboardEvent msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalInt32(map, msg.ctrl_mod);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalInt32(map, msg.ctrl_typ);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalInt32(map, msg.cur_mot_max);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalInt32(map, msg.rpm_mot_max);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalInt32(map, msg.fi_weak_ena);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalInt32(map, msg.fi_weak_hi);
                        break;
                    case 6:

                    msgs_detail::decodeOptionalInt32(map, msg.fi_weak_lo);
                        break;
                    case 7:

                    msgs_detail::decodeOptionalInt32(map, msg.fi_weak_max);
                        break;
                    case 8:

                    msgs_detail::decodeOptionalInt32(map, msg.phase_adv_max_deg);
                        break;
                    case 9:

                    msgs_detail::decodeOptionalInt32(map, msg.input1_raw);
                        break;
                    case 10:

                    msgs_detail::decodeOptionalInt32(map, msg.input1_typ);
                        break;
                    case 11:

                    msgs_detail::decodeOptionalInt32(map, msg.input1_min);
                        break;
                    case 12:

                    msgs_detail::decodeOptionalInt32(map, msg.input1_mid);
                        break;
                    case 13:

                    msgs_detail::decodeOptionalInt32(map, msg.input1_max);
                        break;
                    case 14:

                    msgs_detail::decodeOptionalInt32(map, msg.input1_cmd);
                        break;
                    case 15:

                    msgs_detail::decodeOptionalInt32(map, msg.input2_raw);
                        break;
                    case 16:

                    msgs_detail::decodeOptionalInt32(map, msg.input2_typ);
                        break;
                    case 17:

                    msgs_detail::decodeOptionalInt32(map, msg.input2_min);
                        break;
                    case 18:

                    msgs_detail::decodeOptionalInt32(map, msg.input2_mid);
                        break;
                    case 19:

                    msgs_detail::decodeOptionalInt32(map, msg.input2_max);
                        break;
                    case 20:

                    msgs_detail::decodeOptionalInt32(map, msg.input2_cmd);
                        break;
                    case 21:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input1_raw);
                        break;
                    case 22:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input1_typ);
                        break;
                    case 23:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input1_min);
                        break;
                    case 24:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input1_mid);
                        break;
                    case 25:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input1_max);
                        break;
                    case 26:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input1_cmd);
                        break;
                    case 27:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input2_raw);
                        break;
                    case 28:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input2_typ);
                        break;
                    case 29:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input2_min);
                        break;
                    case 30:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input2_mid);
                        break;
                    case 31:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input2_max);
                        break;
                    case 32:

                    msgs_detail::decodeOptionalInt32(map, msg.aux_input2_cmd);
                        break;
                    case 33:

                    msgs_detail::decodeOptionalFloat(map, msg.dc_curr);
                        break;
                    case 34:

                    msgs_detail::decodeOptionalFloat(map, msg.rdc_curr);
                        break;
                    case 35:

                    msgs_detail::decodeOptionalFloat(map, msg.ldc_curr);
                        break;
                    case 36:

                    msgs_detail::decodeOptionalInt32(map, msg.cmdl);
                        break;
                    case 37:

                    msgs_detail::decodeOptionalInt32(map, msg.cmdr);
                        break;
                    case 38:

                    msgs_detail::decodeOptionalInt32(map, msg.spd_avg);
                        break;
                    case 39:

                    msgs_detail::decodeOptionalInt32(map, msg.spdl);
                        break;
                    case 40:

                    msgs_detail::decodeOptionalInt32(map, msg.spdr);
                        break;
                    case 41:

                    msgs_detail::decodeOptionalInt32(map, msg.filter_rate);
                        break;
                    case 42:

                    msgs_detail::decodeOptionalInt32(map, msg.spd_coef);
                        break;
                    case 43:

                    msgs_detail::decodeOptionalInt32(map, msg.str_coef);
                        break;
                    case 44:

                    msgs_detail::decodeOptionalFloat(map, msg.batv);
                        break;
                    case 45:

                    msgs_detail::decodeOptionalFloat(map, msg.temp);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct HoverboardRequest {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    /// Speed command for the hoverboard
    std::optional<int32_t> speed;
    /// Steering command for the hoverboard
    std::optional<int32_t> steer;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (req_id.has_value()) {
            ++mapSize;
        }
        if (speed.has_value()) {
            ++mapSize;
        }
        if (steer.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, req_id);
        }
        if (err != CborNoError) return err;
        if (speed.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, speed);
        }
        if (err != CborNoError) return err;
        if (steer.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, steer);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a HoverboardRequest from a CBOR map value.
    static HoverboardRequest fromCbor(CborValue& it) {
        HoverboardRequest msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalInt32(map, msg.speed);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalInt32(map, msg.steer);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct Max31855Event {
    /// Thermocouple temperature in Celsius
    std::optional<float> thermocouple_temp;
    /// Internal temperature in Celsius
    std::optional<float> internal_temp;
    /// Fault detected
    std::optional<bool> fault;
    /// Short to VCC detected
    std::optional<bool> fault_short_vcc;
    /// Short to GND detected
    std::optional<bool> fault_short_gnd;
    /// Open thermocouple detected
    std::optional<bool> fault_open_tc;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (thermocouple_temp.has_value()) {
            ++mapSize;
        }
        if (internal_temp.has_value()) {
            ++mapSize;
        }
        if (fault.has_value()) {
            ++mapSize;
        }
        if (fault_short_vcc.has_value()) {
            ++mapSize;
        }
        if (fault_short_gnd.has_value()) {
            ++mapSize;
        }
        if (fault_open_tc.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (thermocouple_temp.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, thermocouple_temp);
        }
        if (err != CborNoError) return err;
        if (internal_temp.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalFloat(mapEncoder, internal_temp);
        }
        if (err != CborNoError) return err;
        if (fault.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, fault);
        }
        if (err != CborNoError) return err;
        if (fault_short_vcc.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, fault_short_vcc);
        }
        if (err != CborNoError) return err;
        if (fault_short_gnd.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, fault_short_gnd);
        }
        if (err != CborNoError) return err;
        if (fault_open_tc.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, fault_open_tc);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a Max31855Event from a CBOR map value.
    static Max31855Event fromCbor(CborValue& it) {
        Max31855Event msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalFloat(map, msg.thermocouple_temp);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalFloat(map, msg.internal_temp);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalBool(map, msg.fault);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalBool(map, msg.fault_short_vcc);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalBool(map, msg.fault_short_gnd);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalBool(map, msg.fault_open_tc);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct PingReply {
    std::optional<uint32_t> req_id;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (req_id.has_value()) {
            ++mapSize;
        }
        if (timestamp.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, req_id);
        }
        if (err != CborNoError) return err;
        if (timestamp.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, timestamp);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a PingReply from a CBOR map value.
    static PingReply fromCbor(CborValue& it) {
        PingReply msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalUInt64(map, msg.timestamp);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct PingRequest {
    std::optional<uint32_t> req_id;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (req_id.has_value()) {
            ++mapSize;
        }
        if (timestamp.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, req_id);
        }
        if (err != CborNoError) return err;
        if (timestamp.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, timestamp);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a PingRequest from a CBOR map value.
    static PingRequest fromCbor(CborValue& it) {
        PingRequest msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalUInt64(map, msg.timestamp);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct Ps4Event {
    std::optional<bool> button_left;
    std::optional<bool> button_right;
    std::optional<bool> button_up;
    std::optional<bool> button_down;
    std::optional<bool> button_square;
    std::optional<bool> button_cross;
    std::optional<bool> button_circle;
    std::optional<bool> button_triangle;
    std::optional<bool> button_left_shoulder;
    std::optional<bool> button_right_shoulder;
    std::optional<bool> button_left_trigger;
    std::optional<bool> button_right_trigger;
    std::optional<bool> button_left_joystick;
    std::optional<bool> button_right_joystick;
    std::optional<bool> button_share;
    std::optional<bool> button_options;
    std::optional<bool> button_touchpad;
    std::optional<bool> button_ps;
    std::optional<int32_t> axis_lx;
    std::optional<int32_t> axis_ly;
    std::optional<int32_t> axis_rx;
    std::optional<int32_t> axis_ry;
    std::optional<int32_t> gyro_x;
    std::optional<int32_t> gyro_y;
    std::optional<int32_t> gyro_z;
    std::optional<int32_t> accel_x;
    std::optional<int32_t> accel_y;
    std::optional<int32_t> accel_z;
    std::optional<bool> connected;
    std::optional<int32_t> battery_level;
    std::optional<bool> bluetooth;
    std::optional<std::string> debug;
    std::optional<int32_t> temp;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (button_left.has_value()) {
            ++mapSize;
        }
        if (button_right.has_value()) {
            ++mapSize;
        }
        if (button_up.has_value()) {
            ++mapSize;
        }
        if (button_down.has_value()) {
            ++mapSize;
        }
        if (button_square.has_value()) {
            ++mapSize;
        }
        if (button_cross.has_value()) {
            ++mapSize;
        }
        if (button_circle.has_value()) {
            ++mapSize;
        }
        if (button_triangle.has_value()) {
            ++mapSize;
        }
        if (button_left_shoulder.has_value()) {
            ++mapSize;
        }
        if (button_right_shoulder.has_value()) {
            ++mapSize;
        }
        if (button_left_trigger.has_value()) {
            ++mapSize;
        }
        if (button_right_trigger.has_value()) {
            ++mapSize;
        }
        if (button_left_joystick.has_value()) {
            ++mapSize;
        }
        if (button_right_joystick.has_value()) {
            ++mapSize;
        }
        if (button_share.has_value()) {
            ++mapSize;
        }
        if (button_options.has_value()) {
            ++mapSize;
        }
        if (button_touchpad.has_value()) {
            ++mapSize;
        }
        if (button_ps.has_value()) {
            ++mapSize;
        }
        if (axis_lx.has_value()) {
            ++mapSize;
        }
        if (axis_ly.has_value()) {
            ++mapSize;
        }
        if (axis_rx.has_value()) {
            ++mapSize;
        }
        if (axis_ry.has_value()) {
            ++mapSize;
        }
        if (gyro_x.has_value()) {
            ++mapSize;
        }
        if (gyro_y.has_value()) {
            ++mapSize;
        }
        if (gyro_z.has_value()) {
            ++mapSize;
        }
        if (accel_x.has_value()) {
            ++mapSize;
        }
        if (accel_y.has_value()) {
            ++mapSize;
        }
        if (accel_z.has_value()) {
            ++mapSize;
        }
        if (connected.has_value()) {
            ++mapSize;
        }
        if (battery_level.has_value()) {
            ++mapSize;
        }
        if (bluetooth.has_value()) {
            ++mapSize;
        }
        if (debug.has_value()) {
            ++mapSize;
        }
        if (temp.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (button_left.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_left);
        }
        if (err != CborNoError) return err;
        if (button_right.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_right);
        }
        if (err != CborNoError) return err;
        if (button_up.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_up);
        }
        if (err != CborNoError) return err;
        if (button_down.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_down);
        }
        if (err != CborNoError) return err;
        if (button_square.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_square);
        }
        if (err != CborNoError) return err;
        if (button_cross.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_cross);
        }
        if (err != CborNoError) return err;
        if (button_circle.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(6));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_circle);
        }
        if (err != CborNoError) return err;
        if (button_triangle.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(7));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_triangle);
        }
        if (err != CborNoError) return err;
        if (button_left_shoulder.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(8));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_left_shoulder);
        }
        if (err != CborNoError) return err;
        if (button_right_shoulder.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(9));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_right_shoulder);
        }
        if (err != CborNoError) return err;
        if (button_left_trigger.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(10));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_left_trigger);
        }
        if (err != CborNoError) return err;
        if (button_right_trigger.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(11));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_right_trigger);
        }
        if (err != CborNoError) return err;
        if (button_left_joystick.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(12));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_left_joystick);
        }
        if (err != CborNoError) return err;
        if (button_right_joystick.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(13));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_right_joystick);
        }
        if (err != CborNoError) return err;
        if (button_share.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(14));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_share);
        }
        if (err != CborNoError) return err;
        if (button_options.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(15));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_options);
        }
        if (err != CborNoError) return err;
        if (button_touchpad.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(16));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_touchpad);
        }
        if (err != CborNoError) return err;
        if (button_ps.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(17));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, button_ps);
        }
        if (err != CborNoError) return err;
        if (axis_lx.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(18));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, axis_lx);
        }
        if (err != CborNoError) return err;
        if (axis_ly.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(19));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, axis_ly);
        }
        if (err != CborNoError) return err;
        if (axis_rx.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(20));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, axis_rx);
        }
        if (err != CborNoError) return err;
        if (axis_ry.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(21));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, axis_ry);
        }
        if (err != CborNoError) return err;
        if (gyro_x.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(22));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, gyro_x);
        }
        if (err != CborNoError) return err;
        if (gyro_y.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(23));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, gyro_y);
        }
        if (err != CborNoError) return err;
        if (gyro_z.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(24));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, gyro_z);
        }
        if (err != CborNoError) return err;
        if (accel_x.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(25));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, accel_x);
        }
        if (err != CborNoError) return err;
        if (accel_y.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(26));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, accel_y);
        }
        if (err != CborNoError) return err;
        if (accel_z.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(27));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, accel_z);
        }
        if (err != CborNoError) return err;
        if (connected.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(28));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, connected);
        }
        if (err != CborNoError) return err;
        if (battery_level.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(29));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, battery_level);
        }
        if (err != CborNoError) return err;
        if (bluetooth.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(30));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, bluetooth);
        }
        if (err != CborNoError) return err;
        if (debug.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(31));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, debug);
        }
        if (err != CborNoError) return err;
        if (temp.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(32));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, temp);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a Ps4Event from a CBOR map value.
    static Ps4Event fromCbor(CborValue& it) {
        Ps4Event msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalBool(map, msg.button_left);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalBool(map, msg.button_right);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalBool(map, msg.button_up);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalBool(map, msg.button_down);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalBool(map, msg.button_square);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalBool(map, msg.button_cross);
                        break;
                    case 6:

                    msgs_detail::decodeOptionalBool(map, msg.button_circle);
                        break;
                    case 7:

                    msgs_detail::decodeOptionalBool(map, msg.button_triangle);
                        break;
                    case 8:

                    msgs_detail::decodeOptionalBool(map, msg.button_left_shoulder);
                        break;
                    case 9:

                    msgs_detail::decodeOptionalBool(map, msg.button_right_shoulder);
                        break;
                    case 10:

                    msgs_detail::decodeOptionalBool(map, msg.button_left_trigger);
                        break;
                    case 11:

                    msgs_detail::decodeOptionalBool(map, msg.button_right_trigger);
                        break;
                    case 12:

                    msgs_detail::decodeOptionalBool(map, msg.button_left_joystick);
                        break;
                    case 13:

                    msgs_detail::decodeOptionalBool(map, msg.button_right_joystick);
                        break;
                    case 14:

                    msgs_detail::decodeOptionalBool(map, msg.button_share);
                        break;
                    case 15:

                    msgs_detail::decodeOptionalBool(map, msg.button_options);
                        break;
                    case 16:

                    msgs_detail::decodeOptionalBool(map, msg.button_touchpad);
                        break;
                    case 17:

                    msgs_detail::decodeOptionalBool(map, msg.button_ps);
                        break;
                    case 18:

                    msgs_detail::decodeOptionalInt32(map, msg.axis_lx);
                        break;
                    case 19:

                    msgs_detail::decodeOptionalInt32(map, msg.axis_ly);
                        break;
                    case 20:

                    msgs_detail::decodeOptionalInt32(map, msg.axis_rx);
                        break;
                    case 21:

                    msgs_detail::decodeOptionalInt32(map, msg.axis_ry);
                        break;
                    case 22:

                    msgs_detail::decodeOptionalInt32(map, msg.gyro_x);
                        break;
                    case 23:

                    msgs_detail::decodeOptionalInt32(map, msg.gyro_y);
                        break;
                    case 24:

                    msgs_detail::decodeOptionalInt32(map, msg.gyro_z);
                        break;
                    case 25:

                    msgs_detail::decodeOptionalInt32(map, msg.accel_x);
                        break;
                    case 26:

                    msgs_detail::decodeOptionalInt32(map, msg.accel_y);
                        break;
                    case 27:

                    msgs_detail::decodeOptionalInt32(map, msg.accel_z);
                        break;
                    case 28:

                    msgs_detail::decodeOptionalBool(map, msg.connected);
                        break;
                    case 29:

                    msgs_detail::decodeOptionalInt32(map, msg.battery_level);
                        break;
                    case 30:

                    msgs_detail::decodeOptionalBool(map, msg.bluetooth);
                        break;
                    case 31:

                    msgs_detail::decodeOptionalString(map, msg.debug);
                        break;
                    case 32:

                    msgs_detail::decodeOptionalInt32(map, msg.temp);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct Ps4Request {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<int32_t> rumble_small;
    std::optional<int32_t> rumble_large;
    std::optional<int32_t> led_red;
    std::optional<int32_t> led_green;
    std::optional<int32_t> led_blue;
    std::optional<int32_t> led_flash_on;
    std::optional<int32_t> led_flash_off;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (req_id.has_value()) {
            ++mapSize;
        }
        if (rumble_small.has_value()) {
            ++mapSize;
        }
        if (rumble_large.has_value()) {
            ++mapSize;
        }
        if (led_red.has_value()) {
            ++mapSize;
        }
        if (led_green.has_value()) {
            ++mapSize;
        }
        if (led_blue.has_value()) {
            ++mapSize;
        }
        if (led_flash_on.has_value()) {
            ++mapSize;
        }
        if (led_flash_off.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, req_id);
        }
        if (err != CborNoError) return err;
        if (rumble_small.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, rumble_small);
        }
        if (err != CborNoError) return err;
        if (rumble_large.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, rumble_large);
        }
        if (err != CborNoError) return err;
        if (led_red.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, led_red);
        }
        if (err != CborNoError) return err;
        if (led_green.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, led_green);
        }
        if (err != CborNoError) return err;
        if (led_blue.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, led_blue);
        }
        if (err != CborNoError) return err;
        if (led_flash_on.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(6));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, led_flash_on);
        }
        if (err != CborNoError) return err;
        if (led_flash_off.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(7));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, led_flash_off);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a Ps4Request from a CBOR map value.
    static Ps4Request fromCbor(CborValue& it) {
        Ps4Request msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalInt32(map, msg.rumble_small);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalInt32(map, msg.rumble_large);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalInt32(map, msg.led_red);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalInt32(map, msg.led_green);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalInt32(map, msg.led_blue);
                        break;
                    case 6:

                    msgs_detail::decodeOptionalInt32(map, msg.led_flash_on);
                        break;
                    case 7:

                    msgs_detail::decodeOptionalInt32(map, msg.led_flash_off);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct SysEvent {
    std::optional<uint64_t> utc;
    std::optional<uint64_t> uptime;
    std::optional<uint64_t> free_heap;
    std::optional<uint64_t> flash;
    std::optional<std::string> cpu_board;
    std::optional<std::string> build_date;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (utc.has_value()) {
            ++mapSize;
        }
        if (uptime.has_value()) {
            ++mapSize;
        }
        if (free_heap.has_value()) {
            ++mapSize;
        }
        if (flash.has_value()) {
            ++mapSize;
        }
        if (cpu_board.has_value()) {
            ++mapSize;
        }
        if (build_date.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (utc.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, utc);
        }
        if (err != CborNoError) return err;
        if (uptime.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, uptime);
        }
        if (err != CborNoError) return err;
        if (free_heap.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, free_heap);
        }
        if (err != CborNoError) return err;
        if (flash.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, flash);
        }
        if (err != CborNoError) return err;
        if (cpu_board.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, cpu_board);
        }
        if (err != CborNoError) return err;
        if (build_date.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, build_date);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a SysEvent from a CBOR map value.
    static SysEvent fromCbor(CborValue& it) {
        SysEvent msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt64(map, msg.utc);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalUInt64(map, msg.uptime);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalUInt64(map, msg.free_heap);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalUInt64(map, msg.flash);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalString(map, msg.cpu_board);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalString(map, msg.build_date);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct SysReply {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<int32_t> rc;
    std::optional<std::string> message;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (req_id.has_value()) {
            ++mapSize;
        }
        if (rc.has_value()) {
            ++mapSize;
        }
        if (message.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, req_id);
        }
        if (err != CborNoError) return err;
        if (rc.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, rc);
        }
        if (err != CborNoError) return err;
        if (message.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, message);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a SysReply from a CBOR map value.
    static SysReply fromCbor(CborValue& it) {
        SysReply msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalInt32(map, msg.rc);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalString(map, msg.message);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct SysRequest {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<uint64_t> set_time;
    std::optional<bool> reboot;
    std::optional<std::string> console;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (req_id.has_value()) {
            ++mapSize;
        }
        if (set_time.has_value()) {
            ++mapSize;
        }
        if (reboot.has_value()) {
            ++mapSize;
        }
        if (console.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt32(mapEncoder, req_id);
        }
        if (err != CborNoError) return err;
        if (set_time.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalUInt64(mapEncoder, set_time);
        }
        if (err != CborNoError) return err;
        if (reboot.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalBool(mapEncoder, reboot);
        }
        if (err != CborNoError) return err;
        if (console.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, console);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a SysRequest from a CBOR map value.
    static SysRequest fromCbor(CborValue& it) {
        SysRequest msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalUInt64(map, msg.set_time);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalBool(map, msg.reboot);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalString(map, msg.console);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


struct WifiEvent {
    std::optional<std::string> ip;
    std::optional<std::string> gateway;
    std::optional<std::string> netmask;
    std::optional<std::string> ssid;
    std::optional<std::string> bssid;
    std::optional<int32_t> channel;
    std::optional<int32_t> rssi;
    std::optional<std::string> mac;

    /// Serialize this message into a CBOR map keyed by field id.
    CborError toCbor(CborEncoder& parentEncoder) const {
        size_t mapSize = 0;
        if (ip.has_value()) {
            ++mapSize;
        }
        if (gateway.has_value()) {
            ++mapSize;
        }
        if (netmask.has_value()) {
            ++mapSize;
        }
        if (ssid.has_value()) {
            ++mapSize;
        }
        if (bssid.has_value()) {
            ++mapSize;
        }
        if (channel.has_value()) {
            ++mapSize;
        }
        if (rssi.has_value()) {
            ++mapSize;
        }
        if (mac.has_value()) {
            ++mapSize;
        }

        CborEncoder mapEncoder;
        CborError err = cbor_encoder_create_map(
            &parentEncoder, &mapEncoder, mapSize);
        if (err != CborNoError) return err;
        if (ip.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(0));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, ip);
        }
        if (err != CborNoError) return err;
        if (gateway.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(1));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, gateway);
        }
        if (err != CborNoError) return err;
        if (netmask.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(2));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, netmask);
        }
        if (err != CborNoError) return err;
        if (ssid.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(3));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, ssid);
        }
        if (err != CborNoError) return err;
        if (bssid.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(4));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, bssid);
        }
        if (err != CborNoError) return err;
        if (channel.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(5));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, channel);
        }
        if (err != CborNoError) return err;
        if (rssi.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(6));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalInt32(mapEncoder, rssi);
        }
        if (err != CborNoError) return err;
        if (mac.has_value()) {
            err = cbor_encode_uint(&mapEncoder, static_cast<uint64_t>(7));
            if (err != CborNoError) return err;

            err = msgs_detail::encodeOptionalString(mapEncoder, mac);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &mapEncoder);
    }

    /// Deserialize a WifiEvent from a CBOR map value.
    static WifiEvent fromCbor(CborValue& it) {
        WifiEvent msg;
        CborValue map;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &map) == CborNoError)
        {
            while (!cbor_value_at_end(&map)) {
                uint64_t key = 0;
                if (cbor_value_is_unsigned_integer(&map)) {
                    cbor_value_get_uint64(&map, &key);
                } else if (cbor_value_is_integer(&map)) {
                    int64_t signedKey = 0;
                    cbor_value_get_int64(&map, &signedKey);
                    if (signedKey < 0) {
                        cbor_value_advance(&map);
                        if (!cbor_value_at_end(&map)) {
                            cbor_value_advance(&map);
                        }
                        continue;
                    }
                    key = static_cast<uint64_t>(signedKey);
                } else {
                    cbor_value_advance(&map);
                    if (!cbor_value_at_end(&map)) {
                        cbor_value_advance(&map);
                    }
                    continue;
                }

                cbor_value_advance(&map);
                if (cbor_value_at_end(&map)) {
                    break;
                }

                switch (key) {
                    case 0:

                    msgs_detail::decodeOptionalString(map, msg.ip);
                        break;
                    case 1:

                    msgs_detail::decodeOptionalString(map, msg.gateway);
                        break;
                    case 2:

                    msgs_detail::decodeOptionalString(map, msg.netmask);
                        break;
                    case 3:

                    msgs_detail::decodeOptionalString(map, msg.ssid);
                        break;
                    case 4:

                    msgs_detail::decodeOptionalString(map, msg.bssid);
                        break;
                    case 5:

                    msgs_detail::decodeOptionalInt32(map, msg.channel);
                        break;
                    case 6:

                    msgs_detail::decodeOptionalInt32(map, msg.rssi);
                        break;
                    case 7:

                    msgs_detail::decodeOptionalString(map, msg.mac);
                        break;
                    default:
                        break;
                }

                if (!cbor_value_at_end(&map)) {
                    cbor_value_advance(&map);
                }
            }
            cbor_value_leave_container(&it, &map);
        }
        return msg;
    }
};


