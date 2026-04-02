#ifndef VALUE_JSON_H
#define VALUE_JSON_H

#include "poly.h"

#include <ArduinoJson.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace poly
{
namespace json
{

using Bytes = std::vector<uint8_t>;

inline bool set_error(std::string *error, const std::string &message)
{
    if (error != nullptr)
    {
        *error = message;
    }
    return false;
}

inline bool serialize(const Poly &value, std::string &out, std::string *error = nullptr)
{
    JsonDocument doc;
    JsonVariant root = doc.to<JsonVariant>();
    value.to_json(root);

    out.clear();
    const size_t written = serializeJson(doc, out);
    if (written == 0 && !value.is_null())
    {
        return set_error(error, "failed to serialize Value to JSON string");
    }

    return true;
}

inline bool serialize(const Poly &value, Bytes &out, std::string *error = nullptr)
{
    std::string json;
    if (!serialize(value, json, error))
    {
        return false;
    }

    out.assign(json.begin(), json.end());
    return true;
}

inline bool deserialize(std::string_view json_text, Poly &out, std::string *error = nullptr)
{
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, json_text.data(), json_text.size());
    if (err != DeserializationError::Ok)
    {
        return set_error(error, std::string("failed to parse JSON: ") + err.c_str());
    }

    out.from_json(doc.as<JsonVariantConst>());
    return true;
}

inline bool deserialize(const uint8_t *data, size_t size, Poly &out, std::string *error = nullptr)
{
    if (data == nullptr)
    {
        return set_error(error, "input buffer is null");
    }

    return deserialize(std::string_view(reinterpret_cast<const char *>(data), size), out, error);
}

inline bool deserialize(const Bytes &data, Poly &out, std::string *error = nullptr)
{
    if (data.empty())
    {
        return set_error(error, "input buffer is empty");
    }

    return deserialize(data.data(), data.size(), out, error);
}

inline bool deserialize(const std::string &json_text, Poly &out, std::string *error = nullptr)
{
    return deserialize(std::string_view(json_text), out, error);
}

inline std::string serialize_or_empty(const Poly &value)
{
    std::string out;
    (void)serialize(value, out, nullptr);
    return out;
}

inline Poly deserialize_or_null(std::string_view json_text)
{
    Poly out;
    (void)deserialize(json_text, out, nullptr);
    return out;
}

} // namespace json
} // namespace poly

#endif // VALUE_JSON_H
