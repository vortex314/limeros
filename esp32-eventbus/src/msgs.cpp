// Auto-generated from robot.hcl — do not edit by hand.
// Source file — method implementations for generated messages.
// Messages are encoded as CBOR maps keyed by field id.
// Uses TinyCBOR for CBOR encoding/decoding.
#include "msgs.h"
#include "cbor_codec.h"


// ── Name lookup tables ─────────────────────────────────────────────────────

std::unordered_map<uint32_t, const char*> id_to_name = {
    { 3190208493, "BrokerSubscribeRequest" },
    { 3197332525, "CompassEvent" },
    { 3974472274, "CutterEvent" },
    { 3901793882, "CutterReply" },
    { 2133047659, "CutterRequest" },
    { 2637772092, "DeviceAliveEvent" },
    { 2371693343, "EndpointAnnounce" },
    { 3238220441, "EndpointAnnounceReply" },
    { 1228864117, "Envelope" },
    { 2578784998, "GenericReply" },
    { 461737375, "HeatingEvent" },
    { 578653874, "HeatingRequest" },
    { 104988481, "HoverboardEvent" },
    { 2095960949, "HoverboardReply" },
    { 2735870956, "HoverboardRequest" },
    { 1802836182, "ImuEvent" },
    { 2831607083, "Max31855Event" },
    { 1594103907, "PingReply" },
    { 31253678, "PingRequest" },
    { 4282593576, "Ps4Event" },
    { 1992038561, "Ps4Request" },
    { 924742914, "SysEvent" },
    { 2952492394, "SysReply" },
    { 2966412411, "SysRequest" },
    { 1082063571, "UsEvent" },
    { 3371536624, "WifiEvent" },
    { 2200474099, "brain" },
    { 2490238132, "broker" },
    { 2753264687, "compass" },
    { 1152836275, "hoverboard" },
    { 2753177333, "logger" },
    { 1092332049, "mower" },
    { 1295055938, "pinger" },
    { 360195552, "ps4" },
    { 836480628, "sniffer" },
    { 3577618233, "tui_sniffer" },
};

const char *id_to_string(uint32_t msg_id)
{
  auto it = id_to_name.find(msg_id);
  if (it != id_to_name.end())
  {
    return it->second;
  }
  return "Unknown";
}

// ── Message encode/decode implementations ──────────────────────────────────



int BrokerSubscribeRequest::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(2)
    .write(src)
    .write(msg_type).err();
}

int BrokerSubscribeRequest::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(2)
    .read(src)
    .read(msg_type)
    .err();
}



int CompassEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(9)
    .write(heading)
    .write(pitch)
    .write(roll)
    .write(mag_x)
    .write(mag_y)
    .write(mag_z)
    .write(accel_x)
    .write(accel_y)
    .write(accel_z).err();
}

int CompassEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(9)
    .read(heading)
    .read(pitch)
    .read(roll)
    .read(mag_x)
    .read(mag_y)
    .read(mag_z)
    .read(accel_x)
    .read(accel_y)
    .read(accel_z)
    .err();
}



int CutterEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(5)
    .write(enabled)
    .write(rpm)
    .write(current)
    .write(voltage)
    .write(temperature).err();
}

int CutterEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(5)
    .read(enabled)
    .read(rpm)
    .read(current)
    .read(voltage)
    .read(temperature)
    .err();
}



int CutterReply::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(4)
    .write(req_id)
    .write(error_code)
    .write(message)
    .write(msg_type).err();
}

int CutterReply::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(4)
    .read(req_id)
    .read(error_code)
    .read(message)
    .read(msg_type)
    .err();
}



int CutterRequest::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(2)
    .write(enabled)
    .write(rpm).err();
}

int CutterRequest::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(2)
    .read(enabled)
    .read(rpm)
    .err();
}



int DeviceAliveEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(3)
    .write(device)
    .write(endpoint)
    .write(timestamp).err();
}

int DeviceAliveEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(3)
    .read(device)
    .read(endpoint)
    .read(timestamp)
    .err();
}



int EndpointAnnounce::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(7)
    .write(id)
    .write(name)
    .write(description)
    .write(services)
    .write(events)
    .write(replies)
    .write(subscribes).err();
}

int EndpointAnnounce::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(7)
    .read(id)
    .read(name)
    .read(description)
    .read(services)
    .read(events)
    .read(replies)
    .read(subscribes)
    .err();
}



int EndpointAnnounceReply::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(1)
    .write(utc).err();
}

int EndpointAnnounceReply::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(1)
    .read(utc)
    .err();
}



int Envelope::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(6)
    .write(src)
    .write(dst)
    .write(msg_type)
    .write(sequence)
    .write(instance_id)
    .write(payload).err();
}

int Envelope::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(6)
    .read(src)
    .read(dst)
    .read(msg_type)
    .read(sequence)
    .read(instance_id)
    .read(payload)
    .err();
}



int GenericReply::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(4)
    .write(req_id)
    .write(error_code)
    .write(message)
    .write(msg_type).err();
}

int GenericReply::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(4)
    .read(req_id)
    .read(error_code)
    .read(message)
    .read(msg_type)
    .err();
}



int HeatingEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(7)
    .write(temperature_c)
    .write(setpoint_c)
    .write(enabled)
    .write(output_pct)
    .write(heater_on)
    .write(fault)
    .write(timestamp_ms).err();
}

int HeatingEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(7)
    .read(temperature_c)
    .read(setpoint_c)
    .read(enabled)
    .read(output_pct)
    .read(heater_on)
    .read(fault)
    .read(timestamp_ms)
    .err();
}



int HeatingRequest::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(6)
    .write(setpoint_c)
    .write(enabled)
    .write(kp)
    .write(ki)
    .write(kd)
    .write(reset_integral).err();
}

int HeatingRequest::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(6)
    .read(setpoint_c)
    .read(enabled)
    .read(kp)
    .read(ki)
    .read(kd)
    .read(reset_integral)
    .err();
}



int HoverboardEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(46)
    .write(ctrl_mod)
    .write(ctrl_typ)
    .write(cur_mot_max)
    .write(rpm_mot_max)
    .write(fi_weak_ena)
    .write(fi_weak_hi)
    .write(fi_weak_lo)
    .write(fi_weak_max)
    .write(phase_adv_max_deg)
    .write(input1_raw)
    .write(input1_typ)
    .write(input1_min)
    .write(input1_mid)
    .write(input1_max)
    .write(input1_cmd)
    .write(input2_raw)
    .write(input2_typ)
    .write(input2_min)
    .write(input2_mid)
    .write(input2_max)
    .write(input2_cmd)
    .write(aux_input1_raw)
    .write(aux_input1_typ)
    .write(aux_input1_min)
    .write(aux_input1_mid)
    .write(aux_input1_max)
    .write(aux_input1_cmd)
    .write(aux_input2_raw)
    .write(aux_input2_typ)
    .write(aux_input2_min)
    .write(aux_input2_mid)
    .write(aux_input2_max)
    .write(aux_input2_cmd)
    .write(dc_curr)
    .write(rdc_curr)
    .write(ldc_curr)
    .write(cmdl)
    .write(cmdr)
    .write(spd_avg)
    .write(spdl)
    .write(spdr)
    .write(filter_rate)
    .write(spd_coef)
    .write(str_coef)
    .write(batv)
    .write(temp).err();
}

int HoverboardEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(46)
    .read(ctrl_mod)
    .read(ctrl_typ)
    .read(cur_mot_max)
    .read(rpm_mot_max)
    .read(fi_weak_ena)
    .read(fi_weak_hi)
    .read(fi_weak_lo)
    .read(fi_weak_max)
    .read(phase_adv_max_deg)
    .read(input1_raw)
    .read(input1_typ)
    .read(input1_min)
    .read(input1_mid)
    .read(input1_max)
    .read(input1_cmd)
    .read(input2_raw)
    .read(input2_typ)
    .read(input2_min)
    .read(input2_mid)
    .read(input2_max)
    .read(input2_cmd)
    .read(aux_input1_raw)
    .read(aux_input1_typ)
    .read(aux_input1_min)
    .read(aux_input1_mid)
    .read(aux_input1_max)
    .read(aux_input1_cmd)
    .read(aux_input2_raw)
    .read(aux_input2_typ)
    .read(aux_input2_min)
    .read(aux_input2_mid)
    .read(aux_input2_max)
    .read(aux_input2_cmd)
    .read(dc_curr)
    .read(rdc_curr)
    .read(ldc_curr)
    .read(cmdl)
    .read(cmdr)
    .read(spd_avg)
    .read(spdl)
    .read(spdr)
    .read(filter_rate)
    .read(spd_coef)
    .read(str_coef)
    .read(batv)
    .read(temp)
    .err();
}



int HoverboardReply::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(2)
    .write(speed)
    .write(steer).err();
}

int HoverboardReply::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(2)
    .read(speed)
    .read(steer)
    .err();
}



int HoverboardRequest::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(2)
    .write(speed)
    .write(steer).err();
}

int HoverboardRequest::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(2)
    .read(speed)
    .read(steer)
    .err();
}



int ImuEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(6)
    .write(gyro_x)
    .write(gyro_y)
    .write(gyro_z)
    .write(accel_x)
    .write(accel_y)
    .write(accel_z).err();
}

int ImuEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(6)
    .read(gyro_x)
    .read(gyro_y)
    .read(gyro_z)
    .read(accel_x)
    .read(accel_y)
    .read(accel_z)
    .err();
}



int Max31855Event::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(6)
    .write(thermocouple_temp)
    .write(internal_temp)
    .write(fault)
    .write(fault_short_vcc)
    .write(fault_short_gnd)
    .write(fault_open_tc).err();
}

int Max31855Event::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(6)
    .read(thermocouple_temp)
    .read(internal_temp)
    .read(fault)
    .read(fault_short_vcc)
    .read(fault_short_gnd)
    .read(fault_open_tc)
    .err();
}



int PingReply::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(2)
    .write(req_id)
    .write(timestamp).err();
}

int PingReply::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(2)
    .read(req_id)
    .read(timestamp)
    .err();
}



int PingRequest::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(2)
    .write(req_id)
    .write(timestamp).err();
}

int PingRequest::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(2)
    .read(req_id)
    .read(timestamp)
    .err();
}



int Ps4Event::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(33)
    .write(button_left)
    .write(button_right)
    .write(button_up)
    .write(button_down)
    .write(button_square)
    .write(button_cross)
    .write(button_circle)
    .write(button_triangle)
    .write(button_left_shoulder)
    .write(button_right_shoulder)
    .write(button_left_trigger)
    .write(button_right_trigger)
    .write(button_left_joystick)
    .write(button_right_joystick)
    .write(button_share)
    .write(button_options)
    .write(button_touchpad)
    .write(button_ps)
    .write(axis_lx)
    .write(axis_ly)
    .write(axis_rx)
    .write(axis_ry)
    .write(gyro_x)
    .write(gyro_y)
    .write(gyro_z)
    .write(accel_x)
    .write(accel_y)
    .write(accel_z)
    .write(connected)
    .write(battery_level)
    .write(bluetooth)
    .write(debug)
    .write(temp).err();
}

int Ps4Event::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(33)
    .read(button_left)
    .read(button_right)
    .read(button_up)
    .read(button_down)
    .read(button_square)
    .read(button_cross)
    .read(button_circle)
    .read(button_triangle)
    .read(button_left_shoulder)
    .read(button_right_shoulder)
    .read(button_left_trigger)
    .read(button_right_trigger)
    .read(button_left_joystick)
    .read(button_right_joystick)
    .read(button_share)
    .read(button_options)
    .read(button_touchpad)
    .read(button_ps)
    .read(axis_lx)
    .read(axis_ly)
    .read(axis_rx)
    .read(axis_ry)
    .read(gyro_x)
    .read(gyro_y)
    .read(gyro_z)
    .read(accel_x)
    .read(accel_y)
    .read(accel_z)
    .read(connected)
    .read(battery_level)
    .read(bluetooth)
    .read(debug)
    .read(temp)
    .err();
}



int Ps4Request::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(8)
    .write(req_id)
    .write(rumble_small)
    .write(rumble_large)
    .write(led_red)
    .write(led_green)
    .write(led_blue)
    .write(led_flash_on)
    .write(led_flash_off).err();
}

int Ps4Request::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(8)
    .read(req_id)
    .read(rumble_small)
    .read(rumble_large)
    .read(led_red)
    .read(led_green)
    .read(led_blue)
    .read(led_flash_on)
    .read(led_flash_off)
    .err();
}



int SysEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(6)
    .write(utc)
    .write(uptime)
    .write(free_heap)
    .write(flash_size)
    .write(cpu_board_type)
    .write(build_date_time).err();
}

int SysEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(6)
    .read(utc)
    .read(uptime)
    .read(free_heap)
    .read(flash_size)
    .read(cpu_board_type)
    .read(build_date_time)
    .err();
}



int SysReply::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(3)
    .write(req_id)
    .write(rc)
    .write(message).err();
}

int SysReply::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(3)
    .read(req_id)
    .read(rc)
    .read(message)
    .err();
}



int SysRequest::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(4)
    .write(req_id)
    .write(set_time)
    .write(reboot)
    .write(console).err();
}

int SysRequest::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(4)
    .read(req_id)
    .read(set_time)
    .read(reboot)
    .read(console)
    .err();
}



int UsEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(3)
    .write(distance)
    .write(temperature)
    .write(status).err();
}

int UsEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(3)
    .read(distance)
    .read(temperature)
    .read(status)
    .err();
}



int WifiEvent::encode(Buffer& buffer) const {
    return CborEncoder(buffer)
    .beginArray(8)
    .write(ip)
    .write(gateway)
    .write(netmask)
    .write(ssid)
    .write(bssid)
    .write(channel)
    .write(rssi)
    .write(mac).err();
}

int WifiEvent::decode(Buffer& buffer) {
    return CborDecoder(buffer)
    .beginArray(8)
    .read(ip)
    .read(gateway)
    .read(netmask)
    .read(ssid)
    .read(bssid)
    .read(channel)
    .read(rssi)
    .read(mac)
    .err();
}


