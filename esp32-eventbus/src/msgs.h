#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <optional>
#include <ArduinoJson.h>
#include <cbor.h>
#include <msg.h>
#include <serdes.h>
#include <poly.h>

typedef std::vector<uint8_t> Bytes;
typedef JsonVariant Value;
typedef poly::Poly Poly;


typedef enum {
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
    ALERT = 6,
} LogLevel;

typedef enum {
    SYS_CMD = 1,
    SYS_INFO = 2,
    WIFI_INFO = 3,
    MOTOR_INFO = 4,
    MOTOR_CMD = 5,
} MessageType;

typedef enum {
    OFF = 0,
    ON = 1,
} Toggle;

typedef enum {
    VOLTAGE = 1,
    SPEED = 2,
    TORQUE = 3,
} CtrlMod;

typedef enum {
    COMMUTATION = 0,
    SINUSOIDAL = 1,
    FOC = 2,
} CtrlTyp;

typedef enum {
    DISABLED = 0,
    NORMAL_POT = 1,
    MIDDLE_RESTING_POT = 2,
    AUTO_DETECT = 3,
} InTyp;

typedef enum {
    MANUAL = 0,
    AUTO = 1,
    PAUSED = 2,
    EMERGENCY_STOP = 3,
} LawnmowerMode;






class UdpMessage : public Msg {
    MSG(UdpMessage);
    public:
    std::optional<std::string> dst;
    std::optional<std::string> src;
    std::optional<std::string> typ;
    std::optional<Poly> msg;
    
    // Field indexes
        typedef enum {
        DST_INDEX = 1,
        SRC_INDEX = 2,
        TYP_INDEX = 3,
        MSG_INDEX = 4,
    } Field;
    static Result<Bytes> json_serialize(const UdpMessage&);
    static Result<UdpMessage*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const UdpMessage&);
    static Result<UdpMessage*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const UdpMessage&);
    static Result<UdpMessage*> from_poly(const Poly&);
};



class UdpMessageCbor : public Msg {
    MSG(UdpMessageCbor);
    public:
    std::optional<uint32_t> dst;
    std::optional<uint32_t> src;
    std::optional<uint32_t> typ;
    std::optional<Poly> msg;
    
    // Field indexes
        typedef enum {
        DST_INDEX = 1,
        SRC_INDEX = 2,
        TYP_INDEX = 3,
        MSG_INDEX = 4,
    } Field;
    static Result<Bytes> json_serialize(const UdpMessageCbor&);
    static Result<UdpMessageCbor*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const UdpMessageCbor&);
    static Result<UdpMessageCbor*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const UdpMessageCbor&);
    static Result<UdpMessageCbor*> from_poly(const Poly&);
};



class AliveEvent : public Msg {
    MSG(AliveEvent);
    public:
    std::vector<std::string> subscribes;
    std::vector<std::string> publishes;
    std::vector<std::string> services;
    
    // Field indexes
        typedef enum {
        SUBSCRIBES_INDEX = 1,
        PUBLISHES_INDEX = 2,
        SERVICES_INDEX = 3,
    } Field;
    static Result<Bytes> json_serialize(const AliveEvent&);
    static Result<AliveEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const AliveEvent&);
    static Result<AliveEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const AliveEvent&);
    static Result<AliveEvent*> from_poly(const Poly&);
};



class LogEvent : public Msg {
    MSG(LogEvent);
    public:
    std::optional<LogLevel> level;
    std::optional<std::string> message;
    std::optional<int32_t> error_code;
    std::optional<std::string> file;
    std::optional<int32_t> line;
    std::optional<uint64_t> timestamp;
    
    // Field indexes
        typedef enum {
        LEVEL_INDEX = 2,
        MESSAGE_INDEX = 3,
        ERROR_CODE_INDEX = 4,
        FILE_INDEX = 5,
        LINE_INDEX = 6,
        TIMESTAMP_INDEX = 7,
    } Field;
    static Result<Bytes> json_serialize(const LogEvent&);
    static Result<LogEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const LogEvent&);
    static Result<LogEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const LogEvent&);
    static Result<LogEvent*> from_poly(const Poly&);
};



class SysRequest : public Msg {
    MSG(SysRequest);
    public:
    std::optional<uint64_t> set_time;
    std::optional<bool> reboot;
    std::optional<std::string> console;
    
    // Field indexes
        typedef enum {
        SET_TIME_INDEX = 1,
        REBOOT_INDEX = 2,
        CONSOLE_INDEX = 3,
    } Field;
    static Result<Bytes> json_serialize(const SysRequest&);
    static Result<SysRequest*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const SysRequest&);
    static Result<SysRequest*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const SysRequest&);
    static Result<SysRequest*> from_poly(const Poly&);
};



class SysReply : public Msg {
    MSG(SysReply);
    public:
    std::optional<int32_t> rc;
    std::optional<std::string> message;
    
    // Field indexes
        typedef enum {
        RC_INDEX = 1,
        MESSAGE_INDEX = 2,
    } Field;
    static Result<Bytes> json_serialize(const SysReply&);
    static Result<SysReply*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const SysReply&);
    static Result<SysReply*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const SysReply&);
    static Result<SysReply*> from_poly(const Poly&);
};



class SysEvent : public Msg {
    MSG(SysEvent);
    public:
    std::optional<uint64_t> utc;
    std::optional<uint64_t> uptime;
    std::optional<uint64_t> free_heap;
    std::optional<uint64_t> flash;
    std::optional<std::string> cpu_board;
    std::optional<std::string> build_date;
    
    // Field indexes
        typedef enum {
        UTC_INDEX = 1,
        UPTIME_INDEX = 2,
        FREE_HEAP_INDEX = 3,
        FLASH_INDEX = 4,
        CPU_BOARD_INDEX = 5,
        BUILD_DATE_INDEX = 6,
    } Field;
    static Result<Bytes> json_serialize(const SysEvent&);
    static Result<SysEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const SysEvent&);
    static Result<SysEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const SysEvent&);
    static Result<SysEvent*> from_poly(const Poly&);
};



class WifiEvent : public Msg {
    MSG(WifiEvent);
    public:
    std::optional<std::string> ssid;
    std::optional<std::string> bssid;
    std::optional<int32_t> rssi;
    std::optional<std::string> ip;
    std::optional<std::string> mac;
    std::optional<int32_t> channel;
    std::optional<std::string> gateway;
    std::optional<std::string> netmask;
    
    // Field indexes
        typedef enum {
        SSID_INDEX = 2,
        BSSID_INDEX = 3,
        RSSI_INDEX = 4,
        IP_INDEX = 5,
        MAC_INDEX = 6,
        CHANNEL_INDEX = 7,
        GATEWAY_INDEX = 8,
        NETMASK_INDEX = 9,
    } Field;
    static Result<Bytes> json_serialize(const WifiEvent&);
    static Result<WifiEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const WifiEvent&);
    static Result<WifiEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const WifiEvent&);
    static Result<WifiEvent*> from_poly(const Poly&);
};



class MulticastEvent : public Msg {
    MSG(MulticastEvent);
    public:
    std::optional<std::string> group;
    std::optional<int32_t> port;
    std::optional<uint32_t> mtu;
    
    // Field indexes
        typedef enum {
        GROUP_INDEX = 2,
        PORT_INDEX = 3,
        MTU_INDEX = 4,
    } Field;
    static Result<Bytes> json_serialize(const MulticastEvent&);
    static Result<MulticastEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const MulticastEvent&);
    static Result<MulticastEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const MulticastEvent&);
    static Result<MulticastEvent*> from_poly(const Poly&);
};



class PingRequest : public Msg {
    MSG(PingRequest);
    public:
    std::optional<uint32_t> number;
    
    // Field indexes
        typedef enum {
        NUMBER_INDEX = 1,
    } Field;
    static Result<Bytes> json_serialize(const PingRequest&);
    static Result<PingRequest*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const PingRequest&);
    static Result<PingRequest*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const PingRequest&);
    static Result<PingRequest*> from_poly(const Poly&);
};



class PingReply : public Msg {
    MSG(PingReply);
    public:
    std::optional<uint32_t> number;
    
    // Field indexes
        typedef enum {
        NUMBER_INDEX = 1,
    } Field;
    static Result<Bytes> json_serialize(const PingReply&);
    static Result<PingReply*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const PingReply&);
    static Result<PingReply*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const PingReply&);
    static Result<PingReply*> from_poly(const Poly&);
};



class HoverboardEventRaw : public Msg {
    MSG(HoverboardEventRaw);
    public:
    std::optional<int32_t> ctrl_mod;
    std::optional<int32_t> ctrl_typ;
    std::optional<int32_t> cur_mot_max;
    std::optional<int32_t> rpm_mot_max;
    std::optional<int32_t> fi_weak_ena;
    std::optional<int32_t> fi_weak_hi;
    std::optional<int32_t> fi_weak_lo;
    std::optional<int32_t> fi_weak_max;
    std::optional<int32_t> phase_adv_max_deg;
    std::optional<int32_t> input1_raw;
    std::optional<int32_t> input1_typ;
    std::optional<int32_t> input1_min;
    std::optional<int32_t> input1_mid;
    std::optional<int32_t> input1_max;
    std::optional<int32_t> input1_cmd;
    std::optional<int32_t> input2_raw;
    std::optional<int32_t> input2_typ;
    std::optional<int32_t> input2_min;
    std::optional<int32_t> input2_mid;
    std::optional<int32_t> input2_max;
    std::optional<int32_t> input2_cmd;
    std::optional<int32_t> aux_input1_raw;
    std::optional<int32_t> aux_input1_typ;
    std::optional<int32_t> aux_input1_min;
    std::optional<int32_t> aux_input1_mid;
    std::optional<int32_t> aux_input1_max;
    std::optional<int32_t> aux_input1_cmd;
    std::optional<int32_t> aux_input2_raw;
    std::optional<int32_t> aux_input2_typ;
    std::optional<int32_t> aux_input2_min;
    std::optional<int32_t> aux_input2_mid;
    std::optional<int32_t> aux_input2_max;
    std::optional<int32_t> aux_input2_cmd;
    std::optional<int32_t> dc_curr;
    std::optional<int32_t> rdc_curr;
    std::optional<int32_t> ldc_curr;
    std::optional<int32_t> cmdl;
    std::optional<int32_t> cmdr;
    std::optional<int32_t> spd_avg;
    std::optional<int32_t> spdl;
    std::optional<int32_t> spdr;
    std::optional<int32_t> filter_rate;
    std::optional<int32_t> spd_coef;
    std::optional<int32_t> str_coef;
    std::optional<int32_t> batv;
    std::optional<int32_t> temp;
    
    // Field indexes
        typedef enum {
        CTRL_MOD_INDEX = 0,
        CTRL_TYP_INDEX = 1,
        CUR_MOT_MAX_INDEX = 2,
        RPM_MOT_MAX_INDEX = 3,
        FI_WEAK_ENA_INDEX = 4,
        FI_WEAK_HI_INDEX = 5,
        FI_WEAK_LO_INDEX = 6,
        FI_WEAK_MAX_INDEX = 7,
        PHASE_ADV_MAX_DEG_INDEX = 8,
        INPUT1_RAW_INDEX = 9,
        INPUT1_TYP_INDEX = 10,
        INPUT1_MIN_INDEX = 11,
        INPUT1_MID_INDEX = 12,
        INPUT1_MAX_INDEX = 13,
        INPUT1_CMD_INDEX = 14,
        INPUT2_RAW_INDEX = 15,
        INPUT2_TYP_INDEX = 16,
        INPUT2_MIN_INDEX = 17,
        INPUT2_MID_INDEX = 18,
        INPUT2_MAX_INDEX = 19,
        INPUT2_CMD_INDEX = 20,
        AUX_INPUT1_RAW_INDEX = 21,
        AUX_INPUT1_TYP_INDEX = 22,
        AUX_INPUT1_MIN_INDEX = 23,
        AUX_INPUT1_MID_INDEX = 24,
        AUX_INPUT1_MAX_INDEX = 25,
        AUX_INPUT1_CMD_INDEX = 26,
        AUX_INPUT2_RAW_INDEX = 27,
        AUX_INPUT2_TYP_INDEX = 28,
        AUX_INPUT2_MIN_INDEX = 29,
        AUX_INPUT2_MID_INDEX = 30,
        AUX_INPUT2_MAX_INDEX = 31,
        AUX_INPUT2_CMD_INDEX = 32,
        DC_CURR_INDEX = 33,
        RDC_CURR_INDEX = 34,
        LDC_CURR_INDEX = 35,
        CMDL_INDEX = 36,
        CMDR_INDEX = 37,
        SPD_AVG_INDEX = 38,
        SPDL_INDEX = 39,
        SPDR_INDEX = 40,
        FILTER_RATE_INDEX = 41,
        SPD_COEF_INDEX = 42,
        STR_COEF_INDEX = 43,
        BATV_INDEX = 44,
        TEMP_INDEX = 45,
    } Field;
    static Result<Bytes> json_serialize(const HoverboardEventRaw&);
    static Result<HoverboardEventRaw*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const HoverboardEventRaw&);
    static Result<HoverboardEventRaw*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const HoverboardEventRaw&);
    static Result<HoverboardEventRaw*> from_poly(const Poly&);
};



class HoverboardEvent : public Msg {
    MSG(HoverboardEvent);
    public:
    std::optional<int32_t> ctrl_mod;
    std::optional<int32_t> ctrl_typ;
    std::optional<int32_t> cur_mot_max;
    std::optional<int32_t> rpm_mot_max;
    std::optional<int32_t> fi_weak_ena;
    std::optional<int32_t> fi_weak_hi;
    std::optional<int32_t> fi_weak_lo;
    std::optional<int32_t> fi_weak_max;
    std::optional<int32_t> phase_adv_max_deg;
    std::optional<int32_t> input1_raw;
    std::optional<int32_t> input1_typ;
    std::optional<int32_t> input1_min;
    std::optional<int32_t> input1_mid;
    std::optional<int32_t> input1_max;
    std::optional<int32_t> input1_cmd;
    std::optional<int32_t> input2_raw;
    std::optional<int32_t> input2_typ;
    std::optional<int32_t> input2_min;
    std::optional<int32_t> input2_mid;
    std::optional<int32_t> input2_max;
    std::optional<int32_t> input2_cmd;
    std::optional<int32_t> aux_input1_raw;
    std::optional<int32_t> aux_input1_typ;
    std::optional<int32_t> aux_input1_min;
    std::optional<int32_t> aux_input1_mid;
    std::optional<int32_t> aux_input1_max;
    std::optional<int32_t> aux_input1_cmd;
    std::optional<int32_t> aux_input2_raw;
    std::optional<int32_t> aux_input2_typ;
    std::optional<int32_t> aux_input2_min;
    std::optional<int32_t> aux_input2_mid;
    std::optional<int32_t> aux_input2_max;
    std::optional<int32_t> aux_input2_cmd;
    std::optional<float> dc_curr;
    std::optional<float> rdc_curr;
    std::optional<float> ldc_curr;
    std::optional<int32_t> cmdl;
    std::optional<int32_t> cmdr;
    std::optional<int32_t> spd_avg;
    std::optional<int32_t> spdl;
    std::optional<int32_t> spdr;
    std::optional<int32_t> filter_rate;
    std::optional<int32_t> spd_coef;
    std::optional<int32_t> str_coef;
    std::optional<float> batv;
    std::optional<float> temp;
    
    // Field indexes
        typedef enum {
        CTRL_MOD_INDEX = 0,
        CTRL_TYP_INDEX = 1,
        CUR_MOT_MAX_INDEX = 2,
        RPM_MOT_MAX_INDEX = 3,
        FI_WEAK_ENA_INDEX = 4,
        FI_WEAK_HI_INDEX = 5,
        FI_WEAK_LO_INDEX = 6,
        FI_WEAK_MAX_INDEX = 7,
        PHASE_ADV_MAX_DEG_INDEX = 8,
        INPUT1_RAW_INDEX = 9,
        INPUT1_TYP_INDEX = 10,
        INPUT1_MIN_INDEX = 11,
        INPUT1_MID_INDEX = 12,
        INPUT1_MAX_INDEX = 13,
        INPUT1_CMD_INDEX = 14,
        INPUT2_RAW_INDEX = 15,
        INPUT2_TYP_INDEX = 16,
        INPUT2_MIN_INDEX = 17,
        INPUT2_MID_INDEX = 18,
        INPUT2_MAX_INDEX = 19,
        INPUT2_CMD_INDEX = 20,
        AUX_INPUT1_RAW_INDEX = 21,
        AUX_INPUT1_TYP_INDEX = 22,
        AUX_INPUT1_MIN_INDEX = 23,
        AUX_INPUT1_MID_INDEX = 24,
        AUX_INPUT1_MAX_INDEX = 25,
        AUX_INPUT1_CMD_INDEX = 26,
        AUX_INPUT2_RAW_INDEX = 27,
        AUX_INPUT2_TYP_INDEX = 28,
        AUX_INPUT2_MIN_INDEX = 29,
        AUX_INPUT2_MID_INDEX = 30,
        AUX_INPUT2_MAX_INDEX = 31,
        AUX_INPUT2_CMD_INDEX = 32,
        DC_CURR_INDEX = 33,
        RDC_CURR_INDEX = 34,
        LDC_CURR_INDEX = 35,
        CMDL_INDEX = 36,
        CMDR_INDEX = 37,
        SPD_AVG_INDEX = 38,
        SPDL_INDEX = 39,
        SPDR_INDEX = 40,
        FILTER_RATE_INDEX = 41,
        SPD_COEF_INDEX = 42,
        STR_COEF_INDEX = 43,
        BATV_INDEX = 44,
        TEMP_INDEX = 45,
    } Field;
    static Result<Bytes> json_serialize(const HoverboardEvent&);
    static Result<HoverboardEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const HoverboardEvent&);
    static Result<HoverboardEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const HoverboardEvent&);
    static Result<HoverboardEvent*> from_poly(const Poly&);
};



class HoverboardRequest : public Msg {
    MSG(HoverboardRequest);
    public:
    std::optional<int32_t> speed;
    std::optional<int32_t> steer;
    
    // Field indexes
        typedef enum {
        SPEED_INDEX = 0,
        STEER_INDEX = 1,
    } Field;
    static Result<Bytes> json_serialize(const HoverboardRequest&);
    static Result<HoverboardRequest*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const HoverboardRequest&);
    static Result<HoverboardRequest*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const HoverboardRequest&);
    static Result<HoverboardRequest*> from_poly(const Poly&);
};



class HoverboardReply : public Msg {
    MSG(HoverboardReply);
    public:
    std::optional<int32_t> error_code;
    std::optional<std::string> message;
    
    // Field indexes
        typedef enum {
        ERROR_CODE_INDEX = 0,
        MESSAGE_INDEX = 1,
    } Field;
    static Result<Bytes> json_serialize(const HoverboardReply&);
    static Result<HoverboardReply*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const HoverboardReply&);
    static Result<HoverboardReply*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const HoverboardReply&);
    static Result<HoverboardReply*> from_poly(const Poly&);
};



class TouchPoint : public Msg {
    MSG(TouchPoint);
    public:
    std::optional<bool> active;
    std::optional<int32_t> id;
    std::optional<int32_t> x;
    std::optional<int32_t> y;
    
    // Field indexes
        typedef enum {
        ACTIVE_INDEX = 4,
        ID_INDEX = 1,
        X_INDEX = 2,
        Y_INDEX = 3,
    } Field;
    static Result<Bytes> json_serialize(const TouchPoint&);
    static Result<TouchPoint*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const TouchPoint&);
    static Result<TouchPoint*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const TouchPoint&);
    static Result<TouchPoint*> from_poly(const Poly&);
};



class Ps4Event : public Msg {
    MSG(Ps4Event);
    public:
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
    
    // Field indexes
        typedef enum {
        BUTTON_LEFT_INDEX = 1,
        BUTTON_RIGHT_INDEX = 2,
        BUTTON_UP_INDEX = 3,
        BUTTON_DOWN_INDEX = 4,
        BUTTON_SQUARE_INDEX = 5,
        BUTTON_CROSS_INDEX = 6,
        BUTTON_CIRCLE_INDEX = 7,
        BUTTON_TRIANGLE_INDEX = 8,
        BUTTON_LEFT_SHOULDER_INDEX = 9,
        BUTTON_RIGHT_SHOULDER_INDEX = 10,
        BUTTON_LEFT_TRIGGER_INDEX = 11,
        BUTTON_RIGHT_TRIGGER_INDEX = 12,
        BUTTON_LEFT_JOYSTICK_INDEX = 13,
        BUTTON_RIGHT_JOYSTICK_INDEX = 14,
        BUTTON_SHARE_INDEX = 15,
        BUTTON_OPTIONS_INDEX = 16,
        BUTTON_TOUCHPAD_INDEX = 33,
        BUTTON_PS_INDEX = 34,
        AXIS_LX_INDEX = 17,
        AXIS_LY_INDEX = 18,
        AXIS_RX_INDEX = 19,
        AXIS_RY_INDEX = 20,
        GYRO_X_INDEX = 21,
        GYRO_Y_INDEX = 22,
        GYRO_Z_INDEX = 23,
        ACCEL_X_INDEX = 24,
        ACCEL_Y_INDEX = 25,
        ACCEL_Z_INDEX = 26,
        CONNECTED_INDEX = 27,
        BATTERY_LEVEL_INDEX = 28,
        BLUETOOTH_INDEX = 30,
        DEBUG_INDEX = 31,
        TEMP_INDEX = 32,
    } Field;
    static Result<Bytes> json_serialize(const Ps4Event&);
    static Result<Ps4Event*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const Ps4Event&);
    static Result<Ps4Event*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const Ps4Event&);
    static Result<Ps4Event*> from_poly(const Poly&);
};



class Ps4Request : public Msg {
    MSG(Ps4Request);
    public:
    std::optional<int32_t> rumble_small;
    std::optional<int32_t> rumble_large;
    std::optional<int32_t> led_red;
    std::optional<int32_t> led_green;
    std::optional<int32_t> led_blue;
    std::optional<int32_t> led_flash_on;
    std::optional<int32_t> led_flash_off;
    
    // Field indexes
        typedef enum {
        RUMBLE_SMALL_INDEX = 1,
        RUMBLE_LARGE_INDEX = 2,
        LED_RED_INDEX = 3,
        LED_GREEN_INDEX = 4,
        LED_BLUE_INDEX = 5,
        LED_FLASH_ON_INDEX = 6,
        LED_FLASH_OFF_INDEX = 7,
    } Field;
    static Result<Bytes> json_serialize(const Ps4Request&);
    static Result<Ps4Request*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const Ps4Request&);
    static Result<Ps4Request*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const Ps4Request&);
    static Result<Ps4Request*> from_poly(const Poly&);
};



class CameraEvent : public Msg {
    MSG(CameraEvent);
    public:
    std::optional<int32_t> width;
    std::optional<int32_t> height;
    std::optional<std::string> format;
    std::optional<Bytes> data;
    std::optional<bool> led;
    std::optional<int32_t> quality;
    
    // Field indexes
        typedef enum {
        WIDTH_INDEX = 1,
        HEIGHT_INDEX = 2,
        FORMAT_INDEX = 3,
        DATA_INDEX = 4,
        LED_INDEX = 5,
        QUALITY_INDEX = 6,
    } Field;
    static Result<Bytes> json_serialize(const CameraEvent&);
    static Result<CameraEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const CameraEvent&);
    static Result<CameraEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const CameraEvent&);
    static Result<CameraEvent*> from_poly(const Poly&);
};



class CameraRequest : public Msg {
    MSG(CameraRequest);
    public:
    std::optional<bool> led;
    std::optional<std::string> capture_tcp_destination;
    std::optional<int32_t> quality;
    
    // Field indexes
        typedef enum {
        LED_INDEX = 1,
        CAPTURE_TCP_DESTINATION_INDEX = 2,
        QUALITY_INDEX = 4,
    } Field;
    static Result<Bytes> json_serialize(const CameraRequest&);
    static Result<CameraRequest*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const CameraRequest&);
    static Result<CameraRequest*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const CameraRequest&);
    static Result<CameraRequest*> from_poly(const Poly&);
};



class CameraReply : public Msg {
    MSG(CameraReply);
    public:
    std::optional<int32_t> error_code;
    std::optional<std::string> message;
    std::optional<Bytes> data;
    
    // Field indexes
        typedef enum {
        ERROR_CODE_INDEX = 1,
        MESSAGE_INDEX = 2,
        DATA_INDEX = 3,
    } Field;
    static Result<Bytes> json_serialize(const CameraReply&);
    static Result<CameraReply*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const CameraReply&);
    static Result<CameraReply*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const CameraReply&);
    static Result<CameraReply*> from_poly(const Poly&);
};



class LawnmowerManualEvent : public Msg {
    MSG(LawnmowerManualEvent);
    public:
    std::optional<int32_t> speed;
    std::optional<int32_t> steering;
    std::optional<bool> blade;
    
    // Field indexes
        typedef enum {
        SPEED_INDEX = 1,
        STEERING_INDEX = 2,
        BLADE_INDEX = 3,
    } Field;
    static Result<Bytes> json_serialize(const LawnmowerManualEvent&);
    static Result<LawnmowerManualEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const LawnmowerManualEvent&);
    static Result<LawnmowerManualEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const LawnmowerManualEvent&);
    static Result<LawnmowerManualEvent*> from_poly(const Poly&);
};



class LawnmowerManualRequest : public Msg {
    MSG(LawnmowerManualRequest);
    public:
    std::optional<float> speed;
    std::optional<float> steer;
    std::optional<bool> blade;
    std::optional<bool> start_manual_control;
    std::optional<bool> stop_manual_control;
    std::optional<bool> emergency_stop;
    std::optional<bool> start_auto_mode;
    std::optional<bool> stop_auto_mode;
    
    // Field indexes
        typedef enum {
        SPEED_INDEX = 1,
        STEER_INDEX = 2,
        BLADE_INDEX = 3,
        START_MANUAL_CONTROL_INDEX = 4,
        STOP_MANUAL_CONTROL_INDEX = 5,
        EMERGENCY_STOP_INDEX = 6,
        START_AUTO_MODE_INDEX = 7,
        STOP_AUTO_MODE_INDEX = 8,
    } Field;
    static Result<Bytes> json_serialize(const LawnmowerManualRequest&);
    static Result<LawnmowerManualRequest*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const LawnmowerManualRequest&);
    static Result<LawnmowerManualRequest*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const LawnmowerManualRequest&);
    static Result<LawnmowerManualRequest*> from_poly(const Poly&);
};



class LawnmowerManualReply : public Msg {
    MSG(LawnmowerManualReply);
    public:
    std::optional<int32_t> error_code;
    std::optional<std::string> message;
    
    // Field indexes
        typedef enum {
        ERROR_CODE_INDEX = 1,
        MESSAGE_INDEX = 2,
    } Field;
    static Result<Bytes> json_serialize(const LawnmowerManualReply&);
    static Result<LawnmowerManualReply*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const LawnmowerManualReply&);
    static Result<LawnmowerManualReply*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const LawnmowerManualReply&);
    static Result<LawnmowerManualReply*> from_poly(const Poly&);
};



class LawnmowerAutoEvent : public Msg {
    MSG(LawnmowerAutoEvent);
    public:
    std::optional<bool> started;
    std::optional<bool> stopped;
    std::optional<bool> paused;
    std::optional<bool> resumed;
    std::optional<std::string> mode;
    std::optional<std::string> path;
    
    // Field indexes
        typedef enum {
        STARTED_INDEX = 1,
        STOPPED_INDEX = 2,
        PAUSED_INDEX = 3,
        RESUMED_INDEX = 4,
        MODE_INDEX = 5,
        PATH_INDEX = 6,
    } Field;
    static Result<Bytes> json_serialize(const LawnmowerAutoEvent&);
    static Result<LawnmowerAutoEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const LawnmowerAutoEvent&);
    static Result<LawnmowerAutoEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const LawnmowerAutoEvent&);
    static Result<LawnmowerAutoEvent*> from_poly(const Poly&);
};



class LawnmowerAutoRequest : public Msg {
    MSG(LawnmowerAutoRequest);
    public:
    std::optional<bool> start;
    std::optional<bool> stop;
    std::optional<bool> pause;
    std::optional<bool> resume;
    std::optional<std::string> mode;
    std::optional<std::string> path;
    
    // Field indexes
        typedef enum {
        START_INDEX = 1,
        STOP_INDEX = 2,
        PAUSE_INDEX = 3,
        RESUME_INDEX = 4,
        MODE_INDEX = 5,
        PATH_INDEX = 6,
    } Field;
    static Result<Bytes> json_serialize(const LawnmowerAutoRequest&);
    static Result<LawnmowerAutoRequest*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const LawnmowerAutoRequest&);
    static Result<LawnmowerAutoRequest*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const LawnmowerAutoRequest&);
    static Result<LawnmowerAutoRequest*> from_poly(const Poly&);
};



class LawnmowerStatus : public Msg {
    MSG(LawnmowerStatus);
    public:
    std::optional<int32_t> battery_level;
    std::optional<bool> blade_status;
    std::optional<std::string> current_mode;
    std::optional<std::string> error_message;
    
    // Field indexes
        typedef enum {
        BATTERY_LEVEL_INDEX = 1,
        BLADE_STATUS_INDEX = 2,
        CURRENT_MODE_INDEX = 3,
        ERROR_MESSAGE_INDEX = 4,
    } Field;
    static Result<Bytes> json_serialize(const LawnmowerStatus&);
    static Result<LawnmowerStatus*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const LawnmowerStatus&);
    static Result<LawnmowerStatus*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const LawnmowerStatus&);
    static Result<LawnmowerStatus*> from_poly(const Poly&);
};



class MotorEvent : public Msg {
    MSG(MotorEvent);
    public:
    std::optional<int32_t> motor_id;
    std::optional<float> temperature;
    std::optional<float> voltage;
    std::optional<float> current;
    std::optional<float> speed;
    std::optional<float> position;
    
    // Field indexes
        typedef enum {
        MOTOR_ID_INDEX = 1,
        TEMPERATURE_INDEX = 2,
        VOLTAGE_INDEX = 3,
        CURRENT_INDEX = 4,
        SPEED_INDEX = 5,
        POSITION_INDEX = 6,
    } Field;
    static Result<Bytes> json_serialize(const MotorEvent&);
    static Result<MotorEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const MotorEvent&);
    static Result<MotorEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const MotorEvent&);
    static Result<MotorEvent*> from_poly(const Poly&);
};



class Max31855Event : public Msg {
    MSG(Max31855Event);
    public:
    std::optional<float> thermocouple_c;
    std::optional<float> internal_c;
    std::optional<bool> fault;
    std::optional<bool> open_circuit;
    std::optional<bool> short_to_gnd;
    std::optional<bool> short_to_vcc;
    std::optional<uint64_t> timestamp_ms;
    
    // Field indexes
        typedef enum {
        THERMOCOUPLE_C_INDEX = 1,
        INTERNAL_C_INDEX = 2,
        FAULT_INDEX = 3,
        OPEN_CIRCUIT_INDEX = 4,
        SHORT_TO_GND_INDEX = 5,
        SHORT_TO_VCC_INDEX = 6,
        TIMESTAMP_MS_INDEX = 7,
    } Field;
    static Result<Bytes> json_serialize(const Max31855Event&);
    static Result<Max31855Event*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const Max31855Event&);
    static Result<Max31855Event*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const Max31855Event&);
    static Result<Max31855Event*> from_poly(const Poly&);
};



class HeatingRequest : public Msg {
    MSG(HeatingRequest);
    public:
    std::optional<float> setpoint_c;
    std::optional<bool> enabled;
    std::optional<float> kp;
    std::optional<float> ki;
    std::optional<float> kd;
    std::optional<bool> reset_integral;
    
    // Field indexes
        typedef enum {
        SETPOINT_C_INDEX = 1,
        ENABLED_INDEX = 2,
        KP_INDEX = 3,
        KI_INDEX = 4,
        KD_INDEX = 5,
        RESET_INTEGRAL_INDEX = 6,
    } Field;
    static Result<Bytes> json_serialize(const HeatingRequest&);
    static Result<HeatingRequest*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const HeatingRequest&);
    static Result<HeatingRequest*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const HeatingRequest&);
    static Result<HeatingRequest*> from_poly(const Poly&);
};



class HeatingEvent : public Msg {
    MSG(HeatingEvent);
    public:
    std::optional<float> temperature_c;
    std::optional<float> setpoint_c;
    std::optional<float> output_pct;
    std::optional<bool> enabled;
    std::optional<bool> heater_on;
    std::optional<bool> fault;
    std::optional<uint64_t> timestamp_ms;
    
    // Field indexes
        typedef enum {
        TEMPERATURE_C_INDEX = 1,
        SETPOINT_C_INDEX = 2,
        OUTPUT_PCT_INDEX = 3,
        ENABLED_INDEX = 4,
        HEATER_ON_INDEX = 5,
        FAULT_INDEX = 6,
        TIMESTAMP_MS_INDEX = 7,
    } Field;
    static Result<Bytes> json_serialize(const HeatingEvent&);
    static Result<HeatingEvent*> json_deserialize(const Bytes&);
    static Result<Bytes> cbor_serialize(const HeatingEvent&);
    static Result<HeatingEvent*> cbor_deserialize(const Bytes&);
    static Result<Poly> to_poly(const HeatingEvent&);
    static Result<HeatingEvent*> from_poly(const Poly&);
};


