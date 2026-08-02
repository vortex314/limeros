//! Ps4Reader — reads a DualShock 4 controller via HID and publishes
//! Ps4Event messages through the UdpEndpoint.

use std::{sync::Arc, time::Duration};

use anyhow::Result;
use generated::generated::{Ps4Event, Ps4Request};
use hidapi::HidApi;
use kameo::prelude::*;
use log::{debug, error, info, warn};

// ── Constants ──────────────────────────────────────────────────────────────

const VID_SONY: u16 = 0x054C;
const PID_DS4_V1: u16 = 0x05C4;
const PID_DS4_V2: u16 = 0x09CC;


// ── Ps4Reader actor ────────────────────────────────────────────────────────



// ── HID report parsing ─────────────────────────────────────────────────────
// Ported from zenoh-linux-eventbus-rs/ps4_actor.rs

fn empty_ps4_event() -> Ps4Event {
    Ps4Event {
        button_left: None,
        button_right: None,
        button_up: None,
        button_down: None,
        button_square: None,
        button_cross: None,
        button_circle: None,
        button_triangle: None,
        button_left_shoulder: None,
        button_right_shoulder: None,
        button_left_trigger: None,
        button_right_trigger: None,
        button_left_joystick: None,
        button_right_joystick: None,
        button_share: None,
        button_options: None,
        button_touchpad: None,
        button_ps: None,
        axis_lx: None,
        axis_ly: None,
        axis_rx: None,
        axis_ry: None,
        gyro_x: None,
        gyro_y: None,
        gyro_z: None,
        accel_x: None,
        accel_y: None,
        accel_z: None,
        connected: None,
        battery_level: None,
        bluetooth: None,
        debug: None,
        temp: None,
    }
}

fn check_change(prev: &Ps4Event, current: &Ps4Event) -> bool {
    prev.button_up != current.button_up
        || prev.button_down != current.button_down
        || prev.button_left != current.button_left
        || prev.button_right != current.button_right
        || prev.button_circle != current.button_circle
        || prev.button_cross != current.button_cross
        || prev.button_square != current.button_square
        || prev.button_triangle != current.button_triangle
        || prev.button_left_trigger != current.button_left_trigger
        || prev.button_right_trigger != current.button_right_trigger
        || prev.button_left_shoulder != current.button_left_shoulder
        || prev.button_right_shoulder != current.button_right_shoulder
        || prev.button_options != current.button_options
        || prev.button_share != current.button_share
        || prev.button_left_joystick != current.button_left_joystick
        || prev.button_right_joystick != current.button_right_joystick
        || prev.button_ps != current.button_ps
        || prev.button_touchpad != current.button_touchpad
        || prev.axis_lx != current.axis_lx
        || prev.axis_ly != current.axis_ly
        || prev.axis_rx != current.axis_rx
        || prev.axis_ry != current.axis_ry
}

fn find_changes(prev: &Ps4Event, current: &Ps4Event) -> Ps4Event {
    let mut changes = empty_ps4_event();

    /*    if prev.accel_x != current.accel_x {
        changes.accel_x = current.accel_x;
    }
    if prev.accel_y != current.accel_y {
        changes.accel_y = current.accel_y;
    }
    if prev.accel_z != current.accel_z {
        changes.accel_z = current.accel_z;
    }
    if prev.gyro_x != current.gyro_x {
        changes.gyro_x = current.gyro_x;
    }
    if prev.gyro_y != current.gyro_y {
        changes.gyro_y = current.gyro_y;
    }
    if prev.gyro_z != current.gyro_z {
        changes.gyro_z = current.gyro_z;
    }*/
    if prev.axis_lx != current.axis_lx {
        changes.axis_lx = current.axis_lx;
    }
    if prev.axis_ly != current.axis_ly {
        changes.axis_ly = current.axis_ly;
    }
    if prev.axis_rx != current.axis_rx {
        changes.axis_rx = current.axis_rx;
    }
    if prev.axis_ry != current.axis_ry {
        changes.axis_ry = current.axis_ry;
    }
    // Emit both press and release transitions so consumers see the full
    // button state change (not just rising edges).
    if prev.button_up != current.button_up {
        changes.button_up = current.button_up;
    }
    if prev.button_down != current.button_down {
        changes.button_down = current.button_down;
    }
    if prev.button_left != current.button_left {
        changes.button_left = current.button_left;
    }
    if prev.button_right != current.button_right {
        changes.button_right = current.button_right;
    }
    if prev.button_circle != current.button_circle {
        changes.button_circle = current.button_circle;
    }
    if prev.button_cross != current.button_cross {
        changes.button_cross = current.button_cross;
    }
    if prev.button_square != current.button_square {
        changes.button_square = current.button_square;
    }
    if prev.button_triangle != current.button_triangle {
        changes.button_triangle = current.button_triangle;
    }
    if prev.button_left_trigger != current.button_left_trigger {
        changes.button_left_trigger = current.button_left_trigger;
    }
    if prev.button_right_trigger != current.button_right_trigger {
        changes.button_right_trigger = current.button_right_trigger;
    }
    if prev.button_left_shoulder != current.button_left_shoulder {
        changes.button_left_shoulder = current.button_left_shoulder;
    }
    if prev.button_right_shoulder != current.button_right_shoulder {
        changes.button_right_shoulder = current.button_right_shoulder;
    }
    if prev.button_options != current.button_options {
        changes.button_options = current.button_options;
    }
    if prev.button_share != current.button_share {
        changes.button_share = current.button_share;
    }
    if prev.button_left_joystick != current.button_left_joystick {
        changes.button_left_joystick = current.button_left_joystick;
    }
    if prev.button_right_joystick != current.button_right_joystick {
        changes.button_right_joystick = current.button_right_joystick;
    }
    if prev.button_ps != current.button_ps {
        changes.button_ps = current.button_ps;
    }
    if prev.button_touchpad != current.button_touchpad {
        changes.button_touchpad = current.button_touchpad;
    }
    if prev.battery_level != current.battery_level {
        changes.battery_level = current.battery_level;
    }
    if prev.temp != current.temp {
        changes.temp = current.temp;
    }
    if prev.bluetooth != current.bluetooth {
        changes.bluetooth = current.bluetooth;
    }
    changes
}

fn parse_input_report(buf: &[u8], bluetooth: bool) -> Ps4Event {
    let offset = if bluetooth { 2 } else { 0 };

    let dpad_hat = buf[5 + offset] & 0x0F;
    let right_buttons = buf[5 + offset] & 0xF0;

    let mut ev = empty_ps4_event();

    ev.bluetooth = Some(bluetooth);

    ev.accel_x = Some(i16::from_le_bytes([buf[19 + offset], buf[20 + offset]]) as i32);
    ev.accel_y = Some(i16::from_le_bytes([buf[21 + offset], buf[22 + offset]]) as i32);
    ev.accel_z = Some(i16::from_le_bytes([buf[23 + offset], buf[24 + offset]]) as i32);

    ev.gyro_x = Some(i16::from_le_bytes([buf[13 + offset], buf[14 + offset]]) as i32);
    ev.gyro_y = Some(i16::from_le_bytes([buf[15 + offset], buf[16 + offset]]) as i32);
    ev.gyro_z = Some(i16::from_le_bytes([buf[17 + offset], buf[18 + offset]]) as i32);

    ev.axis_lx = Some(buf[1 + offset] as i32 - 128);
    ev.axis_ly = Some(-(buf[2 + offset] as i32 - 128));
    ev.axis_rx = Some(buf[3 + offset] as i32 - 128);
    ev.axis_ry = Some(-(buf[4 + offset] as i32 - 128));

    ev.button_up = Some(dpad_hat == 0 || dpad_hat == 1 || dpad_hat == 7);
    ev.button_down = Some(dpad_hat == 3 || dpad_hat == 4 || dpad_hat == 5);
    ev.button_left = Some(dpad_hat == 5 || dpad_hat == 6 || dpad_hat == 7);
    ev.button_right = Some(dpad_hat == 1 || dpad_hat == 2 || dpad_hat == 3);

    ev.button_circle = Some((right_buttons & 0x40) != 0);
    ev.button_cross = Some((right_buttons & 0x20) != 0);
    ev.button_square = Some((right_buttons & 0x10) != 0);
    ev.button_triangle = Some((right_buttons & 0x80) != 0);

    ev.button_left_trigger = Some((buf[8 + offset] & 0xFF) != 0);
    ev.button_right_trigger = Some((buf[9 + offset] & 0xFF) != 0);

    ev.button_left_shoulder = Some((buf[6 + offset] & 0x01) != 0);
    ev.button_right_shoulder = Some((buf[6 + offset] & 0x02) != 0);
    ev.button_share = Some((buf[6 + offset] & 0x10) != 0);
    ev.button_options = Some((buf[6 + offset] & 0x20) != 0);
    ev.button_left_joystick = Some((buf[6 + offset] & 0x40) != 0);
    ev.button_right_joystick = Some((buf[6 + offset] & 0x80) != 0);

    ev.button_ps = Some((buf[7 + offset] & 0x01) != 0);
    ev.button_touchpad = Some((buf[7 + offset] & 0x02) != 0);

    ev.battery_level = Some((buf[30 + offset] & 0x0F) as i32);
    ev.temp = Some(buf[12 + offset] as i32);

    ev.debug = Some(format!(
        "gyro=({},{},{})",
        ev.gyro_x.unwrap_or(0),
        ev.gyro_y.unwrap_or(0),
        ev.gyro_z.unwrap_or(0),
    ));

    ev
}
pub struct Ps4Bridge {
    listener: Recipient<Ps4Event>,
}

impl Ps4Bridge {
    pub fn new(listener: Recipient<Ps4Event>) -> Self {
        Ps4Bridge { listener }
    }

    fn find_controller(api: &HidApi) -> Result<(u16, u16)> {
        for dev in api.device_list() {
            if dev.vendor_id() == VID_SONY
                && (dev.product_id() == PID_DS4_V1 || dev.product_id() == PID_DS4_V2)
            {
                return Ok((dev.vendor_id(), dev.product_id()));
            }
        }
        anyhow::bail!("PS4 controller not found. Is it connected?");
    }

    /// Spawns a blocking HID receive loop that retries connection and
    /// re-discovery indefinitely — mirrors the serial actor's retry shape.
    fn spawn_receive_loop(recipient: Recipient<Ps4Event>) -> tokio::task::JoinHandle<()> {
        tokio::task::spawn_blocking(move || {
            let retry_delay = Duration::from_secs(5);

            loop {
                let hidapi = match HidApi::new() {
                    Ok(a) => a,
                    Err(e) => {
                        error!("Failed to init HIDAPI: {e} — retrying in {}s ...", retry_delay.as_secs());
                        std::thread::sleep(retry_delay);
                        continue;
                    }
                };

                let (vendor_id, product_id) = match Self::find_controller(&hidapi) {
                    Ok(ids) => ids,
                    Err(e) => {
                        debug!("PS4 controller not found: {e} — retrying in {}s ...", retry_delay.as_secs());
                        std::thread::sleep(retry_delay);
                        continue;
                    }
                };

                let device = match hidapi.open(vendor_id, product_id) {
                    Ok(d) => d,
                    Err(e) => {
                        warn!(
                            "Failed to open PS4 controller (VID={vendor_id:04X} PID={product_id:04X}): {e} — retrying in {}s ...",
                            retry_delay.as_secs()
                        );
                        std::thread::sleep(retry_delay);
                        continue;
                    }
                };

                info!(
                    "PS4 controller connected: {}",
                    device
                        .get_product_string()
                        .ok()
                        .flatten()
                        .unwrap_or_else(|| "Unknown".to_string())
                );

                let mut buf = [0u8; 64];
                let mut prev = empty_ps4_event();

                // Reader loop — break on error to fall through and retry
                // the whole discover→open→read cycle from the top.
                'reader: loop {
                    match device.read_timeout(&mut buf, -1) {
                        Ok(n) if n > 0 => {
                            let is_bluetooth = buf[0] == 0x11;
                            let current = parse_input_report(&buf, is_bluetooth);
                            let changes = find_changes(&prev, &current);
                            if check_change(&prev, &current) {
                                if let Err(e) = recipient.tell(changes).blocking_send() {
                                    warn!("Ps4Bridge: failed to deliver event: {e}");
                                }
                            }
                            prev = current;
                        }
                        Ok(_) => {}
                        Err(e) => {
                            warn!(
                                "HID read error: {e} — connection lost, retrying in {}s ...",
                                retry_delay.as_secs()
                            );
                            break 'reader;
                        }
                    }
                }

                drop(device);
                std::thread::sleep(retry_delay);
            }
        })
    }
}

impl Actor for Ps4Bridge {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, _actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("Ps4Bridge starting — HID discovery/read loop running in background");
        // No longer fails on_start if the controller isn't plugged in yet —
        // discovery, opening, and reconnection all happen inside the retry
        // loop now, same as the serial actor.
        Self::spawn_receive_loop(state.listener.clone());
        Ok(state)
    }
}

impl Message<Ps4Request> for Ps4Bridge {
    type Reply = ();

    async fn handle(
        &mut self,
        _msg: Ps4Request,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
    }
}
