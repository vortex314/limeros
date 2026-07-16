// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `std::optional<T>`; missing fields serialize as CBOR null.
// Messages are encoded as CBOR maps keyed by field id.
// Uses tinycbor for CBOR encoding/decoding.






#pragma once

#include <codec.h>



class  BrokerSubscribeRequest : public Serializeable {
public:
    static uint32_t id() { return FNV("BrokerSubscribeRequest"); };
    static const char* name() { return "BrokerSubscribeRequest"; };
    typedef enum FieldId {
        SRC = 0,
        MSG_TYPE = 1,
    } FieldId;
    std::optional<uint32_t> src;
    std::optional<uint32_t> msg_type;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (src.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(src);
        }
        if (msg_type.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_uint32(msg_type);
        }

        return encoder.end_map();
    }

    /// Deserialize a BrokerSubscribeRequest from a CBOR map value.
    static Result<BrokerSubscribeRequest> decode(FrameDecoder& decoder) {
        BrokerSubscribeRequest msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<BrokerSubscribeRequest>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<BrokerSubscribeRequest>::Err("Failed to decode map key");
            }

            switch (key.value()) {
                case 0:
                    
                    msgs_detail::decodeOptionalUInt32(map, msg.src);
                    break;
                case 1:
                    
                    msgs_detail::decodeOptionalUInt32(map, msg.msg_type);
                    break;
                default:
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  CompassEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("CompassEvent"); };
    static const char* name() { return "CompassEvent"; };
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
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (heading.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_float(heading);
        }
        if (pitch.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_float(pitch);
        }
        if (roll.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_float(roll);
        }
        if (mag_x.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_float(mag_x);
        }
        if (mag_y.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_float(mag_y);
        }
        if (mag_z.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_float(mag_z);
        }
        if (accel_x.has_value()) {
            encoder.encode_uint32(6);
            
            encoder.encode_float(accel_x);
        }
        if (accel_y.has_value()) {
            encoder.encode_uint32(7);
            
            encoder.encode_float(accel_y);
        }
        if (accel_z.has_value()) {
            encoder.encode_uint32(8);
            
            encoder.encode_float(accel_z);
        }

        return encoder.end_map();
    }

    /// Deserialize a CompassEvent from a CBOR map value.
    static Result<CompassEvent> decode(FrameDecoder& decoder) {
        CompassEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<CompassEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<CompassEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  DeviceAliveEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("DeviceAliveEvent"); };
    static const char* name() { return "DeviceAliveEvent"; };
    typedef enum FieldId {
        DEVICE = 0,
        ENDPOINT = 1,
        TIMESTAMP = 2,
    } FieldId;
    std::optional<std::string> device;
    std::optional<std::string> endpoint;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (device.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_str(device);
        }
        if (endpoint.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_str(endpoint);
        }
        if (timestamp.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_uint64(timestamp);
        }

        return encoder.end_map();
    }

    /// Deserialize a DeviceAliveEvent from a CBOR map value.
    static Result<DeviceAliveEvent> decode(FrameDecoder& decoder) {
        DeviceAliveEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<DeviceAliveEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<DeviceAliveEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  EndpointAnnounce : public Serializeable {
public:
    static uint32_t id() { return FNV("EndpointAnnounce"); };
    static const char* name() { return "EndpointAnnounce"; };
    typedef enum FieldId {
        ID = 0,
        NAME = 1,
        SERVICES = 2,
        EVENTS = 3,
        REPLIES = 4,
        SUBSCRIBES = 5,
    } FieldId;
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
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(id);
        }
        if (name.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_str(name);
        }
        if (services.has_value()) {
            encoder.encode_uint32(2);
            
        }
        if (events.has_value()) {
            encoder.encode_uint32(3);
            
        }
        if (replies.has_value()) {
            encoder.encode_uint32(4);
            
        }
        if (subscribes.has_value()) {
            encoder.encode_uint32(5);
            
        }

        return encoder.end_map();
    }

    /// Deserialize a EndpointAnnounce from a CBOR map value.
    static Result<EndpointAnnounce> decode(FrameDecoder& decoder) {
        EndpointAnnounce msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<EndpointAnnounce>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<EndpointAnnounce>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  EndpointAnnounceReply : public Serializeable {
public:
    static uint32_t id() { return FNV("EndpointAnnounceReply"); };
    static const char* name() { return "EndpointAnnounceReply"; };
    typedef enum FieldId {
        UTC = 0,
    } FieldId;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> utc;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (utc.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint64(utc);
        }

        return encoder.end_map();
    }

    /// Deserialize a EndpointAnnounceReply from a CBOR map value.
    static Result<EndpointAnnounceReply> decode(FrameDecoder& decoder) {
        EndpointAnnounceReply msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<EndpointAnnounceReply>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<EndpointAnnounceReply>::Err("Failed to decode map key");
            }

            switch (key.value()) {
                case 0:
                    
                    msgs_detail::decodeOptionalUInt64(map, msg.utc);
                    break;
                default:
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  Envelope : public Serializeable {
public:
    static uint32_t id() { return FNV("Envelope"); };
    static const char* name() { return "Envelope"; };
    typedef enum FieldId {
        SRC = 0,
        DST = 1,
        MSG_TYPE = 2,
        REQUEST_ID = 3,
        INSTANCE_ID = 4,
        PAYLOAD = 5,
    } FieldId;
    /// Source endpoint name
    std::optional<uint32_t> src;
    /// Destination endpoint name
    std::optional<uint32_t> dst;
    /// Message type name
    std::optional<uint32_t> msg_type;
    /// Request ID for matching request/reply
    std::optional<uint32_t> request_id;
    /// Instance ID for matching request/reply
    std::optional<uint32_t> instance_id;
    /// Serialized payload of the message
    std::optional<std::vector<uint8_t>> payload;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (src.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(src);
        }
        if (dst.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_uint32(dst);
        }
        if (msg_type.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_uint32(msg_type);
        }
        if (request_id.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_uint32(request_id);
        }
        if (instance_id.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_uint32(instance_id);
        }
        if (payload.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_bstr(&payload);
        }

        return encoder.end_map();
    }

    /// Deserialize a Envelope from a CBOR map value.
    static Result<Envelope> decode(FrameDecoder& decoder) {
        Envelope msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<Envelope>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<Envelope>::Err("Failed to decode map key");
            }

            switch (key.value()) {
                case 0:
                    
                    msgs_detail::decodeOptionalUInt32(map, msg.src);
                    break;
                case 1:
                    
                    msgs_detail::decodeOptionalUInt32(map, msg.dst);
                    break;
                case 2:
                    
                    msgs_detail::decodeOptionalUInt32(map, msg.msg_type);
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  GenericReply : public Serializeable {
public:
    static uint32_t id() { return FNV("GenericReply"); };
    static const char* name() { return "GenericReply"; };
    typedef enum FieldId {
        REQ_ID = 0,
        ERROR_CODE = 1,
        MESSAGE = 2,
        MSG_TYPE = 3,
    } FieldId;
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    /// Error code, 0 if no error
    std::optional<uint32_t> error_code;
    /// Error message or additional information
    std::optional<std::string> message;
    /// Message type identifier , the original request
    std::optional<uint32_t> msg_type;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (req_id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(req_id);
        }
        if (error_code.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_uint32(error_code);
        }
        if (message.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_str(message);
        }
        if (msg_type.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_uint32(msg_type);
        }

        return encoder.end_map();
    }

    /// Deserialize a GenericReply from a CBOR map value.
    static Result<GenericReply> decode(FrameDecoder& decoder) {
        GenericReply msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<GenericReply>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<GenericReply>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  HeatingEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("HeatingEvent"); };
    static const char* name() { return "HeatingEvent"; };
    typedef enum FieldId {
        TEMPERATURE = 0,
        SETPOINT = 1,
        HEATING = 2,
    } FieldId;
    /// Current temperature in Celsius
    std::optional<float> temperature;
    /// Setpoint temperature in Celsius
    std::optional<float> setpoint;
    /// Heating status
    std::optional<bool> heating;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (temperature.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_float(temperature);
        }
        if (setpoint.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_float(setpoint);
        }
        if (heating.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_bool(heating);
        }

        return encoder.end_map();
    }

    /// Deserialize a HeatingEvent from a CBOR map value.
    static Result<HeatingEvent> decode(FrameDecoder& decoder) {
        HeatingEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<HeatingEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<HeatingEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  HoverboardEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("HoverboardEvent"); };
    static const char* name() { return "HoverboardEvent"; };
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
    std::optional<int32_t> dc_curr;
    /// Right DC Link current A *100
    std::optional<int32_t> rdc_curr;
    /// Left DC Link current A *100
    std::optional<int32_t> ldc_curr;
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
    std::optional<int32_t> batv;
    /// Calibrated Temperature C *10
    std::optional<int32_t> temp;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (ctrl_mod.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_int32(ctrl_mod);
        }
        if (ctrl_typ.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_int32(ctrl_typ);
        }
        if (cur_mot_max.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_int32(cur_mot_max);
        }
        if (rpm_mot_max.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_int32(rpm_mot_max);
        }
        if (fi_weak_ena.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_int32(fi_weak_ena);
        }
        if (fi_weak_hi.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_int32(fi_weak_hi);
        }
        if (fi_weak_lo.has_value()) {
            encoder.encode_uint32(6);
            
            encoder.encode_int32(fi_weak_lo);
        }
        if (fi_weak_max.has_value()) {
            encoder.encode_uint32(7);
            
            encoder.encode_int32(fi_weak_max);
        }
        if (phase_adv_max_deg.has_value()) {
            encoder.encode_uint32(8);
            
            encoder.encode_int32(phase_adv_max_deg);
        }
        if (input1_raw.has_value()) {
            encoder.encode_uint32(9);
            
            encoder.encode_int32(input1_raw);
        }
        if (input1_typ.has_value()) {
            encoder.encode_uint32(10);
            
            encoder.encode_int32(input1_typ);
        }
        if (input1_min.has_value()) {
            encoder.encode_uint32(11);
            
            encoder.encode_int32(input1_min);
        }
        if (input1_mid.has_value()) {
            encoder.encode_uint32(12);
            
            encoder.encode_int32(input1_mid);
        }
        if (input1_max.has_value()) {
            encoder.encode_uint32(13);
            
            encoder.encode_int32(input1_max);
        }
        if (input1_cmd.has_value()) {
            encoder.encode_uint32(14);
            
            encoder.encode_int32(input1_cmd);
        }
        if (input2_raw.has_value()) {
            encoder.encode_uint32(15);
            
            encoder.encode_int32(input2_raw);
        }
        if (input2_typ.has_value()) {
            encoder.encode_uint32(16);
            
            encoder.encode_int32(input2_typ);
        }
        if (input2_min.has_value()) {
            encoder.encode_uint32(17);
            
            encoder.encode_int32(input2_min);
        }
        if (input2_mid.has_value()) {
            encoder.encode_uint32(18);
            
            encoder.encode_int32(input2_mid);
        }
        if (input2_max.has_value()) {
            encoder.encode_uint32(19);
            
            encoder.encode_int32(input2_max);
        }
        if (input2_cmd.has_value()) {
            encoder.encode_uint32(20);
            
            encoder.encode_int32(input2_cmd);
        }
        if (aux_input1_raw.has_value()) {
            encoder.encode_uint32(21);
            
            encoder.encode_int32(aux_input1_raw);
        }
        if (aux_input1_typ.has_value()) {
            encoder.encode_uint32(22);
            
            encoder.encode_int32(aux_input1_typ);
        }
        if (aux_input1_min.has_value()) {
            encoder.encode_uint32(23);
            
            encoder.encode_int32(aux_input1_min);
        }
        if (aux_input1_mid.has_value()) {
            encoder.encode_uint32(24);
            
            encoder.encode_int32(aux_input1_mid);
        }
        if (aux_input1_max.has_value()) {
            encoder.encode_uint32(25);
            
            encoder.encode_int32(aux_input1_max);
        }
        if (aux_input1_cmd.has_value()) {
            encoder.encode_uint32(26);
            
            encoder.encode_int32(aux_input1_cmd);
        }
        if (aux_input2_raw.has_value()) {
            encoder.encode_uint32(27);
            
            encoder.encode_int32(aux_input2_raw);
        }
        if (aux_input2_typ.has_value()) {
            encoder.encode_uint32(28);
            
            encoder.encode_int32(aux_input2_typ);
        }
        if (aux_input2_min.has_value()) {
            encoder.encode_uint32(29);
            
            encoder.encode_int32(aux_input2_min);
        }
        if (aux_input2_mid.has_value()) {
            encoder.encode_uint32(30);
            
            encoder.encode_int32(aux_input2_mid);
        }
        if (aux_input2_max.has_value()) {
            encoder.encode_uint32(31);
            
            encoder.encode_int32(aux_input2_max);
        }
        if (aux_input2_cmd.has_value()) {
            encoder.encode_uint32(32);
            
            encoder.encode_int32(aux_input2_cmd);
        }
        if (dc_curr.has_value()) {
            encoder.encode_uint32(33);
            
            encoder.encode_int32(dc_curr);
        }
        if (rdc_curr.has_value()) {
            encoder.encode_uint32(34);
            
            encoder.encode_int32(rdc_curr);
        }
        if (ldc_curr.has_value()) {
            encoder.encode_uint32(35);
            
            encoder.encode_int32(ldc_curr);
        }
        if (cmdl.has_value()) {
            encoder.encode_uint32(36);
            
            encoder.encode_int32(cmdl);
        }
        if (cmdr.has_value()) {
            encoder.encode_uint32(37);
            
            encoder.encode_int32(cmdr);
        }
        if (spd_avg.has_value()) {
            encoder.encode_uint32(38);
            
            encoder.encode_int32(spd_avg);
        }
        if (spdl.has_value()) {
            encoder.encode_uint32(39);
            
            encoder.encode_int32(spdl);
        }
        if (spdr.has_value()) {
            encoder.encode_uint32(40);
            
            encoder.encode_int32(spdr);
        }
        if (filter_rate.has_value()) {
            encoder.encode_uint32(41);
            
            encoder.encode_int32(filter_rate);
        }
        if (spd_coef.has_value()) {
            encoder.encode_uint32(42);
            
            encoder.encode_int32(spd_coef);
        }
        if (str_coef.has_value()) {
            encoder.encode_uint32(43);
            
            encoder.encode_int32(str_coef);
        }
        if (batv.has_value()) {
            encoder.encode_uint32(44);
            
            encoder.encode_int32(batv);
        }
        if (temp.has_value()) {
            encoder.encode_uint32(45);
            
            encoder.encode_int32(temp);
        }

        return encoder.end_map();
    }

    /// Deserialize a HoverboardEvent from a CBOR map value.
    static Result<HoverboardEvent> decode(FrameDecoder& decoder) {
        HoverboardEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<HoverboardEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<HoverboardEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    
                    msgs_detail::decodeOptionalInt32(map, msg.dc_curr);
                    break;
                case 34:
                    
                    msgs_detail::decodeOptionalInt32(map, msg.rdc_curr);
                    break;
                case 35:
                    
                    msgs_detail::decodeOptionalInt32(map, msg.ldc_curr);
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
                    
                    msgs_detail::decodeOptionalInt32(map, msg.batv);
                    break;
                case 45:
                    
                    msgs_detail::decodeOptionalInt32(map, msg.temp);
                    break;
                default:
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  HoverboardRequest : public Serializeable {
public:
    static uint32_t id() { return FNV("HoverboardRequest"); };
    static const char* name() { return "HoverboardRequest"; };
    typedef enum FieldId {
        REQ_ID = 0,
        SPEED = 1,
        STEER = 2,
    } FieldId;
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    /// Speed command for the hoverboard
    std::optional<int32_t> speed;
    /// Steering command for the hoverboard
    std::optional<int32_t> steer;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (req_id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(req_id);
        }
        if (speed.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_int32(speed);
        }
        if (steer.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_int32(steer);
        }

        return encoder.end_map();
    }

    /// Deserialize a HoverboardRequest from a CBOR map value.
    static Result<HoverboardRequest> decode(FrameDecoder& decoder) {
        HoverboardRequest msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<HoverboardRequest>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<HoverboardRequest>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  ImuEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("ImuEvent"); };
    static const char* name() { return "ImuEvent"; };
    typedef enum FieldId {
        GYRO_X = 0,
        GYRO_Y = 1,
        GYRO_Z = 2,
        ACCEL_X = 3,
        ACCEL_Y = 4,
        ACCEL_Z = 5,
    } FieldId;
    /// Gyroscope X axis in deg/s
    std::optional<float> gyro_x;
    /// Gyroscope Y axis in deg/s
    std::optional<float> gyro_y;
    /// Gyroscope Z axis in deg/s
    std::optional<float> gyro_z;
    /// Accelerometer X axis in m/s^2
    std::optional<float> accel_x;
    /// Accelerometer Y axis in m/s^2
    std::optional<float> accel_y;
    /// Accelerometer Z axis in m/s^2
    std::optional<float> accel_z;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (gyro_x.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_float(gyro_x);
        }
        if (gyro_y.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_float(gyro_y);
        }
        if (gyro_z.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_float(gyro_z);
        }
        if (accel_x.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_float(accel_x);
        }
        if (accel_y.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_float(accel_y);
        }
        if (accel_z.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_float(accel_z);
        }

        return encoder.end_map();
    }

    /// Deserialize a ImuEvent from a CBOR map value.
    static Result<ImuEvent> decode(FrameDecoder& decoder) {
        ImuEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<ImuEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<ImuEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
                case 0:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.gyro_x);
                    break;
                case 1:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.gyro_y);
                    break;
                case 2:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.gyro_z);
                    break;
                case 3:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.accel_x);
                    break;
                case 4:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.accel_y);
                    break;
                case 5:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.accel_z);
                    break;
                default:
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  Max31855Event : public Serializeable {
public:
    static uint32_t id() { return FNV("Max31855Event"); };
    static const char* name() { return "Max31855Event"; };
    typedef enum FieldId {
        THERMOCOUPLE_TEMP = 0,
        INTERNAL_TEMP = 1,
        FAULT = 2,
        FAULT_SHORT_VCC = 3,
        FAULT_SHORT_GND = 4,
        FAULT_OPEN_TC = 5,
    } FieldId;
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
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (thermocouple_temp.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_float(thermocouple_temp);
        }
        if (internal_temp.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_float(internal_temp);
        }
        if (fault.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_bool(fault);
        }
        if (fault_short_vcc.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_bool(fault_short_vcc);
        }
        if (fault_short_gnd.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_bool(fault_short_gnd);
        }
        if (fault_open_tc.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_bool(fault_open_tc);
        }

        return encoder.end_map();
    }

    /// Deserialize a Max31855Event from a CBOR map value.
    static Result<Max31855Event> decode(FrameDecoder& decoder) {
        Max31855Event msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<Max31855Event>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<Max31855Event>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  PingReply : public Serializeable {
public:
    static uint32_t id() { return FNV("PingReply"); };
    static const char* name() { return "PingReply"; };
    typedef enum FieldId {
        REQ_ID = 0,
        TIMESTAMP = 1,
    } FieldId;
    std::optional<uint32_t> req_id;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (req_id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(req_id);
        }
        if (timestamp.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_uint64(timestamp);
        }

        return encoder.end_map();
    }

    /// Deserialize a PingReply from a CBOR map value.
    static Result<PingReply> decode(FrameDecoder& decoder) {
        PingReply msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<PingReply>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<PingReply>::Err("Failed to decode map key");
            }

            switch (key.value()) {
                case 0:
                    
                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                    break;
                case 1:
                    
                    msgs_detail::decodeOptionalUInt64(map, msg.timestamp);
                    break;
                default:
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  PingRequest : public Serializeable {
public:
    static uint32_t id() { return FNV("PingRequest"); };
    static const char* name() { return "PingRequest"; };
    typedef enum FieldId {
        REQ_ID = 0,
        TIMESTAMP = 1,
    } FieldId;
    std::optional<uint32_t> req_id;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (req_id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(req_id);
        }
        if (timestamp.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_uint64(timestamp);
        }

        return encoder.end_map();
    }

    /// Deserialize a PingRequest from a CBOR map value.
    static Result<PingRequest> decode(FrameDecoder& decoder) {
        PingRequest msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<PingRequest>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<PingRequest>::Err("Failed to decode map key");
            }

            switch (key.value()) {
                case 0:
                    
                    msgs_detail::decodeOptionalUInt32(map, msg.req_id);
                    break;
                case 1:
                    
                    msgs_detail::decodeOptionalUInt64(map, msg.timestamp);
                    break;
                default:
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  Ps4Event : public Serializeable {
public:
    static uint32_t id() { return FNV("Ps4Event"); };
    static const char* name() { return "Ps4Event"; };
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
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (button_left.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_bool(button_left);
        }
        if (button_right.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_bool(button_right);
        }
        if (button_up.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_bool(button_up);
        }
        if (button_down.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_bool(button_down);
        }
        if (button_square.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_bool(button_square);
        }
        if (button_cross.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_bool(button_cross);
        }
        if (button_circle.has_value()) {
            encoder.encode_uint32(6);
            
            encoder.encode_bool(button_circle);
        }
        if (button_triangle.has_value()) {
            encoder.encode_uint32(7);
            
            encoder.encode_bool(button_triangle);
        }
        if (button_left_shoulder.has_value()) {
            encoder.encode_uint32(8);
            
            encoder.encode_bool(button_left_shoulder);
        }
        if (button_right_shoulder.has_value()) {
            encoder.encode_uint32(9);
            
            encoder.encode_bool(button_right_shoulder);
        }
        if (button_left_trigger.has_value()) {
            encoder.encode_uint32(10);
            
            encoder.encode_bool(button_left_trigger);
        }
        if (button_right_trigger.has_value()) {
            encoder.encode_uint32(11);
            
            encoder.encode_bool(button_right_trigger);
        }
        if (button_left_joystick.has_value()) {
            encoder.encode_uint32(12);
            
            encoder.encode_bool(button_left_joystick);
        }
        if (button_right_joystick.has_value()) {
            encoder.encode_uint32(13);
            
            encoder.encode_bool(button_right_joystick);
        }
        if (button_share.has_value()) {
            encoder.encode_uint32(14);
            
            encoder.encode_bool(button_share);
        }
        if (button_options.has_value()) {
            encoder.encode_uint32(15);
            
            encoder.encode_bool(button_options);
        }
        if (button_touchpad.has_value()) {
            encoder.encode_uint32(16);
            
            encoder.encode_bool(button_touchpad);
        }
        if (button_ps.has_value()) {
            encoder.encode_uint32(17);
            
            encoder.encode_bool(button_ps);
        }
        if (axis_lx.has_value()) {
            encoder.encode_uint32(18);
            
            encoder.encode_int32(axis_lx);
        }
        if (axis_ly.has_value()) {
            encoder.encode_uint32(19);
            
            encoder.encode_int32(axis_ly);
        }
        if (axis_rx.has_value()) {
            encoder.encode_uint32(20);
            
            encoder.encode_int32(axis_rx);
        }
        if (axis_ry.has_value()) {
            encoder.encode_uint32(21);
            
            encoder.encode_int32(axis_ry);
        }
        if (gyro_x.has_value()) {
            encoder.encode_uint32(22);
            
            encoder.encode_int32(gyro_x);
        }
        if (gyro_y.has_value()) {
            encoder.encode_uint32(23);
            
            encoder.encode_int32(gyro_y);
        }
        if (gyro_z.has_value()) {
            encoder.encode_uint32(24);
            
            encoder.encode_int32(gyro_z);
        }
        if (accel_x.has_value()) {
            encoder.encode_uint32(25);
            
            encoder.encode_int32(accel_x);
        }
        if (accel_y.has_value()) {
            encoder.encode_uint32(26);
            
            encoder.encode_int32(accel_y);
        }
        if (accel_z.has_value()) {
            encoder.encode_uint32(27);
            
            encoder.encode_int32(accel_z);
        }
        if (connected.has_value()) {
            encoder.encode_uint32(28);
            
            encoder.encode_bool(connected);
        }
        if (battery_level.has_value()) {
            encoder.encode_uint32(29);
            
            encoder.encode_int32(battery_level);
        }
        if (bluetooth.has_value()) {
            encoder.encode_uint32(30);
            
            encoder.encode_bool(bluetooth);
        }
        if (debug.has_value()) {
            encoder.encode_uint32(31);
            
            encoder.encode_str(debug);
        }
        if (temp.has_value()) {
            encoder.encode_uint32(32);
            
            encoder.encode_int32(temp);
        }

        return encoder.end_map();
    }

    /// Deserialize a Ps4Event from a CBOR map value.
    static Result<Ps4Event> decode(FrameDecoder& decoder) {
        Ps4Event msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<Ps4Event>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<Ps4Event>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  Ps4Request : public Serializeable {
public:
    static uint32_t id() { return FNV("Ps4Request"); };
    static const char* name() { return "Ps4Request"; };
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
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (req_id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(req_id);
        }
        if (rumble_small.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_int32(rumble_small);
        }
        if (rumble_large.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_int32(rumble_large);
        }
        if (led_red.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_int32(led_red);
        }
        if (led_green.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_int32(led_green);
        }
        if (led_blue.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_int32(led_blue);
        }
        if (led_flash_on.has_value()) {
            encoder.encode_uint32(6);
            
            encoder.encode_int32(led_flash_on);
        }
        if (led_flash_off.has_value()) {
            encoder.encode_uint32(7);
            
            encoder.encode_int32(led_flash_off);
        }

        return encoder.end_map();
    }

    /// Deserialize a Ps4Request from a CBOR map value.
    static Result<Ps4Request> decode(FrameDecoder& decoder) {
        Ps4Request msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<Ps4Request>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<Ps4Request>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  SysEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("SysEvent"); };
    static const char* name() { return "SysEvent"; };
    typedef enum FieldId {
        UTC = 0,
        UPTIME = 1,
        FREE_HEAP = 2,
        FLASH = 3,
        CPU_BOARD = 4,
        BUILD_DATE = 5,
    } FieldId;
    std::optional<uint64_t> utc;
    std::optional<uint64_t> uptime;
    std::optional<uint64_t> free_heap;
    std::optional<uint64_t> flash;
    std::optional<std::string> cpu_board;
    std::optional<std::string> build_date;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (utc.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint64(utc);
        }
        if (uptime.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_uint64(uptime);
        }
        if (free_heap.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_uint64(free_heap);
        }
        if (flash.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_uint64(flash);
        }
        if (cpu_board.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_str(cpu_board);
        }
        if (build_date.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_str(build_date);
        }

        return encoder.end_map();
    }

    /// Deserialize a SysEvent from a CBOR map value.
    static Result<SysEvent> decode(FrameDecoder& decoder) {
        SysEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<SysEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<SysEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  SysReply : public Serializeable {
public:
    static uint32_t id() { return FNV("SysReply"); };
    static const char* name() { return "SysReply"; };
    typedef enum FieldId {
        REQ_ID = 0,
        RC = 1,
        MESSAGE = 2,
    } FieldId;
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<int32_t> rc;
    std::optional<std::string> message;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (req_id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(req_id);
        }
        if (rc.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_int32(rc);
        }
        if (message.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_str(message);
        }

        return encoder.end_map();
    }

    /// Deserialize a SysReply from a CBOR map value.
    static Result<SysReply> decode(FrameDecoder& decoder) {
        SysReply msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<SysReply>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<SysReply>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  SysRequest : public Serializeable {
public:
    static uint32_t id() { return FNV("SysRequest"); };
    static const char* name() { return "SysRequest"; };
    typedef enum FieldId {
        REQ_ID = 0,
        SET_TIME = 1,
        REBOOT = 2,
        CONSOLE = 3,
    } FieldId;
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<uint64_t> set_time;
    std::optional<bool> reboot;
    std::optional<std::string> console;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (req_id.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_uint32(req_id);
        }
        if (set_time.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_uint64(set_time);
        }
        if (reboot.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_bool(reboot);
        }
        if (console.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_str(console);
        }

        return encoder.end_map();
    }

    /// Deserialize a SysRequest from a CBOR map value.
    static Result<SysRequest> decode(FrameDecoder& decoder) {
        SysRequest msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<SysRequest>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<SysRequest>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  UsEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("UsEvent"); };
    static const char* name() { return "UsEvent"; };
    typedef enum FieldId {
        DISTANCE = 0,
        TEMPERATURE = 1,
        STATUS = 2,
    } FieldId;
    /// Distance in meters
    std::optional<float> distance;
    /// Temperature in Celsius
    std::optional<float> temperature;
    /// Status code, 0 if no error
    std::optional<int32_t> status;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (distance.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_float(distance);
        }
        if (temperature.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_float(temperature);
        }
        if (status.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_int32(status);
        }

        return encoder.end_map();
    }

    /// Deserialize a UsEvent from a CBOR map value.
    static Result<UsEvent> decode(FrameDecoder& decoder) {
        UsEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<UsEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<UsEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
                case 0:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.distance);
                    break;
                case 1:
                    
                    msgs_detail::decodeOptionalFloat(map, msg.temperature);
                    break;
                case 2:
                    
                    msgs_detail::decodeOptionalInt32(map, msg.status);
                    break;
                default:
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};



class  WifiEvent : public Serializeable {
public:
    static uint32_t id() { return FNV("WifiEvent"); };
    static const char* name() { return "WifiEvent"; };
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
    std::optional<std::string> ip;
    std::optional<std::string> gateway;
    std::optional<std::string> netmask;
    std::optional<std::string> ssid;
    std::optional<std::string> bssid;
    std::optional<int32_t> channel;
    std::optional<int32_t> rssi;
    std::optional<std::string> mac;

    /// Serialize this message into a CBOR map keyed by field id.
    Result<Void> encode(FrameEncoder& encoder) const {
        encoder.begin_map().is_err()) {
            return Result<Void>::Err("Failed to encode map size");
        }
        if (ip.has_value()) {
            encoder.encode_uint32(0);
            
            encoder.encode_str(ip);
        }
        if (gateway.has_value()) {
            encoder.encode_uint32(1);
            
            encoder.encode_str(gateway);
        }
        if (netmask.has_value()) {
            encoder.encode_uint32(2);
            
            encoder.encode_str(netmask);
        }
        if (ssid.has_value()) {
            encoder.encode_uint32(3);
            
            encoder.encode_str(ssid);
        }
        if (bssid.has_value()) {
            encoder.encode_uint32(4);
            
            encoder.encode_str(bssid);
        }
        if (channel.has_value()) {
            encoder.encode_uint32(5);
            
            encoder.encode_int32(channel);
        }
        if (rssi.has_value()) {
            encoder.encode_uint32(6);
            
            encoder.encode_int32(rssi);
        }
        if (mac.has_value()) {
            encoder.encode_uint32(7);
            
            encoder.encode_str(mac);
        }

        return encoder.end_map();
    }

    /// Deserialize a WifiEvent from a CBOR map value.
    static Result<WifiEvent> decode(FrameDecoder& decoder) {
        WifiEvent msg;
        auto map = decoder.begin_map();
        if (map.is_err()) {
            return Result<WifiEvent>::Err("Failed to decode map size");
        }

        for (uint32_t i = 0; i < map.value(); ++i) {
            auto key = decoder.decode_uint32();
            if (key.is_err()) {
                return Result<WifiEvent>::Err("Failed to decode map key");
            }

            switch (key.value()) {
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
                    // Unknown field id; skip the value.
                    decoder.skip_value();
                    break;          

    }
};


