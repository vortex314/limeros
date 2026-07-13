robot "ronald" {
  model          = "Limero-v1"
  description    = "Hoverboard lawn mower"
  multicast_port = 50000
  broker_port   = 50001
  multicast_addr = "224.0.0.1"

  device "broker" {
    description = "Message broker for the hoverboard"
    mac         = "00:1A:7D:DA:71:12"
    mdns        = "broker"

    endpoint "broker" {
      services    = [$ { message.EndpointAnnounce }, $ { message.EndpointAnnounceReply }, $ { message.PingRequest }, $ { message.PingReply }]
      description = "Message broker for the hoverboard"
    }
  }

  device "pinger" {
    description = "Pinger device for testing"
    mac         = "00:1A:7D:DA:71:18"
    mdns        = "pinger"

    endpoint "pinger" {
      services    = [$ { message.PingRequest }, $ { message.PingReply }]
      description = "Pinger device for testing"
    }
  }

  device "sniffer" {
    description = "Sniffer device for all messages"
    mac         = "00:1A:7D:DA:71:17"
    mdns        = "sniffer"

    endpoint "sniffer" {
      subscribes  = [{ src = null msg_type = null dst = null }]
    }

    endpoint "tui_sniffer" {
      description = "Sniffer device for all messages"
      subscribes  = [{ src = null msg_type = null dst = null }]
    }

  }

  device compass {
    description = "Compass device"
    mac         = "00:1A:7D:DA:71:19"
    mdns        = "compass"

    endpoint "compass" {
      services    = [$ { message.SysRequest }]
      events      = [$ { message.SysEvent } , ${ message.CompassEvent}]
      description = "Compass device for the hoverboard"
    }
  }

  device "hoverboard" {
    description = "Motor controller for hoverboard"
    mac         = "00:1A:7D:DA:71:13"
    mdns        = "hoverboard"

    endpoint "hoverboard" {
      services    = [$ { message.HoverboardRequest }, $ { message.SysRequest }]
      events      = [$ { message.HoverboardEvent }, $ { message.SysEvent }]
      replies     = [$ { message.GenericReply }, $ { message.SysReply }]
      description = "Hoverboard to drive the mower"
    }
  }

  device "logger" {
    description = "Logger device"
    mac         = "00:1A:7D:DA:71:16"
    mdns        = "logger"

    endpoint "logger" {
      description = "Logger interface for the hoverboard"
      subscribes  = [{ src = "hoverboard" msg_type = "HoverboardEvent" dst = null }]
    }

  }

  device "ps4" {
    description = "PS4 controller for hoverboard"
    mac         = "00:1A:7D:DA:71:14"
    mdns        = "ps4"

    endpoint "ps4" {
      description = "PS4 controller for the hoverboard"
      services    = [$ { message.Ps4Request }, $ { message.SysRequest }]
      events      = [$ { message.Ps4Event }, $ { message.SysEvent }]
      replies    = [$ { message.GenericReply }, $ { message.SysReply }]
    }

  }

  device "mower" {
    description = "Mower device"
    mac         = "00:1A:7D:DA:71:15"
    mdns        = "mower"

    endpoint "mower" {
      services    = [$ { message.SystemRequest }]
      description = "System interface for the mower"
    }

  }

  message "HoverboardRequest" {
    description = "Request command for hoverboard drive"
    field "req_id" { id = 0 type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
    field "speed" { id = 1 type = "int32" unit = "m/s" description = "Speed command for the hoverboard" }
    field "steer" { id = 2 type = "int32" unit = "deg" description = "Steering command for the hoverboard" }
  }

  message "GenericReply" {
    description = "Reply for hoverboard request"
    field "req_id" { id = 0 type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
    field "error_code" { id = 1 type = "uint32" description = "Error code, 0 if no error" }
    field "message" { id = 2 type = "string" description = "Error message or additional information" }
    field "msg_type" { id = 3 type = "uint32" description = "Message type identifier , the original request" }
  }

  message "HoverboardEvent" {
    description = "Telemetry emitted by the hoverboard"
    field "ctrl_mod" { id = 0 type = "int32" description = "1:Voltage 2:Speed 3:Torque" }
    field "ctrl_typ" { id = 1 type = "int32" description = "0:Commutation 1:Sinusoidal 2:FOC" }
    field "cur_mot_max" { id = 2 type = "int32" description = "Max phase current A" }
    field "rpm_mot_max" { id = 3 type = "int32" description = "Max motor RPM" }
    field "fi_weak_ena" { id = 4 type = "int32" description = "Enable field weak 0:OFF 1:ON" }
    field "fi_weak_hi" { id = 5 type = "int32" description = "Field weak high RPM" }
    field "fi_weak_lo" { id = 6 type = "int32" description = "Field weak low RPM" }
    field "fi_weak_max" { id = 7   type = "int32" description = "Field weak max current A (FOC only)" }
    field "phase_adv_max_deg" { id = 8 type = "int32" description = "Max Phase Adv angle Deg (SIN only)" }
    field "input1_raw" { id = 9 type = "int32" description = "Input1 raw value" }
    field "input1_typ" { id = 10 type = "int32" description = "Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect" }
    field "input1_min" { id = 11 type = "int32" description = "Input1 minimum value" }
    field "input1_mid" { id = 12 type = "int32" description = "Input1 middle value" }
    field "input1_max" { id = 13 type = "int32" description = "Input1 maximum value" }
    field "input1_cmd" { id = 14 type = "int32" description = "Input1 command value" }
    field "input2_raw" { id = 15 type = "int32" description = "Input2 raw value" }
    field "input2_typ" { id = 16 type = "int32" description = "Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect" }
    field "input2_min" { id = 17 type = "int32" description = "Input2 minimum value" }
    field "input2_mid" { id = 18 type = "int32" description = "Input2 middle value" }
    field "input2_max" { id = 19 type = "int32" description = "Input2 maximum value" }
    field "input2_cmd" { id = 20 type = "int32" description = "Input2 command value" }
    field "aux_input1_raw" { id = 21 type = "int32" description = "Input1 raw value" }
    field "aux_input1_typ" { id = 22 type = "int32" description = "Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect" }
    field "aux_input1_min" { id = 23 type = "int32" description = "Input1 minimum value" }
    field "aux_input1_mid" { id = 24 type = "int32" description = "Input1 middle value" }
    field "aux_input1_max" { id = 25 type = "int32" description = "Input1 maximum value" }
    field "aux_input1_cmd" { id = 26 type = "int32" description = "Input1 command value" }
    field "aux_input2_raw" { id = 27 type = "int32" description = "Input2 raw value" }
    field "aux_input2_typ" {
      id = 28
      type        = "enum"
      description = "Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect"
      values = {
        0 = "Disabled"
        1 = "Normal Pot"
        2 = "Middle Resting Pot"
        3 = "Auto-detect"
      }
    }
    field "aux_input2_min" { id = 29 type = "int32" description = "Input2 minimum value" }
    field "aux_input2_mid" { id = 30 type = "int32" description = "Input2 middle value" }
    field "aux_input2_max" { id = 31 type = "int32" description = "Input2 maximum value" }
    field "aux_input2_cmd" { id = 32 type = "int32" description = "Input2 command value" }
    field "dc_curr" { id = 33 type = "float" description = "Total DC Link current A *100" }
    field "rdc_curr" { id = 34 type = "float" description = "Right DC Link current A *100" }
    field "ldc_curr" { id = 35 type = "float" description = "Left DC Link current A *100" }
    field "cmdl" { id = 36 type = "int32" description = "Left Motor Command RPM" }
    field "cmdr" { id = 37 type = "int32" description = "Right Motor Command RPM" }
    field "spd_avg" { id = 38 type = "int32" description = "Motor Measured Avg RPM" }
    field "spdl" { id = 39 type = "int32" description = "Left Motor Measured RPM" }
    field "spdr" { id = 40 type = "int32" description = "Right Motor Measured RPM" }
    field "filter_rate" { id = 41 type = "int32" description = "Rate *10" }
    field "spd_coef" { id = 42 type = "int32" description = "Speed Coefficient *10" }
    field "str_coef" { id = 43 type = "int32" description = "Steer Coefficient *10" }
    field "batv" { id = 44 type = "float" description = "Calibrated Battery Voltage *100" }
    field "temp" { id = 45 type = "float" description = "Calibrated Temperature C *10" }
  }

  message CompassEvent {
    description = "Compass event message"
    field "heading" { id = 0 type = "float" description = "Heading in degrees" }
    field "pitch" { id = 1 type = "float" description = "Pitch in degrees" }
    field "roll" { id = 2 type = "float" description = "Roll in degrees" }
    field "mag_x" { id = 3 type = "float" description = "Magnetometer X axis in uT" }
    field "mag_y" { id = 4 type = "float" description = "Magnetometer Y axis in uT" }
    field "mag_z" { id = 5 type = "float" description = "Magnetometer Z axis in uT" }
    field "accel_x" { id = 6 type = "float" description = "Accelerometer X axis in m/s^2" }
    field "accel_y" { id = 7 type = "float" description = "Accelerometer Y axis in m/s^2" }
    field "accel_z" { id = 8 type = "float" description = "Accelerometer Z axis in m/s^2" }
  }

  message "Ps4Event" {
    description = "PS4 controller state event"
    field "button_left" { id = 0 type = "bool" }
    field "button_right" { id = 1 type = "bool" }
    field "button_up" { id = 2 type = "bool" }
    field "button_down" { id = 3 type = "bool" }
    field "button_square" { id = 4 type = "bool" }
    field "button_cross" { id = 5 type = "bool" }
    field "button_circle" { id = 6 type = "bool" }
    field "button_triangle" { id = 7 type = "bool" }
    field "button_left_shoulder" { id = 8 type = "bool" }
    field "button_right_shoulder" { id = 9 type = "bool" }
    field "button_left_trigger" { id = 10 type = "bool" }
    field "button_right_trigger" { id = 11 type = "bool" }
    field "button_left_joystick" { id = 12 type = "bool" }
    field "button_right_joystick" { id = 13 type = "bool" }
    field "button_share" { id = 14 type = "bool" }
    field "button_options" { id = 15 type = "bool" }
    field "button_touchpad" { id = 16 type = "bool" }
    field "button_ps" { id = 17 type = "bool" }
    field "axis_lx" { id = 18 type = "int32" }
    field "axis_ly" { id = 19 type = "int32" }
    field "axis_rx" { id = 20 type = "int32" }
    field "axis_ry" { id = 21 type = "int32" }
    field "gyro_x" { id = 22 type = "int32" }
    field "gyro_y" { id = 23 type = "int32" }
    field "gyro_z" { id = 24 type = "int32" }
    field "accel_x" { id = 25 type = "int32" }
    field "accel_y" { id = 26 type = "int32" }
    field "accel_z" { id = 27 type = "int32" }
    field "connected" { id = 28 type = "bool" }
    field "battery_level" { id = 29 type = "int32" }
    field "bluetooth" { id = 30 type = "bool" }
    field "debug" { id = 31 type = "string" }
    field "temp" { id = 32 type = "int32" }
  }

  message "Ps4Request" {
    description = "PS4 controller output command"
    field "req_id" { id = 0 type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
    field "rumble_small" { id = 1 type = "int32" }
    field "rumble_large" { id = 2 type = "int32" }
    field "led_red" { id = 3 type = "int32" }
    field "led_green" { id = 4 type = "int32" }
    field "led_blue" { id = 5 type = "int32" }
    field "led_flash_on" { id = 6 type = "int32" }
    field "led_flash_off" { id = 7 type = "int32" }
  }

  message "SysRequest" {
    description = "System request message"
    field "req_id" { id = 0 type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
    field "set_time" { id = 1 type = "uint64" }
    field "reboot" { id = 2 type = "bool" }
    field "console" { id = 3 type = "string" }
  }

  message "SysReply" {
    description = "System reply message"
    field "req_id" { id = 0 type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
    field "rc" { id = 1 type = "int32" }
    field "message" { id = 2 type = "string" }
  }

  message "SysEvent" {
    description = "System event message"
    field "utc" { id = 0 type = "uint64" }
    field "uptime" { id = 1 type = "uint64" }
    field "free_heap" { id = 2 type = "uint64" }
    field "flash" { id = 3 type = "uint64" }
    field "cpu_board" { id = 4 type = "string" }
    field "build_date" { id = 5 type = "string" }
  }

  message PingRequest { // is send as a reply to EndpointAnnounce, on ip port and addr of the sender
    description = "Ping request message"
    field "req_id" { id = 0 type = "uint32" }
    field "timestamp" { id = 1 type = "uint64"  description = "Timestamp in milliseconds since epoch"}
  }

  message PingReply { // is send as a reply to EndpointAnnounce, on ip port and addr of the sender
    description = "Ping reply message"
    field "req_id" { id = 0 type = "uint32" }
    field "timestamp" { id = 1 type = "uint64"  description = "Timestamp in milliseconds since epoch"}
  }

  message BrokerSubscribeRequest {
    description = "Subscribe to a message type from a source endpoint"
    // if no field is set, then subscribe to all sources
    field "src" {
      id = 0
      type = "uint32"
    }
    // if no field is set, then subscribe to all message types
    field "msg_type" {
      id = 1
      type = "uint32"
    }
  }

  message WifiEvent {
    description = "Wifi event message"
    field "ip" { id = 0 type = "string" }
    field "gateway" { id = 1 type = "string" }
    field "netmask" { id = 2 type = "string" }
    field "ssid" { id = 3 type = "string" }
    field "bssid" { id = 4 type = "string" }
    field "channel" { id = 5 type = "int32" }
    field "rssi" { id = 6 type = "int32" }
    field "mac" { id = 7 type = "string" }
  }

  message DeviceAliveEvent {
    description = "Device alive event message"
    field "device" { id = 0 type = "string" }
    field "endpoint" { id = 1 type = "string" }
    field "timestamp" { id = 2 type = "uint64"  description = "Timestamp in milliseconds since epoch"}
  }

  message Max31855Event {
    description = "Max31855 event message"
    field "thermocouple_temp" { id = 0 type = "float"  description = "Thermocouple temperature in Celsius"}
    field "internal_temp" { id = 1 type = "float"  description = "Internal temperature in Celsius"}
    field "fault" { id = 2 type = "bool"  description = "Fault detected"}
    field "fault_short_vcc" { id = 3 type = "bool"  description = "Short to VCC detected"}
    field "fault_short_gnd" { id = 4 type = "bool"  description = "Short to GND detected"}
    field "fault_open_tc" { id = 5 type = "bool"  description = "Open thermocouple detected"}
  }

  message HeatingEvent {
    description = "Heating event message"
    field "temperature" { id = 0 type = "float"  description = "Current temperature in Celsius"}
    field "setpoint" { id = 1 type = "float"  description = "Setpoint temperature in Celsius"}
    field "heating" { id = 2 type = "bool"  description = "Heating status"}
  }

  message Envelope {
    description = "Envelope message for encapsulating other messages"
    field "src" { id = 0 type = "uint32"  description = "Source endpoint name"}
    field "dst" { id = 1 type = "uint32"  description = "Destination endpoint name"}
    field "msg_type" { id = 2 type = "uint32"  description = "Message type name"}
    field "request_id" { id = 3 type = "uint32"  description = "Request ID for matching request/reply"}
    field "instance_id" { id = 4 type = "uint32"  description = "Instance ID for matching request/reply"}
    field "payload" { id = 5 type = "bytes"  description = "Serialized payload of the message"}
  }

  message EndpointAnnounce {
    field "id" { id = 0 type = "uint32"  description = "Unique identifier for the announcing endpoint"}
    description = "Endpoint announce message for service discovery"
    field "name" { id = 1 type = "string"  description = "Name of the announcing endpoint"}
    field "services" { id = 2 type = "uint32[]"  description = "List of services provided by the endpoint"}
    field "events" { id = 3 type = "uint32[]"  description = "List of events emitted by the endpoint"}
    field "replies" { id = 4 type = "uint32[]"  description = "List of replies supported by the endpoint"}
    field "subscribes" { id = 5 type = "uint32[]"  description = "List of subscriptions for the endpoint"}
  }

  message EndpointAnnounceReply {
  }



}
