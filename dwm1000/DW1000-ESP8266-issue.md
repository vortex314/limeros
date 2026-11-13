# ESP8266: ISR IRAM placement, IRQ reset hang, and anchor discovery/collision fixes

## Summary

Running this library on ESP8266 (NodeMCU) exposes a few issues that make the examples hang or cause some anchors to never be discovered. This issue documents them with proposed patches.

Tested on: ESP8266 (nodemcuv2), Arduino ESP8266 core, PlatformIO, DWM1000 via hardware SPI.

## 1. Interrupt handler not placed in IRAM

`handleInterrupt()` is registered with `attachInterrupt()`, but on ESP8266 an ISR must reside in IRAM or the core aborts with `ISR not in IRAM!`.

In `DW1000.h`:

```cpp
// before
static void handleInterrupt();
// after
static void IRAM_ATTR handleInterrupt();
```

In `DW1000.cpp`:

```cpp
// before
void DW1000Class::handleInterrupt() {
// after
void IRAM_ATTR DW1000Class::handleInterrupt() {
```

## 2. IRQ line still asserted on reset

`begin()` attaches the interrupt before the chip is reset. If the IRQ line is still asserted from a previous run, the ISR fires immediately and its SPI reads from interrupt context hang the ESP8266. Hard-reset the chip before attaching the interrupt.

```cpp
_rst        = rst;
_irq        = irq;
_deviceMode = IDLE_MODE;

// Hard-reset the chip before attaching the interrupt so its IRQ line is de-asserted.
if(_rst != 0xff) {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, LOW);
    delay(2);
    pinMode(_rst, INPUT);
    delay(10);
}
// attach interrupt
```

## 3. Anchor blink replies collide

All anchors answer the tag discovery blink immediately, so their replies collide on air and some anchors are never discovered. Add a small random backoff before replying.

```cpp
if(addNetworkDevices(&myTag)) {
    if(_handleBlinkDevice != 0) {
        (*_handleBlinkDevice)(&myTag);
    }
    delay(random(0, 50));  // random backoff so anchors do not collide on the blink reply
    //we reply by the transmit ranging init message
    transmitRangingInit(&myTag);
```

## 4. Short address collisions

The random short address is seeded from `analogRead(0)`, which can collide across boards, and the fixed path uses the first two bytes of the EUI, which are identical for devices from the same batch. Use the last two bytes instead, giving each device a deterministic unique short address. Applies in both `startAsAnchor()` and `startAsTag()`.

```cpp
// before
_currentShortAddress[0] = _currentAddress[0];
_currentShortAddress[1] = _currentAddress[1];
// after
_currentShortAddress[0] = _currentAddress[6];
_currentShortAddress[1] = _currentAddress[7];
```

## Note on ESP32

The ESP32 path additionally needs the whole ISR call-chain marked `IRAM_ATTR`, custom SPI pins, removal of `SPI.usingInterrupt()`, and a slower SPI clock to avoid an interrupt watchdog panic. Happy to file that separately if useful.
