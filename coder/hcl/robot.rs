// Converted from hcl/robot.hcl.
// Non-repeated fields are `Option<T>`; repeated (`repeated = true`) and `T[]` fields are `Vec<T>`.

struct DeviceEvent {
    device_name: Option<String>, // Device Name
    request_types: Vec<u32>,     // Requests supported
    event_types: Vec<u32>,       // Requests supported
}

struct DeviceRequest {
    ip_address: Option<Vec<u8>>, // Broker Ip Address
    port: Option<u32>,           // Broker UDP port
    device_name: Option<String>, // Device Name
}

struct DW1000TagEvent {
    uwb_mac_address: Option<Vec<u8>>, // UWB Mac Address of Tag
    anchor: Option<u16>,              // Short address of anchor
    distance_m: Option<f32>,          // Distance from anchor to Tag
    rx_power_db: Option<f32>,         // Reception strength of tag from anchor
}

struct DW1000AnchorEvent {
    uwb_mac_address: Option<Vec<u8>>, // UWB Mac Address of Tag
}

// Request command for hoverboard drive
struct HoverboardRequest {
    speed: Option<i32>, // Speed command for the hoverboard
    steer: Option<i32>, // Steering command for the hoverboard
}

// Request command for hoverboard drive
struct HoverboardReply {
    speed: Option<i32>, // Speed command for the hoverboard
    steer: Option<i32>, // Steering command for the hoverboard
}

// Reply for hoverboard request
struct GenericReply {
    req_id: Option<u32>,     // For request/reply matching, 0 if not a request/reply
    error_code: Option<u32>, // Error code, 0 if no error
    message: Option<String>, // Error message or additional information
    msg_type: Option<u32>,   // Message type identifier , the original request
}

// Telemetry emitted by the hoverboard
struct HoverboardEvent {
    ctrl_mod: Option<i32>,          // 1:Voltage 2:Speed 3:Torque
    ctrl_typ: Option<i32>,          // 0:Commutation 1:Sinusoidal 2:FOC
    cur_mot_max: Option<i32>,       // Max phase current A
    rpm_mot_max: Option<i32>,       // Max motor RPM
    fi_weak_ena: Option<i32>,       // Enable field weak 0:OFF 1:ON
    fi_weak_hi: Option<i32>,        // Field weak high RPM
    fi_weak_lo: Option<i32>,        // Field weak low RPM
    fi_weak_max: Option<i32>,       // Field weak max current A (FOC only)
    phase_adv_max_deg: Option<i32>, // Max Phase Adv angle Deg (SIN only)
    input1_raw: Option<i32>,        // Input1 raw value
    input1_typ: Option<i32>,        // Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    input1_min: Option<i32>,        // Input1 minimum value
    input1_mid: Option<i32>,        // Input1 middle value
    input1_max: Option<i32>,        // Input1 maximum value
    input1_cmd: Option<i32>,        // Input1 command value
    input2_raw: Option<i32>,        // Input2 raw value
    input2_typ: Option<i32>,        // Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    input2_min: Option<i32>,        // Input2 minimum value
    input2_mid: Option<i32>,        // Input2 middle value
    input2_max: Option<i32>,        // Input2 maximum value
    input2_cmd: Option<i32>,        // Input2 command value
    aux_input1_raw: Option<i32>,    // Input1 raw value
    aux_input1_typ: Option<i32>,    // Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    aux_input1_min: Option<i32>,    // Input1 minimum value
    aux_input1_mid: Option<i32>,    // Input1 middle value
    aux_input1_max: Option<i32>,    // Input1 maximum value
    aux_input1_cmd: Option<i32>,    // Input1 command value
    aux_input2_raw: Option<i32>,    // Input2 raw value
    aux_input2_typ: Option<i32>,    // Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    aux_input2_min: Option<i32>,    // Input2 minimum value
    aux_input2_mid: Option<i32>,    // Input2 middle value
    aux_input2_max: Option<i32>,    // Input2 maximum value
    aux_input2_cmd: Option<i32>,    // Input2 command value
    dc_curr: Option<i32>,           // Total DC Link current A *100
    rdc_curr: Option<i32>,          // Right DC Link current A *100
    ldc_curr: Option<i32>,          // Left DC Link current A *100
    cmdl: Option<i32>,              // Left Motor Command RPM
    cmdr: Option<i32>,              // Right Motor Command RPM
    spd_avg: Option<i32>,           // Motor Measured Avg RPM
    spdl: Option<i32>,              // Left Motor Measured RPM
    spdr: Option<i32>,              // Right Motor Measured RPM
    filter_rate: Option<i32>,       // Rate *10
    spd_coef: Option<i32>,          // Speed Coefficient *10
    str_coef: Option<i32>,          // Steer Coefficient *10
    batv: Option<i32>,              // Calibrated Battery Voltage *100
    temp: Option<i32>,              // Calibrated Temperature C *10
}

// Request command for cutter
struct MotorRequest {
    enabled: Option<bool>,   // Enable or disable the cutter
    rpm_target: Option<i32>, // Speed command for the cutter in RPM
    Kp: Option<f32>,         // Proportional gain for the cutter motor controller
    Ki: Option<f32>,         // Integral gain for the cutter motor controller
    Kd: Option<f32>,         // Derivative gain for the cutter motor controller
}

// Reply for cutter request
struct MotorReply {
    status_code: Option<u32>, // Error code, 0 if no error
    message: Option<String>,  // Error message or additional information
}

// Telemetry emitted by the cutter
struct MotorEvent {
    enabled: Option<bool>,     // Cutter enabled or disabled
    rpm_measured: Option<i32>, // Current speed of the cutter in RPM
    rpm_target: Option<i32>,   // Commanded speed of the cutter in RPM
    pwm: Option<f32>,          // PWM duty cycle for the cutter motor in percent
    current: Option<f32>,      // Current drawn by the cutter in Amperes
    voltage: Option<f32>,      // Voltage supplied to the cutter in Volts
    temperature: Option<f32>,  // Temperature of the cutter in Celsius
    Kp: Option<f32>,           // Proportional gain for the cutter motor controller
    Ki: Option<f32>,           // Integral gain for the cutter motor controller
    Kd: Option<f32>,           // Derivative gain for the cutter motor controller
}

// Compass event message
struct CompassEvent {
    heading: Option<f32>, // Heading in degrees
    pitch: Option<f32>,   // Pitch in degrees
    roll: Option<f32>,    // Roll in degrees
    mag_x: Option<f32>,   // Magnetometer X axis in uT
    mag_y: Option<f32>,   // Magnetometer Y axis in uT
    mag_z: Option<f32>,   // Magnetometer Z axis in uT
    accel_x: Option<f32>, // Accelerometer X axis in m/s^2
    accel_y: Option<f32>, // Accelerometer Y axis in m/s^2
    accel_z: Option<f32>, // Accelerometer Z axis in m/s^2
}

// IMU event message
struct ImuEvent {
    gyro_x: Option<f32>,  // Gyroscope X axis in deg/s
    gyro_y: Option<f32>,  // Gyroscope Y axis in deg/s
    gyro_z: Option<f32>,  // Gyroscope Z axis in deg/s
    accel_x: Option<f32>, // Accelerometer X axis in m/s^2
    accel_y: Option<f32>, // Accelerometer Y axis in m/s^2
    accel_z: Option<f32>, // Accelerometer Z axis in m/s^2
}

// Ultrasonic sensor event message
struct UsEvent {
    distance: Option<f32>,    // Distance in meters
    temperature: Option<f32>, // Temperature in Celsius
    status: Option<i32>,      // Status code, 0 if no error
}

// PS4 controller state event
struct Ps4Event {
    button_left: Option<bool>,
    button_right: Option<bool>,
    button_up: Option<bool>,
    button_down: Option<bool>,
    button_square: Option<bool>,
    button_cross: Option<bool>,
    button_circle: Option<bool>,
    button_triangle: Option<bool>,
    button_left_shoulder: Option<bool>,
    button_right_shoulder: Option<bool>,
    button_left_trigger: Option<bool>,
    button_right_trigger: Option<bool>,
    button_left_joystick: Option<bool>,
    button_right_joystick: Option<bool>,
    button_share: Option<bool>,
    button_options: Option<bool>,
    button_touchpad: Option<bool>,
    button_ps: Option<bool>,
    axis_lx: Option<i32>,
    axis_ly: Option<i32>,
    axis_rx: Option<i32>,
    axis_ry: Option<i32>,
    gyro_x: Option<i32>,
    gyro_y: Option<i32>,
    gyro_z: Option<i32>,
    accel_x: Option<i32>,
    accel_y: Option<i32>,
    accel_z: Option<i32>,
    connected: Option<bool>,
    battery_level: Option<i32>,
    bluetooth: Option<bool>,
    debug: Option<String>,
    temp: Option<i32>,
}

// PS4 controller output command
struct Ps4Request {
    req_id: Option<u32>,        // For request/reply matching, 0 if not a request/reply
    rumble_small: Option<i32>,
    rumble_large: Option<i32>,
    led_red: Option<i32>,
    led_green: Option<i32>,
    led_blue: Option<i32>,
    led_flash_on: Option<i32>,
    led_flash_off: Option<i32>,
}

// System request message
struct SysRequest {
    req_id: Option<u32>,    // For request/reply matching, 0 if not a request/reply
    set_time: Option<u64>,
    reboot: Option<bool>,
    console: Option<String>,
}

// System reply message
struct SysReply {
    req_id: Option<u32>,    // For request/reply matching, 0 if not a request/reply
    rc: Option<i32>,
    message: Option<String>,
}

// System event message
struct SysEvent {
    utc: Option<u64>,
    uptime: Option<u64>,
    free_heap: Option<u64>,
    flash_size: Option<u64>,
    cpu_board_type: Option<String>,
    build_date_time: Option<String>,
}

// Ping request message
struct PingRequest {
    req_id: Option<u32>,
    timestamp: Option<u64>, // Timestamp in milliseconds since epoch
}

// Ping reply message
struct PingReply {
    req_id: Option<u32>,
    timestamp: Option<u64>, // Timestamp in milliseconds since epoch
}

// Subscribe to a message type from a source endpoint
struct BrokerSubscribeRequest {
    src: Option<u32>,      // if no field is set, then subscribe to all sources
    msg_type: Option<u32>, // if no field is set, then subscribe to all message types
}

// Wifi event message
struct WifiEvent {
    ip: Option<String>,
    gateway: Option<String>,
    netmask: Option<String>,
    ssid: Option<String>,
    bssid: Option<String>,
    channel: Option<i32>,
    rssi: Option<i32>,
    mac: Option<String>,
}

// Device alive event message
struct DeviceAliveEvent {
    device: Option<String>,
    endpoint: Option<String>,
    timestamp: Option<u64>, // Timestamp in milliseconds since epoch
}

// Max31855 event message
struct Max31855Event {
    thermocouple_temp: Option<f32>, // Thermocouple temperature in Celsius
    internal_temp: Option<f32>,     // Internal temperature in Celsius
    fault: Option<bool>,            // Fault detected
    fault_short_vcc: Option<bool>,  // Short to VCC detected
    fault_short_gnd: Option<bool>,  // Short to GND detected
    fault_open_tc: Option<bool>,    // Open thermocouple detected
}

// Heating event message
struct HeatingEvent {
    temperature_c: Option<f32>, // Current temperature in Celsius
    setpoint_c: Option<f32>,    // Setpoint temperature in Celsius
    enabled: Option<bool>,      // Heating enabled or disabled
    output_pct: Option<f32>,    // Output percentage of the heating element
    heater_on: Option<bool>,    // Heater is currently on or off
    fault: Option<bool>,        // Fault detected in the heating system
    timestamp_ms: Option<u64>,  // Timestamp in milliseconds since epoch
}

// Heating request message
struct HeatingRequest {
    setpoint_c: Option<f32>,      // Setpoint temperature in Celsius
    enabled: Option<bool>,        // Enable or disable heating
    kp: Option<f32>,              // Proportional gain for PID controller
    ki: Option<f32>,              // Integral gain for PID controller
    kd: Option<f32>,              // Derivative gain for PID controller
    reset_integral: Option<bool>, // Reset the integral term of the PID controller
}

// Envelope message for encapsulating other messages
struct Envelope {
    src: Option<u32>,          // Source endpoint name
    dst: Option<u32>,          // Destination endpoint name
    msg_type: Option<u32>,     // Message type name
    sequence: Option<u32>,     // Request ID for matching request/reply
    instance_id: Option<u32>,  // Instance ID for matching request/reply
    payload: Option<Vec<u8>>,  // Serialized payload of the message
}

// Endpoint announce message for service discovery
struct EndpointAnnounce {
    id: Option<u32>,            // Unique identifier for the announcing endpoint
    name: Option<String>,       // Name of the announcing endpoint
    description: Option<String>, // Description of the announcing endpoint
    services: Vec<u32>,         // List of services provided by the endpoint
    events: Vec<u32>,           // List of events emitted by the endpoint
    replies: Vec<u32>,          // List of replies supported by the endpoint
    subscribes: Vec<u32>,       // List of subscriptions for the endpoint
}

// Endpoint announce reply message for service discovery
struct EndpointAnnounceReply {
    utc: Option<u64>, // Timestamp in milliseconds since epoch
}
