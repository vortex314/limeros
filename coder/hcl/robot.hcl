robot "ronald" {
  model       = "Limero-v1"
  description = "Hoverboard lawn mower"
  multicast_port = 12345

  device "hoverboard" {
    description = "Motor controller for hoverboard"
    mac = "00:1A:7D:DA:71:13"
    mdns = "hoverboard"

    endpoint "hoverboard" {
      interface   = $ { interface.hoverboard }
      description = "Hoverboard to drive the mower"
    }
    endpoint "system" {
      interface   = $ { interface.System }
      description = "System interface for the hoverboard"
    }
  }

  device "logger" {
    description = "Logger device"
    mac = "00:1A:7D:DA:71:16"
    mdns = "logger"

    endpoint "logger" {
      interface   = $ { interface.Logger }
      description = "Logger interface for the hoverboard"
    }

    subscriptions = [
            { device = "hoverboard", source = "hoverboard", message = "HoverboardEvent" },
            { device = "hoverboard", source = "system", message = "SysEvent" }
        ]
  }

  device "ps4" {
    description = "PS4 controller for hoverboard"
    mac = "00:1A:7D:DA:71:14"
    mdns = "ps4"

    endpoint "ps4" {
      interface   = $ { interface.ps4 }
      description = "PS4 controller for the hoverboard"
    }
    endpoint "system" {
      interface   = $ { interface.System }
      description = "System interface for the hoverboard"
    }

  }

  device "mower" {
    description = "Mower device"
    mac = "00:1A:7D:DA:71:15"
    mdns = "mower"

    endpoint "system" {
      interface   = $ { interface.System }
      description = "System interface for the mower"
    }

    subscriptions = [
            { device = "hoverboard", source = "hoverboard", message = "HoverboardEvent" },
            { device = "hoverboard", source = "system", message = "SysEvent" }
        ]
  }

  interface "hoverboard" {
    description = "Interface for controlling the hoverboard drive system"

    message "HoverboardRequest" {
      description = "Request command for hoverboard drive"
      field "req_id" { type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
      field "speed" { type = "int32" unit = "m/s" description = "Speed command for the hoverboard" }
      field "steer" { type = "int32" unit = "deg" description = "Steering command for the hoverboard" }
    }

    message "HoverboardReply" {
      description = "Reply for hoverboard request"
      field "req_id" { type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
      field "error_code" { type = "int32" }
      field "message" { type = "string" }
    }

    message "HoverboardEvent" {
      description = "Telemetry emitted by the hoverboard"
      field "ctrl_mod" { type = "int32" description = "1:Voltage 2:Speed 3:Torque" }
      field "ctrl_typ" { type = "int32" description = "0:Commutation 1:Sinusoidal 2:FOC" }
      field "cur_mot_max" { type = "int32" description = "Max phase current A" }
      field "rpm_mot_max" { type = "int32" description = "Max motor RPM" }
      field "fi_weak_ena" { type = "int32" description = "Enable field weak 0:OFF 1:ON" }
      field "fi_weak_hi" { type = "int32" description = "Field weak high RPM" }
      field "fi_weak_lo" { type = "int32" description = "Field weak low RPM" }
      field "fi_weak_max" { type = "int32" description = "Field weak max current A (FOC only)" }
      field "phase_adv_max_deg" { type = "int32" description = "Max Phase Adv angle Deg (SIN only)" }
      field "input1_raw" { type = "int32" description = "Input1 raw value" }
      field "input1_typ" { type = "int32" description = "Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect" }
      field "input1_min" { type = "int32" description = "Input1 minimum value" }
      field "input1_mid" { type = "int32" description = "Input1 middle value" }
      field "input1_max" { type = "int32" description = "Input1 maximum value" }
      field "input1_cmd" { type = "int32" description = "Input1 command value" }
      field "input2_raw" { type = "int32" description = "Input2 raw value" }
      field "input2_typ" { type = "int32" description = "Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect" }
      field "input2_min" { type = "int32" description = "Input2 minimum value" }
      field "input2_mid" { type = "int32" description = "Input2 middle value" }
      field "input2_max" { type = "int32" description = "Input2 maximum value" }
      field "input2_cmd" { type = "int32" description = "Input2 command value" }
      field "aux_input1_raw" { type = "int32" description = "Input1 raw value" }
      field "aux_input1_typ" { type = "int32" description = "Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect" }
      field "aux_input1_min" { type = "int32" description = "Input1 minimum value" }
      field "aux_input1_mid" { type = "int32" description = "Input1 middle value" }
      field "aux_input1_max" { type = "int32" description = "Input1 maximum value" }
      field "aux_input1_cmd" { type = "int32" description = "Input1 command value" }
      field "aux_input2_raw" { type = "int32" description = "Input2 raw value" }
      field "aux_input2_typ" { 
        type = "enum" 
        description = "Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect" 
        values = {
          0 = "Disabled"
          1 = "Normal Pot"
          2 = "Middle Resting Pot"
          3 = "Auto-detect"
        }
      }
      field "aux_input2_min" { type = "int32" description = "Input2 minimum value" }
      field "aux_input2_mid" { type = "int32" description = "Input2 middle value" }
      field "aux_input2_max" { type = "int32" description = "Input2 maximum value" }
      field "aux_input2_cmd" { type = "int32" description = "Input2 command value" }
      field "dc_curr" { type = "float" description = "Total DC Link current A *100" }
      field "rdc_curr" { type = "float" description = "Right DC Link current A *100" }
      field "ldc_curr" { type = "float" description = "Left DC Link current A *100" }
      field "cmdl" { type = "int32" description = "Left Motor Command RPM" }
      field "cmdr" { type = "int32" description = "Right Motor Command RPM" }
      field "spd_avg" { type = "int32" description = "Motor Measured Avg RPM" }
      field "spdl" { type = "int32" description = "Left Motor Measured RPM" }
      field "spdr" { type = "int32" description = "Right Motor Measured RPM" }
      field "filter_rate" { type = "int32" description = "Rate *10" }
      field "spd_coef" { type = "int32" description = "Speed Coefficient *10" }
      field "str_coef" { type = "int32" description = "Steer Coefficient *10" }
      field "batv" { type = "float" description = "Calibrated Battery Voltage *100" }
      field "temp" { type = "float" description = "Calibrated Temperature C *10" }
    }

  }

  interface "ps4" {
    description = "Interface for PS4 controller input and output"

    message "Ps4Event" {
      description = "PS4 controller state event"
      field "button_left" { type = "bool" }
      field "button_right" { type = "bool" }
      field "button_up" { type = "bool" }
      field "button_down" { type = "bool" }
      field "button_square" { type = "bool" }
      field "button_cross" { type = "bool" }
      field "button_circle" { type = "bool" }
      field "button_triangle" { type = "bool" }
      field "button_left_shoulder" { type = "bool" }
      field "button_right_shoulder" { type = "bool" }
      field "button_left_trigger" { type = "bool" }
      field "button_right_trigger" { type = "bool" }
      field "button_left_joystick" { type = "bool" }
      field "button_right_joystick" { type = "bool" }
      field "button_share" { type = "bool" }
      field "button_options" { type = "bool" }
      field "button_touchpad" { type = "bool" }
      field "button_ps" { type = "bool" }
      field "axis_lx" { type = "int32" }
      field "axis_ly" { type = "int32" }
      field "axis_rx" { type = "int32" }
      field "axis_ry" { type = "int32" }
      field "gyro_x" { type = "int32" }
      field "gyro_y" { type = "int32" }
      field "gyro_z" { type = "int32" }
      field "accel_x" { type = "int32" }
      field "accel_y" { type = "int32" }
      field "accel_z" { type = "int32" }
      field "connected" { type = "bool" }
      field "battery_level" { type = "int32" }
      field "bluetooth" { type = "bool" }
      field "debug" { type = "string" }
      field "temp" { type = "int32" }
    }

    message "Ps4Request" {
      description = "PS4 controller output command"
      field "req_id" { type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
      field "rumble_small" { type = "int32" }
      field "rumble_large" { type = "int32" }
      field "led_red" { type = "int32" }
      field "led_green" { type = "int32" }
      field "led_blue" { type = "int32" }
      field "led_flash_on" { type = "int32" }
      field "led_flash_off" { type = "int32" }
    }
  }

  interface "System" {
    description = "Interface for controlling the hoverboard system"

    message "SysRequest" {
      description = "System request message"
      field "req_id" { type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
      field "set_time" { type = "uint64" }
      field "reboot" { type = "bool" }
      field "console" { type = "string" }
    }

    message "SysReply" {
      description = "System reply message"
      field "req_id" { type = "uint32" description = "For request/reply matching, 0 if not a request/reply" }
      field "rc" { type = "int32" }
      field "message" { type = "string" }
    }

    message "SysEvent" {
      description = "System event message"
      field "utc" { type = "uint64" }
      field "uptime" { type = "uint64" }
      field "free_heap" { type = "uint64" }
      field "flash" { type = "uint64" }
      field "cpu_board" { type = "string" }
      field "build_date" { type = "string" }
    }
  }
}
