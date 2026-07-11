// Msg.hpp — C++17, header-only
#pragma once
#include <string_view>
#include <vector>
#include <functional>
#include <type_traits>
#include <optional>
#include <result.h>
#include <stdint.h>
#include <util.h>

using Bytes = std::vector<uint8_t>;

typedef uint32_t MsgId;
// ————————————————————————————————————————
// 1. Non-virtual base (for polymorphic use)
// ————————————————————————————————————————
struct Msg {
    virtual ~Msg() = default;
    // Runtime ID (fast)
    public:
    virtual MsgId type_id() const = 0;
    virtual const char* type_name() const = 0;

    // Functional dispatch
    template<typename T, typename F>
    void handle(F&& f) const {
        if (type_id() == T::id_value) {
            f(static_cast<const T&>(*this));
        }
    }

protected:
    template<typename T> struct Derived;
};

#define MSG(Name) \
public: \
    static constexpr const char* name_value = #Name; \
    static constexpr MsgId id_value = fnv32(FNV32_OFFSET, #Name); \
    inline MsgId type_id() const noexcept override { return id_value; }; \
    inline const char* type_name() const noexcept override { return name_value; }; \
    ~Name() override = default; 

#define DEFINE_MSG(Name,...) \
struct Name : public Msg { \
    MSG(Name) \
    __VA_ARGS__; \
    Name() = default; \
};
/*
#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct UdpMessage {
    #[cbor(key = 0)]
    pub src: Option<u32>,
    #[cbor(key = 1)]
    pub dst: Option<u32>,
    #[cbor(key = 2)]
    pub msg_type: Option<u32>,
    #[cbor(key = 3)]
    pub req_id: Option<u32>,
    #[cbor(key = 4)]
    pub payload: Option<Vec<u8>>,
}


impl Msg<EndpointAnnounce> for EndpointAnnounce {
    fn id() -> u32 {
        2371693343
    }
    fn name() -> &'static str {
        "EndpointAnnounce"
    }
}

#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct EndpointAnnounce {
    #[cbor(key = 0)]
    pub endpoint_id: Option<u32>,
    #[cbor(key = 1)]
    pub endpoint_name: Option<String>,
}


impl Msg<EndpointAnnounceReply> for EndpointAnnounceReply {
    fn id() -> u32 {
        3238220441
    }
    fn name() -> &'static str {
        "EndpointAnnounceReply"
    }
}

#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct EndpointAnnounceReply {
    #[cbor(key = 0)]
    pub broker_id: Option<u32>,
}*/

struct UdpMessage : public Msg {
    MSG(UdpMessage)
    std::optional<uint32_t> src;
    std::optional<uint32_t> dst;
    std::optional<uint32_t> msg_type;
    std::optional<uint32_t> req_id;
    std::optional<Bytes> payload;
};

struct EndpointAnnounce : public Msg {
    MSG(EndpointAnnounce)
    std::optional<uint32_t> endpoint_id;
    std::optional<std::string> endpoint_name;
};

struct EndpointAnnounceReply : public Msg {
    MSG(EndpointAnnounceReply)
    std::optional<uint32_t> broker_id;
};

