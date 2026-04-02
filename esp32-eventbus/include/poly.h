#ifndef VALUE_H
#define VALUE_H

#include <ArduinoJson.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace poly
{

class Poly
{
public:
    enum class Type
    {
        Null,
        Bool,
        Signed,
        Unsigned,
        Float,
        String,
        Array,
        Object
    };

    using Array = std::vector<Poly>;
    using Object = std::unordered_map<std::string, Poly>;

    Poly() : node_(std::make_unique<NullNode>()) {}
    Poly(std::nullptr_t) : node_(std::make_unique<NullNode>()) {}
    Poly(bool v) : node_(std::make_unique<BoolNode>(v)) {}

    template <typename T,
              typename std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T> && !std::is_same_v<T, bool>, int> = 0>
    Poly(T v) : node_(std::make_unique<SignedNode>(static_cast<int64_t>(v))) {}

    template <typename T,
              typename std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, int> = 0>
    Poly(T v) : node_(std::make_unique<UnsignedNode>(static_cast<uint64_t>(v))) {}

    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    Poly(T v) : node_(std::make_unique<FloatNode>(static_cast<double>(v))) {}

    Poly(const char *v) : node_(std::make_unique<StringNode>(v != nullptr ? v : "")) {}
    Poly(const std::string &v) : node_(std::make_unique<StringNode>(v)) {}
    Poly(std::string &&v) : node_(std::make_unique<StringNode>(std::move(v))) {}
    Poly(const Array &v) : node_(std::make_unique<ArrayNode>(v)) {}
    Poly(Array &&v) : node_(std::make_unique<ArrayNode>(std::move(v))) {}
    Poly(const Object &v) : node_(std::make_unique<ObjectNode>(v)) {}
    Poly(Object &&v) : node_(std::make_unique<ObjectNode>(std::move(v))) {}

    Poly(const Poly &other) : node_(other.node_->clone()) {}
    Poly(Poly &&other) noexcept = default;

    Poly &operator=(const Poly &other)
    {
        if (this != &other)
        {
            node_ = other.node_->clone();
        }
        return *this;
    }

    Poly &operator=(Poly &&other) noexcept = default;

    static Poly array()
    {
        return Poly(Array{});
    }

    static Poly object()
    {
        return Poly(Object{});
    }

    Type type() const
    {
        return node_->type();
    }

    bool is_null() const { return type() == Type::Null; }
    bool is_bool() const { return type() == Type::Bool; }
    bool is_signed() const { return type() == Type::Signed; }
    bool is_unsigned() const { return type() == Type::Unsigned; }
    bool is_float() const { return type() == Type::Float; }
    bool is_number() const { return is_signed() || is_unsigned() || is_float(); }
    bool is_string() const { return type() == Type::String; }
    bool is_array() const { return type() == Type::Array; }
    bool is_object() const { return type() == Type::Object; }

    Poly &operator[](const std::string &key)
    {
        ensure_object();
        return as_object_mut()[key];
    }

    Poly &operator[](const char *key)
    {
        return (*this)[std::string(key != nullptr ? key : "")];
    }

    const Poly &operator[](const std::string &key) const
    {
        if (!is_object())
        {
            return null_ref();
        }
        const auto &obj = as_object();
        auto it = obj.find(key);
        return it != obj.end() ? it->second : null_ref();
    }

    const Poly &operator[](const char *key) const
    {
        return (*this)[std::string(key != nullptr ? key : "")];
    }

    Poly &operator[](size_t index)
    {
        ensure_array();
        auto &arr = as_array_mut();
        if (index >= arr.size())
        {
            arr.resize(index + 1);
        }
        return arr[index];
    }

    const Poly &operator[](size_t index) const
    {
        if (!is_array())
        {
            return null_ref();
        }
        const auto &arr = as_array();
        return index < arr.size() ? arr[index] : null_ref();
    }

    size_t size() const
    {
        if (is_array())
        {
            return as_array().size();
        }
        if (is_object())
        {
            return as_object().size();
        }
        return 0;
    }

    bool contains(const std::string &key) const
    {
        if (!is_object())
        {
            return false;
        }
        const auto &obj = as_object();
        return obj.find(key) != obj.end();
    }

    void push_back(const Poly &v)
    {
        ensure_array();
        as_array_mut().push_back(v);
    }

    void push_back(Poly &&v)
    {
        ensure_array();
        as_array_mut().push_back(std::move(v));
    }

    int64_t as_int64(int64_t fallback = 0) const
    {
        if (is_signed()) return static_cast<const SignedNode *>(node_.get())->value;
        if (is_unsigned()) return static_cast<int64_t>(static_cast<const UnsignedNode *>(node_.get())->value);
        if (is_float()) return static_cast<int64_t>(static_cast<const FloatNode *>(node_.get())->value);
        if (is_bool()) return static_cast<const BoolNode *>(node_.get())->value ? 1 : 0;
        return fallback;
    }

    uint64_t as_uint64(uint64_t fallback = 0) const
    {
        if (is_unsigned()) return static_cast<const UnsignedNode *>(node_.get())->value;
        if (is_signed())
        {
            int64_t v = static_cast<const SignedNode *>(node_.get())->value;
            return v < 0 ? fallback : static_cast<uint64_t>(v);
        }
        if (is_float()) return static_cast<uint64_t>(static_cast<const FloatNode *>(node_.get())->value);
        if (is_bool()) return static_cast<const BoolNode *>(node_.get())->value ? 1U : 0U;
        return fallback;
    }

    double as_double(double fallback = 0.0) const
    {
        if (is_float()) return static_cast<const FloatNode *>(node_.get())->value;
        if (is_signed()) return static_cast<double>(static_cast<const SignedNode *>(node_.get())->value);
        if (is_unsigned()) return static_cast<double>(static_cast<const UnsignedNode *>(node_.get())->value);
        if (is_bool()) return static_cast<const BoolNode *>(node_.get())->value ? 1.0 : 0.0;
        return fallback;
    }

    bool as_bool(bool fallback = false) const
    {
        if (is_bool()) return static_cast<const BoolNode *>(node_.get())->value;
        if (is_signed()) return static_cast<const SignedNode *>(node_.get())->value != 0;
        if (is_unsigned()) return static_cast<const UnsignedNode *>(node_.get())->value != 0;
        if (is_float()) return static_cast<const FloatNode *>(node_.get())->value != 0.0;
        if (is_string()) return !static_cast<const StringNode *>(node_.get())->value.empty();
        if (is_array()) return !as_array().empty();
        if (is_object()) return !as_object().empty();
        return fallback;
    }

    std::string as_string(const std::string &fallback = "") const
    {
        if (is_string()) return static_cast<const StringNode *>(node_.get())->value;
        if (is_bool()) return static_cast<const BoolNode *>(node_.get())->value ? "true" : "false";
        if (is_signed()) return std::to_string(static_cast<const SignedNode *>(node_.get())->value);
        if (is_unsigned()) return std::to_string(static_cast<const UnsignedNode *>(node_.get())->value);
        if (is_float()) return std::to_string(static_cast<const FloatNode *>(node_.get())->value);
        return fallback;
    }

    const Array &as_array() const
    {
        static const Array empty;
        return is_array() ? static_cast<const ArrayNode *>(node_.get())->value : empty;
    }

    const Object &as_object() const
    {
        static const Object empty;
        return is_object() ? static_cast<const ObjectNode *>(node_.get())->value : empty;
    }

    Array &as_array_mut()
    {
        ensure_array();
        return static_cast<ArrayNode *>(node_.get())->value;
    }

    Object &as_object_mut()
    {
        ensure_object();
        return static_cast<ObjectNode *>(node_.get())->value;
    }

    void from_json(JsonVariantConst src)
    {
        if (src.is<JsonObjectConst>())
        {
            Object obj;
            JsonObjectConst in = src.as<JsonObjectConst>();
            for (JsonPairConst kv : in)
            {
                Poly child;
                child.from_json(kv.value());
                obj[std::string(kv.key().c_str())] = std::move(child);
            }
            *this = Poly(std::move(obj));
            return;
        }

        if (src.is<JsonArrayConst>())
        {
            Array arr;
            JsonArrayConst in = src.as<JsonArrayConst>();
            arr.reserve(in.size());
            for (JsonVariantConst item : in)
            {
                Poly child;
                child.from_json(item);
                arr.push_back(std::move(child));
            }
            *this = Poly(std::move(arr));
            return;
        }

        if (src.is<std::nullptr_t>())
        {
            *this = Poly(nullptr);
            return;
        }
        if (src.is<bool>())
        {
            *this = Poly(src.as<bool>());
            return;
        }
        if (src.is<int64_t>())
        {
            *this = Poly(src.as<int64_t>());
            return;
        }
        if (src.is<uint64_t>())
        {
            *this = Poly(src.as<uint64_t>());
            return;
        }
        if (src.is<double>())
        {
            *this = Poly(src.as<double>());
            return;
        }
        if (src.is<const char *>())
        {
            *this = Poly(src.as<const char *>());
            return;
        }

        *this = Poly(nullptr);
    }

    void to_json(JsonVariant dst) const
    {
        switch (type())
        {
            case Type::Null:
                dst.set(nullptr);
                break;
            case Type::Bool:
                dst.set(static_cast<const BoolNode *>(node_.get())->value);
                break;
            case Type::Signed:
                dst.set(static_cast<const SignedNode *>(node_.get())->value);
                break;
            case Type::Unsigned:
                dst.set(static_cast<const UnsignedNode *>(node_.get())->value);
                break;
            case Type::Float:
                dst.set(static_cast<const FloatNode *>(node_.get())->value);
                break;
            case Type::String:
                dst.set(static_cast<const StringNode *>(node_.get())->value);
                break;
            case Type::Array:
            {
                JsonArray out = dst.to<JsonArray>();
                for (const auto &item : static_cast<const ArrayNode *>(node_.get())->value)
                {
                    if (item.is_object())
                    {
                        JsonObject slot = out.add<JsonObject>();
                        item.to_json(slot);
                    }
                    else if (item.is_array())
                    {
                        JsonArray slot = out.add<JsonArray>();
                        item.to_json(slot);
                    }
                    else
                    {
                        JsonVariant slot = out.add<JsonVariant>();
                        item.to_json(slot);
                    }
                }
                break;
            }
            case Type::Object:
            {
                JsonObject out = dst.to<JsonObject>();
                for (const auto &kv : static_cast<const ObjectNode *>(node_.get())->value)
                {
                    if (kv.second.is_object())
                    {
                        JsonObject slot = out[kv.first.c_str()].to<JsonObject>();
                        kv.second.to_json(slot);
                    }
                    else if (kv.second.is_array())
                    {
                        JsonArray slot = out[kv.first.c_str()].to<JsonArray>();
                        kv.second.to_json(slot);
                    }
                    else
                    {
                        JsonVariant slot = out[kv.first.c_str()];
                        kv.second.to_json(slot);
                    }
                }
                break;
            }
        }
    }

private:
    struct Node
    {
        virtual ~Node() = default;
        virtual Type type() const = 0;
        virtual std::unique_ptr<Node> clone() const = 0;
    };

    struct NullNode : Node
    {
        Type type() const override { return Type::Null; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<NullNode>(); }
    };

    struct BoolNode : Node
    {
        explicit BoolNode(bool v) : value(v) {}
        bool value;
        Type type() const override { return Type::Bool; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<BoolNode>(value); }
    };

    struct SignedNode : Node
    {
        explicit SignedNode(int64_t v) : value(v) {}
        int64_t value;
        Type type() const override { return Type::Signed; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<SignedNode>(value); }
    };

    struct UnsignedNode : Node
    {
        explicit UnsignedNode(uint64_t v) : value(v) {}
        uint64_t value;
        Type type() const override { return Type::Unsigned; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<UnsignedNode>(value); }
    };

    struct FloatNode : Node
    {
        explicit FloatNode(double v) : value(v) {}
        double value;
        Type type() const override { return Type::Float; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<FloatNode>(value); }
    };

    struct StringNode : Node
    {
        explicit StringNode(std::string v) : value(std::move(v)) {}
        std::string value;
        Type type() const override { return Type::String; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<StringNode>(value); }
    };

    struct ArrayNode : Node
    {
        explicit ArrayNode(Array v) : value(std::move(v)) {}
        Array value;
        Type type() const override { return Type::Array; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<ArrayNode>(value); }
    };

    struct ObjectNode : Node
    {
        explicit ObjectNode(Object v) : value(std::move(v)) {}
        Object value;
        Type type() const override { return Type::Object; }
        std::unique_ptr<Node> clone() const override { return std::make_unique<ObjectNode>(value); }
    };

    static const Poly &null_ref()
    {
        static const Poly v;
        return v;
    }

    void ensure_array()
    {
        if (!is_array())
        {
            node_ = std::make_unique<ArrayNode>(Array{});
        }
    }

    void ensure_object()
    {
        if (!is_object())
        {
            node_ = std::make_unique<ObjectNode>(Object{});
        }
    }

    std::unique_ptr<Node> node_;
};

} // namespace poly

#endif // VALUE_H
