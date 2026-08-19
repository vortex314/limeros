// Auto-generated from robot.hcl — do not edit by hand.
// Header file — class declarations for generated messages.
// Every field is `Option<T>`; missing fields are omitted from encoded CBOR.
// Messages are encoded as CBOR maps keyed by field id.
// Uses TinyCBOR for CBOR encoding/decoding.
#ifndef HCL_CPP_TERA
#define HCL_CPP_TERA
#pragma once

#include <errno.h>
#include <msg.h>
#include <unordered_map>
#include <string>

// ── TinyCBOR helper ────────────────────────────────────────────────────────

#define cbor_check(X) { \
    CborError err = X; \
    if (err != CborNoError) { \
        WARN("failed cbor : %s : %d",#X, err); \
         return EINVAL; \
    }   \
    }

// ── Name lookup ────────────────────────────────────────────────────────────

extern std::unordered_map<uint32_t, const char*> id_to_name;
const char *id_to_string(uint32_t msg_id);

// ── Messages ───────────────────────────────────────────────────────────────



class BrokerSubscribeRequest : public Msg {
public:

    static const uint32_t MSG_ID = FNV("BrokerSubscribeRequest");
    static constexpr const char *MSG_NAME ="BrokerSubscribeRequest";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> src;
    Option<uint32_t> msg_type;

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class CompassEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("CompassEvent");
    static constexpr const char *MSG_NAME ="CompassEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<float> heading;// Heading in degrees
    Option<float> pitch;// Pitch in degrees
    Option<float> roll;// Roll in degrees
    Option<float> mag_x;// Magnetometer X axis in uT
    Option<float> mag_y;// Magnetometer Y axis in uT
    Option<float> mag_z;// Magnetometer Z axis in uT
    Option<float> accel_x;// Accelerometer X axis in m/s^2
    Option<float> accel_y;// Accelerometer Y axis in m/s^2
    Option<float> accel_z;// Accelerometer Z axis in m/s^2

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class CutterEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("CutterEvent");
    static constexpr const char *MSG_NAME ="CutterEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<bool> enabled;// Cutter enabled or disabled
    Option<int32_t> rpm;// Current speed of the cutter in RPM
    Option<float> current;// Current drawn by the cutter in Amperes
    Option<float> voltage;// Voltage supplied to the cutter in Volts
    Option<float> temperature;// Temperature of the cutter in Celsius

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class CutterReply : public Msg {
public:

    static const uint32_t MSG_ID = FNV("CutterReply");
    static constexpr const char *MSG_NAME ="CutterReply";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<uint32_t> error_code;// Error code, 0 if no error
    Option<std::string> message;// Error message or additional information
    Option<uint32_t> msg_type;// Message type identifier , the original request

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class CutterRequest : public Msg {
public:

    static const uint32_t MSG_ID = FNV("CutterRequest");
    static constexpr const char *MSG_NAME ="CutterRequest";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<bool> enabled;// Enable or disable the cutter
    Option<int32_t> rpm;// Speed command for the cutter in RPM

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class DeviceAliveEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("DeviceAliveEvent");
    static constexpr const char *MSG_NAME ="DeviceAliveEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<std::string> device;
    Option<std::string> endpoint;
    Option<uint64_t> timestamp;// Timestamp in milliseconds since epoch

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class EndpointAnnounce : public Msg {
public:

    static const uint32_t MSG_ID = FNV("EndpointAnnounce");
    static constexpr const char *MSG_NAME ="EndpointAnnounce";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> id;// Unique identifier for the announcing endpoint
    Option<std::string> name;// Name of the announcing endpoint
    Option<std::string> description;// Description of the announcing endpoint
    Option<std::vector<uint32_t>> services;// List of services provided by the endpoint
    Option<std::vector<uint32_t>> events;// List of events emitted by the endpoint
    Option<std::vector<uint32_t>> replies;// List of replies supported by the endpoint
    Option<std::vector<uint32_t>> subscribes;// List of subscriptions for the endpoint

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class EndpointAnnounceReply : public Msg {
public:

    static const uint32_t MSG_ID = FNV("EndpointAnnounceReply");
    static constexpr const char *MSG_NAME ="EndpointAnnounceReply";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint64_t> utc;// Timestamp in milliseconds since epoch

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class Envelope : public Msg {
public:

    static const uint32_t MSG_ID = FNV("Envelope");
    static constexpr const char *MSG_NAME ="Envelope";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> src;// Source endpoint name
    Option<uint32_t> dst;// Destination endpoint name
    Option<uint32_t> msg_type;// Message type name
    Option<uint32_t> sequence;// Request ID for matching request/reply
    Option<uint32_t> instance_id;// Instance ID for matching request/reply
    Option<std::vector<uint8_t>> payload;// Serialized payload of the message

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class GenericReply : public Msg {
public:

    static const uint32_t MSG_ID = FNV("GenericReply");
    static constexpr const char *MSG_NAME ="GenericReply";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<uint32_t> error_code;// Error code, 0 if no error
    Option<std::string> message;// Error message or additional information
    Option<uint32_t> msg_type;// Message type identifier , the original request

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class HeatingEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("HeatingEvent");
    static constexpr const char *MSG_NAME ="HeatingEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<float> temperature_c;// Current temperature in Celsius
    Option<float> setpoint_c;// Setpoint temperature in Celsius
    Option<bool> enabled;// Heating enabled or disabled
    Option<float> output_pct;// Output percentage of the heating element
    Option<bool> heater_on;// Heater is currently on or off
    Option<bool> fault;// Fault detected in the heating system
    Option<uint64_t> timestamp_ms;// Timestamp in milliseconds since epoch

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class HeatingRequest : public Msg {
public:

    static const uint32_t MSG_ID = FNV("HeatingRequest");
    static constexpr const char *MSG_NAME ="HeatingRequest";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<float> setpoint_c;// Setpoint temperature in Celsius
    Option<bool> enabled;// Enable or disable heating
    Option<float> kp;// Proportional gain for PID controller
    Option<float> ki;// Integral gain for PID controller
    Option<float> kd;// Derivative gain for PID controller
    Option<bool> reset_integral;// Reset the integral term of the PID controller

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class HoverboardEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("HoverboardEvent");
    static constexpr const char *MSG_NAME ="HoverboardEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
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

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class HoverboardReply : public Msg {
public:

    static const uint32_t MSG_ID = FNV("HoverboardReply");
    static constexpr const char *MSG_NAME ="HoverboardReply";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<int32_t> speed;// Speed command for the hoverboard
    Option<int32_t> steer;// Steering command for the hoverboard

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class HoverboardRequest : public Msg {
public:

    static const uint32_t MSG_ID = FNV("HoverboardRequest");
    static constexpr const char *MSG_NAME ="HoverboardRequest";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<int32_t> speed;// Speed command for the hoverboard
    Option<int32_t> steer;// Steering command for the hoverboard

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class ImuEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("ImuEvent");
    static constexpr const char *MSG_NAME ="ImuEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<float> gyro_x;// Gyroscope X axis in deg/s
    Option<float> gyro_y;// Gyroscope Y axis in deg/s
    Option<float> gyro_z;// Gyroscope Z axis in deg/s
    Option<float> accel_x;// Accelerometer X axis in m/s^2
    Option<float> accel_y;// Accelerometer Y axis in m/s^2
    Option<float> accel_z;// Accelerometer Z axis in m/s^2

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class Max31855Event : public Msg {
public:

    static const uint32_t MSG_ID = FNV("Max31855Event");
    static constexpr const char *MSG_NAME ="Max31855Event";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<float> thermocouple_temp;// Thermocouple temperature in Celsius
    Option<float> internal_temp;// Internal temperature in Celsius
    Option<bool> fault;// Fault detected
    Option<bool> fault_short_vcc;// Short to VCC detected
    Option<bool> fault_short_gnd;// Short to GND detected
    Option<bool> fault_open_tc;// Open thermocouple detected

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class PingReply : public Msg {
public:

    static const uint32_t MSG_ID = FNV("PingReply");
    static constexpr const char *MSG_NAME ="PingReply";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> req_id;
    Option<uint64_t> timestamp;// Timestamp in milliseconds since epoch

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class PingRequest : public Msg {
public:

    static const uint32_t MSG_ID = FNV("PingRequest");
    static constexpr const char *MSG_NAME ="PingRequest";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> req_id;
    Option<uint64_t> timestamp;// Timestamp in milliseconds since epoch

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class Ps4Event : public Msg {
public:

    static const uint32_t MSG_ID = FNV("Ps4Event");
    static constexpr const char *MSG_NAME ="Ps4Event";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
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

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class Ps4Request : public Msg {
public:

    static const uint32_t MSG_ID = FNV("Ps4Request");
    static constexpr const char *MSG_NAME ="Ps4Request";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<int32_t> rumble_small;
    Option<int32_t> rumble_large;
    Option<int32_t> led_red;
    Option<int32_t> led_green;
    Option<int32_t> led_blue;
    Option<int32_t> led_flash_on;
    Option<int32_t> led_flash_off;

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class SysEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("SysEvent");
    static constexpr const char *MSG_NAME ="SysEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint64_t> utc;
    Option<uint64_t> uptime;
    Option<uint64_t> free_heap;
    Option<uint64_t> flash_size;
    Option<std::string> cpu_board_type;
    Option<std::string> build_date_time;

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class SysReply : public Msg {
public:

    static const uint32_t MSG_ID = FNV("SysReply");
    static constexpr const char *MSG_NAME ="SysReply";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<int32_t> rc;
    Option<std::string> message;

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class SysRequest : public Msg {
public:

    static const uint32_t MSG_ID = FNV("SysRequest");
    static constexpr const char *MSG_NAME ="SysRequest";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<uint32_t> req_id;// For request/reply matching, 0 if not a request/reply
    Option<uint64_t> set_time;
    Option<bool> reboot;
    Option<std::string> console;

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class UsEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("UsEvent");
    static constexpr const char *MSG_NAME ="UsEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<float> distance;// Distance in meters
    Option<float> temperature;// Temperature in Celsius
    Option<int32_t> status;// Status code, 0 if no error

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};



class WifiEvent : public Msg {
public:

    static const uint32_t MSG_ID = FNV("WifiEvent");
    static constexpr const char *MSG_NAME ="WifiEvent";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };
    Option<std::string> ip;
    Option<std::string> gateway;
    Option<std::string> netmask;
    Option<std::string> ssid;
    Option<std::string> bssid;
    Option<int32_t> channel;
    Option<int32_t> rssi;
    Option<std::string> mac;

    int encode(Buffer& buffer) const;
    int decode(Buffer& buffer);
};


#endif
