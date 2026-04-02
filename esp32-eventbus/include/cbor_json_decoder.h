#ifndef CBOR_JSON_DECODER_H
#define CBOR_JSON_DECODER_H

#include <ArduinoJson.h>
#include <cbor.h>

#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <string>

namespace cbor_json
{

namespace detail
{

inline bool set_error(std::string *error, const char *msg)
{
    if (error != nullptr)
    {
        *error = (msg != nullptr) ? msg : "unknown error";
    }
    return false;
}

inline bool set_error(std::string *error, CborError err, const char *msg)
{
    if (error != nullptr)
    {
        *error = std::string(msg != nullptr ? msg : "CBOR error") + ": " +
                 (cbor_error_string(err) != nullptr ? cbor_error_string(err) : "unknown");
    }
    return false;
}

inline bool copy_text_string(CborValue *it, std::string &out, std::string *error)
{
    char *tmp = nullptr;
    size_t copy_len = 0;
    CborValue next;
    CborError err = cbor_value_dup_text_string(it, &tmp, &copy_len, &next);
    if (err != CborNoError)
    {
        return set_error(error, err, "failed to duplicate text string");
    }

    out.assign(tmp != nullptr ? tmp : "", copy_len);
    std::free(tmp);
    *it = next;
    return true;
}

inline bool map_key_to_string(CborValue *it, std::string &key, std::string *error)
{
    CborType t = cbor_value_get_type(it);

    if (t == CborTextStringType)
    {
        return copy_text_string(it, key, error);
    }

    if (t == CborIntegerType)
    {
        if (cbor_value_is_unsigned_integer(it))
        {
            uint64_t v = 0;
            CborError err = cbor_value_get_uint64(it, &v);
            if (err != CborNoError)
            {
                return set_error(error, err, "failed to decode unsigned map key");
            }
            key = std::to_string(static_cast<unsigned long long>(v));
        }
        else
        {
            int64_t v = 0;
            CborError err = cbor_value_get_int64(it, &v);
            if (err != CborNoError)
            {
                return set_error(error, err, "failed to decode signed map key");
            }
            key = std::to_string(static_cast<long long>(v));
        }
        CborError err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance after integer map key");
        }
        return true;
    }

    if (t == CborBooleanType)
    {
        bool b = false;
        CborError err = cbor_value_get_boolean(it, &b);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to decode boolean map key");
        }
        key = b ? "true" : "false";
        err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance after boolean map key");
        }
        return true;
    }

    return set_error(error, "unsupported CBOR map key type (expected text/integer/bool)");
}

inline bool decode_value(CborValue *it, JsonVariant dst, std::string *error, uint8_t depth);

inline bool decode_array(CborValue *it, JsonVariant dst, std::string *error, uint8_t depth)
{
    if (depth > 32)
    {
        return set_error(error, "maximum nesting depth exceeded");
    }

    JsonArray arr = dst.to<JsonArray>();
    CborValue elem;
    CborError err = cbor_value_enter_container(it, &elem);
    if (err != CborNoError)
    {
        return set_error(error, err, "failed to enter CBOR array");
    }

    while (!cbor_value_at_end(&elem))
    {
        JsonVariant slot = arr.add<JsonVariant>();
        if (slot.isUnbound())
        {
            return set_error(error, "ArduinoJson allocation failed for array element");
        }

        if (!decode_value(&elem, slot, error, static_cast<uint8_t>(depth + 1)))
        {
            return false;
        }
    }

    err = cbor_value_leave_container(it, &elem);
    if (err != CborNoError)
    {
        return set_error(error, err, "failed to leave CBOR array");
    }

    return true;
}

inline bool decode_map(CborValue *it, JsonVariant dst, std::string *error, uint8_t depth)
{
    if (depth > 32)
    {
        return set_error(error, "maximum nesting depth exceeded");
    }

    JsonObject obj = dst.to<JsonObject>();
    CborValue map_it;
    CborError err = cbor_value_enter_container(it, &map_it);
    if (err != CborNoError)
    {
        return set_error(error, err, "failed to enter CBOR map");
    }

    while (!cbor_value_at_end(&map_it))
    {
        std::string key;
        if (!map_key_to_string(&map_it, key, error))
        {
            return false;
        }

        JsonVariant value_slot = obj[key.c_str()];
        if (value_slot.isUnbound())
        {
            return set_error(error, "ArduinoJson allocation failed for map value");
        }

        if (!decode_value(&map_it, value_slot, error, static_cast<uint8_t>(depth + 1)))
        {
            return false;
        }
    }

    err = cbor_value_leave_container(it, &map_it);
    if (err != CborNoError)
    {
        return set_error(error, err, "failed to leave CBOR map");
    }

    return true;
}

inline bool decode_bytes(CborValue *it, JsonVariant dst, std::string *error)
{
    uint8_t *tmp = nullptr;
    size_t len = 0;
    CborValue next;
    CborError err = cbor_value_dup_byte_string(it, &tmp, &len, &next);
    if (err != CborNoError)
    {
        return set_error(error, err, "failed to duplicate byte string");
    }

    JsonArray arr = dst.to<JsonArray>();
    for (size_t i = 0; i < len; ++i)
    {
        arr.add(tmp[i]);
    }

    std::free(tmp);
    *it = next;
    return true;
}

inline bool decode_value(CborValue *it, JsonVariant dst, std::string *error, uint8_t depth)
{
    CborType t = cbor_value_get_type(it);

    if (t == CborMapType)
    {
        return decode_map(it, dst, error, depth);
    }
    if (t == CborArrayType)
    {
        return decode_array(it, dst, error, depth);
    }
    if (t == CborTextStringType)
    {
        std::string s;
        if (!copy_text_string(it, s, error))
        {
            return false;
        }
        dst.set(s);
        return true;
    }
    if (t == CborByteStringType)
    {
        return decode_bytes(it, dst, error);
    }
    if (t == CborIntegerType)
    {
        if (cbor_value_is_unsigned_integer(it))
        {
            uint64_t v = 0;
            CborError err = cbor_value_get_uint64(it, &v);
            if (err != CborNoError)
            {
                return set_error(error, err, "failed to decode unsigned integer");
            }
            dst.set(v);
        }
        else
        {
            int64_t v = 0;
            CborError err = cbor_value_get_int64(it, &v);
            if (err != CborNoError)
            {
                return set_error(error, err, "failed to decode signed integer");
            }
            dst.set(v);
        }

        CborError err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance after integer");
        }
        return true;
    }
    if (t == CborBooleanType)
    {
        bool b = false;
        CborError err = cbor_value_get_boolean(it, &b);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to decode boolean");
        }
        dst.set(b);
        err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance after boolean");
        }
        return true;
    }
    if (t == CborNullType || t == CborUndefinedType)
    {
        dst.set(nullptr);
        CborError err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance after null/undefined");
        }
        return true;
    }
    if (t == CborTagType)
    {
        CborError err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance over tag");
        }
        return decode_value(it, dst, error, depth);
    }
    if (t == CborSimpleType)
    {
        uint8_t v = 0;
        CborError err = cbor_value_get_simple_type(it, &v);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to decode simple type");
        }
        dst.set(v);
        err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance after simple type");
        }
        return true;
    }
    if (t == CborHalfFloatType || t == CborFloatType || t == CborDoubleType)
    {
        double d = 0.0;
        CborError err = cbor_value_get_double(it, &d);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to decode floating-point value");
        }
        dst.set(d);
        err = cbor_value_advance(it);
        if (err != CborNoError)
        {
            return set_error(error, err, "failed to advance after floating-point value");
        }
        return true;
    }

    return set_error(error, "unsupported CBOR type");
}

} // namespace detail

inline bool decode_cbor_value_to_json_object(CborValue *decoder, JsonObject out, std::string *error = nullptr)
{
    if (decoder == nullptr)
    {
        return detail::set_error(error, "decoder is null");
    }

    JsonVariant root = out;
    if (cbor_value_get_type(decoder) != CborMapType)
    {
        return detail::set_error(error, "top-level CBOR item is not a map");
    }

    return detail::decode_map(decoder, root, error, 0);
}

inline bool decode_cbor_to_json_object(const uint8_t *data,
                                       size_t size,
                                       JsonObject out,
                                       std::string *error = nullptr)
{
    if (data == nullptr || size == 0)
    {
        return detail::set_error(error, "input buffer is empty");
    }

    CborParser parser;
    CborValue it;
    CborError err = cbor_parser_init(data, size, 0, &parser, &it);
    if (err != CborNoError)
    {
        return detail::set_error(error, err, "failed to initialize CBOR parser");
    }

    bool ok = decode_cbor_value_to_json_object(&it, out, error);
    if (!ok)
    {
        return false;
    }

    if (!cbor_value_at_end(&it))
    {
        return detail::set_error(error, "trailing CBOR data after top-level map");
    }

    return true;
}

} // namespace cbor_json

#endif // CBOR_JSON_DECODER_H
