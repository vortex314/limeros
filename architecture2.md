# Lawnmower: Architecture

## 1. Overview

A DIY autonomous lawnmower built from off-the-shelf parts. An Android phone or tablet is the central **brain**. It runs the control software and provides the display, microphone, speaker and camera. ESP32-based **devices** handle the sensors and actuators and talk to the brain over a lightweight CBOR protocol.

### Goals
| Priority | Target | Description |
|---|---|---|
| Primary | **Mower** | Mows a defined area on its own. The area is taught in manual mode. |
| Secondary | **Sentry** | Guards the house at night or while you're away: patrols, deters intruders and records evidence. |

### Non-goals (proposed)
- Commercial-grade safety certification
- Cloud dependency: the system must work fully offline on the local network

---

## 2. System context

```
                 ┌──────────────────────────────────────────┐
                 │        Brain (Android phone/tablet)      │
                 │  Rust + kameo actors                     │
                 │  display · mic · speaker · camera        │
                 └───────────────┬──────────────────────────┘
                                 │ USB-serial / UDP (CBOR)
                        ┌────────┴────────┐
                        │  Gateway ESP32  │  (proposed, see 5.2)
                        └────────┬────────┘
                                 │ ESP-NOW (CBOR)
      ┌──────────────┬───────────┼──────────────┬───────────────┐
  IMU/compass    UWB tag      Drive ctrl     Cutter ctrl     Lights
  ESP32          ESP32+DWM1000 ESP32+hoverboard ESP32+ESC     ESP32
                     ▲
                     │ UWB ranging
             Fixed UWB anchors (garden)

  External: house cameras / NVR ──(person detected)──► Brain
```

---

## 3. Hardware

### 3.1 Sensors
| Sensor | Purpose | Notes |
|---|---|---|
| IMU (accel + gyro) | Heading rate, tilt and lift detection | Tilt/lift also feed the safety logic (§7) |
| Magnetometer | Absolute heading (compass) | ⚠ Hoverboard hub motors have strong magnets. Mount the sensor far from them and calibrate with the motors installed. |
| DWM1000 (UWB) | Localization by ranging against fixed **anchors** in the garden | At least 3 anchors for 2D. Plan anchor placement for line of sight. |
| Wheel odometry | Dead reckoning between UWB fixes | Hall sensors in the hoverboard motors (proposed) |
| Bumper / lift switch | Collision and lift detection | **(proposed)**. Strongly recommended. |
| Battery voltage/current | Return-to-charge, brown-out protection | **(proposed)** |
| Rain sensor | Don't mow wet grass | **(proposed, optional)** |
| Phone camera | Obstacle hints, sentry recording | Provided by the brain |

### 3.2 Actuators
| Actuator | Description |
|---|---|
| Drive | Hoverboard 2-wheel differential drive on a 3-wheel chassis with one passive caster. Controlled through hoverboard FOC firmware over UART, bridged by an ESP32. |
| Cutter | Single-blade motor, PWM driven, RPM measured. **Load feedback:** when current rises (thick grass), the drive speed drops. |
| Speaker (TTS) | Spoken failure and status messages, sentry warnings |
| Display | "Face" with expressions showing the current state and issues |
| Lights | Floodlight for night sentry mode, plus a status LED |

---

## 4. Software architecture (brain)

- **Language/runtime:** Rust, actor model on **kameo**
- **Android integration:** the Rust core is built as a native library (`cargo-ndk`) and loaded by one Kotlin host app. That app provides the phone services (camera, audio playback, TTS, mic) and talks to the core over UniFFI. See §4.3.

### 4.1 Actor hierarchy (proposed)
```
Supervisor
├── DeviceRegistry            – discovery, heartbeats, routing
│   └── <Device>Shadow ×N     – one per physical device
├── Localization              – fuses UWB + IMU + compass + odometry (EKF)
├── MapStore                  – cutting regions, no-go zones, home/dock
├── Planner                   – coverage path (e.g. boustrophedon)
├── MotionController          – path → wheel speeds, cutter-load slowdown
├── SafetyMonitor             – may veto any motion or cutter command
├── ModeManager               – state machine (§6)
├── PhoneShadow               – phone camera/mic/speaker as a device (§4.3)
├── Sentry                    – external triggers, patrol, recording
└── HMI                       – face/expressions, TTS, manual control
```

### 4.2 Shadow actors
Each physical device has a **shadow actor** in the brain that:
- holds the device's last reported state and the desired state
- turns typed Rust messages into CBOR commands and back
- tracks liveness through heartbeats and marks the device `Stale` or `Lost` on timeout
- retries commands that need an acknowledgement

Other actors only talk to shadows and never to the transport directly. That keeps the transport layer swappable and makes simulation and testing easy: a fake shadow can stand in for real hardware.

### 4.3 Phone services (Kotlin host app)

One Android app hosts the Rust brain **in the same process**. The phone's own hardware is treated as one more device: a **`PhoneShadow` actor** holds a handle to the Kotlin services, the same way the ESP32 shadows hold a transport.

```
┌───────────────────── Android app (one APK, one process) ─────────────────────┐
│  Kotlin                                    Rust (libbrain.so)                │
│  ┌────────────────────┐   PhoneServices    ┌──────────────────────────────┐  │
│  │ PhoneServicesImpl  │ ◄───────────────── │ PhoneShadow actor (kameo)    │  │
│  │  CameraX           │   (commands)       │  pending requests by id      │  │
│  │  SoundPool / TTS   │                    │                              │  │
│  │  AudioRecord (mic) │ ─────────────────► │ BrainHandle.on_photo/on_audio│  │
│  └────────────────────┘   (results/data)   └──────────────────────────────┘  │
│  Foreground service + face Activity (screen on)                              │
└──────────────────────────────────────────────────────────────────────────────┘
```

#### Interface

Communication goes both ways over UniFFI:

- **Rust → Kotlin:** the `PhoneServices` trait. It is a callback interface implemented in Kotlin, and every method returns immediately.
- **Kotlin → Rust:** the `BrainHandle` object. Kotlin uses it to deliver results and data streams.

```rust
#[uniffi::export(with_foreign)]
pub trait PhoneServices: Send + Sync {
    fn take_photo(&self, req_id: u64);
    fn start_video(&self, req_id: u64, path: String);
    fn stop_video(&self, req_id: u64);
    fn play_wav(&self, path: String, interrupt: bool);
    fn speak(&self, text: String, interrupt: bool);
    fn start_mic(&self, sample_rate: u32);
    fn stop_mic(&self);
}

#[derive(uniffi::Object)]
pub struct BrainHandle { /* tokio runtime + ActorRef<PhoneShadow> */ }

#[uniffi::export]
impl BrainHandle {
    #[uniffi::constructor]
    pub fn start(services: Arc<dyn PhoneServices>, data_dir: String) -> Arc<Self>;
    pub fn on_photo(&self, req_id: u64, jpeg: Vec<u8>);
    pub fn on_video_saved(&self, req_id: u64, path: String);
    pub fn on_error(&self, req_id: u64, msg: String);
    pub fn on_audio(&self, pcm_le16: Vec<u8>);   // 20 ms chunks, 16-bit mono
}
```

#### Kotlin implementations
| Service | Android API | Notes |
|---|---|---|
| Photo | CameraX `ImageCapture` | Returns JPEG bytes through `on_photo` |
| Video | CameraX `VideoCapture` + `Recorder` | Writes to the path Rust chose, then reports through `on_video_saved` |
| Play sound | `SoundPool` (short clips), `MediaPlayer` (long) | Preload frequently used clips at startup |
| TTS | `TextToSpeech` | `interrupt` → `QUEUE_FLUSH` for urgent messages |
| Mic | `AudioRecord`, 16 kHz mono PCM16 | Dedicated thread, streams chunks to `on_audio` |

#### Request/response pattern
- `PhoneShadow` assigns a `req_id` to each request and stores a `oneshot::Sender` in a pending map.
- The calling actor awaits the `oneshot::Receiver` outside the shadow's handler, so `PhoneShadow` never blocks.
- Every request has a timeout. An unanswered request fails with an error, the same way ESP32 shadows detect a lost device.

#### Rules for the Kotlin/Rust bridge
| Rule | Why |
|---|---|
| Kotlin methods return immediately and report results later through `BrainHandle` | UniFFI runs the Kotlin method on the calling Rust thread. A method that blocks would stall the tokio runtime threads. |
| Bind and unbind CameraX on the main thread | That's a CameraX requirement |
| Send audio as `Vec<u8>` (`ByteArray`), never `Vec<i16>` | `Vec<i16>` crosses as `List<Short>`, which boxes every sample and is very slow |
| Pass file paths for video and large files, bytes only for photos | Video files are too big to copy across the bridge |
| Rust chooses file paths, under `data_dir` | The Rust side keeps track of where the footage is stored |

#### Running unattended (Sentry at night)
Android blocks camera and mic access from the background, so the app must stay in the foreground:
1. Keep the face Activity in the foreground with `FLAG_KEEP_SCREEN_ON` and use **lock-task (kiosk) mode**.
2. Run a **foreground service** with `foregroundServiceType="camera|microphone|mediaPlayback"`, started while the app is visible.
3. Exclude the app from battery optimization (Doze).

**Permissions:** `CAMERA`, `RECORD_AUDIO`, `FOREGROUND_SERVICE`, `FOREGROUND_SERVICE_CAMERA`, `FOREGROUND_SERVICE_MICROPHONE`, `FOREGROUND_SERVICE_MEDIA_PLAYBACK`, `POST_NOTIFICATIONS`, `WAKE_LOCK`.

#### Development mode (future)
The transport sits behind `PhoneShadow`, so a second transport can be added later. In that setup the Kotlin app speaks the CBOR protocol (§5) over UDP, and the brain runs on a **Linux laptop** with the phone as a remote camera, mic and speaker over Wi-Fi. No other actor changes.

---

## 5. Communication

### 5.1 Message format (CBOR), proposed envelope
```
{
  v:    u8,      // protocol version
  dev:  u16,     // device id
  seq:  u32,     // sequence number (dedupe, ack matching)
  ts:   u32,     // device ms since boot
  kind: u8,      // telemetry | command | ack | heartbeat | event
  body: map      // type-specific payload
}
```
- Keep each message ≤ 250 bytes, the ESP-NOW payload limit.
- Define message schemas in a shared crate used by both the brain and the firmware (`esp-hal`/`esp-idf` Rust, or generated for C).

### 5.2 Transport
| Link | Use |
|---|---|
| ESP-NOW | Device ↔ gateway. Low latency, no AP needed. |
| UDP over Wi-Fi | Devices on the home network (e.g. stationary units, cameras) |
| USB-serial or UDP | Gateway ↔ brain |

⚠ **Android cannot speak ESP-NOW directly.** You need a **gateway ESP32** that bridges ESP-NOW to the phone over USB-OTG serial or UDP.

### 5.3 Timing (proposed)
| Stream | Rate |
|---|---|
| IMU | 50–100 Hz |
| UWB ranges | 5–10 Hz |
| Drive commands | 20–50 Hz |
| Heartbeats | 5 Hz, timeout 500 ms |

---

## 6. Operating modes

```
Idle ──► Manual ──► (teach region) ──► Idle
  │
  ├──► Mowing ──► Returning ──► Docked/Charging
  │       └──► Paused ──► Fault
  └──► Sentry (blade always OFF)
```

| Mode | Description |
|---|---|
| **Manual** | Driven by phone/remote. Used to **drive the boundary and record the cutting region** and no-go zones. |
| **Mowing** | Autonomous coverage of the stored regions |
| **Sentry** | Patrol or wait. Reacts to external person-detection events. |
| **Fault** | Safe stop. Shown on the face and announced by TTS. |

---

## 7. Safety (proposed, the most important missing section)

- **Fail-safe devices:** the drive and cutter ESP32s stop on their own if they get no valid command or heartbeat within N ms. Don't rely on the brain alone.
- **Blade stop** within 2 s on: lift, tilt > threshold, bumper hit, comms loss, manual e-stop.
- **Physical e-stop** button that cuts motor power in hardware.
- **Geofence:** motion outside the mapped region is refused.
- **Sentry:** blade is hard-disabled and speed is limited near people.
- `SafetyMonitor` has veto power over `MotionController`.

---

## 8. Sentry mode

1. External cameras (NVR, Frigate, Home Assistant, etc.) send a *person detected* event with a location.
2. The robot drives toward the location, keeping a safe distance, and turns on its lights.
3. TTS gives a spoken warning that the police will be called.
4. The phone camera records photos and video. Footage is uploaded or synced off-device immediately so it survives theft.
5. The owner gets a notification.

⚠ Check local privacy law on recording people and public areas. It's usually fine on your own property, but signage may be required.

---

## 9. HMI / expressions
| State | Face | Voice |
|---|---|---|
| Mowing | happy/focused | — |
| Blade overloaded | strained | — |
| Stuck / lifted | distressed | "I'm stuck" |
| Device lost | confused | "Lost contact with <device>" |
| Low battery | tired | "Going home to charge" |
| Sentry alert | stern | warning message |

---

## 10. Open questions
- Docking/charging: automatic dock, or manual charging?
- UWB accuracy: is it good enough alone, or is RTK-GPS needed as a fallback?
- Phone power and heat: is the phone powered from the mower battery, and how is it kept cool in the sun?
- Weatherproofing of the phone mount
- Firmware language for the ESP32s: Rust (`esp-hal`) or C/Arduino?
- Simulation: run the brain against simulated shadows before field tests?
- Audio processing: wake word or voice commands from the mic stream, and in which actor?
- Footage off-device: where are sentry photos and video synced to (NAS, Home Assistant, cloud)?
