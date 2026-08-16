#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <vector>

#include <option.h>
#include "cbor_codec.h"
#include "cobs_codec.h"

#include "actor_framework.h"
#include "led_actor.h"

// ---------------------------------------------------------------------------
// Configuration
// ---------------------------------------------------------------------------

// UART1 baud rate.
#define UART1_BAUD 115200

// UART0 debug serial buffers.
#define UART0_TX_BUFFER_SIZE 512
#define UART0_RX_BUFFER_SIZE 512

// Length of the visible debug LED pulse around each UART0 println.
#define UART0_DEBUG_LED_PULSE_MS 120

// How often the firmware sends a CBOR status message.
#define STATUS_PERIOD_MS 1000

// Maximum CBOR payload size before CRC is appended.
#define MAX_CBOR_PAYLOAD 96

// CBOR payload + 2-byte CRC.
#define MAX_PACKET_LEN (MAX_CBOR_PAYLOAD + 2)

// COBS frame buffer. More than enough for the packet sizes used here.
#define MAX_COBS_FRAME 128

// USE_NATIVE_UART1 == 0:
//   Use Arduino/Energia Serial1. On the LM4F120 Energia core this is
//   normally UART1 on PB0/PB1.
//
// USE_NATIVE_UART1 == 1:
//   Explicitly enable UART1 and configure PB0/PB1 using TivaWare driverlib
//   bundled with the Energia core.
#ifndef USE_NATIVE_UART1
#define USE_NATIVE_UART1 0
#endif

#if USE_NATIVE_UART1
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <inc/hw_memmap.h>
#endif
#include <actor_framework.h>
#include "serial_actor.h"
#include "led_actor.h"

Log logger;


void setup()
{

  // UART0 debug output, usually routed to the onboard ICDI USB serial.
  Serial.setBufferSize(UART0_TX_BUFFER_SIZE, UART0_RX_BUFFER_SIZE);
  Serial.begin(115200);
  delay(250);

  Serial.println(F("LM4F120 CBOR/COBS/CRC UART1 node: boot"));

  ActorId redLedActorId = actor_system.registerActor(new LedActor(RED_LED));
  ActorId greenLedActorId = actor_system.registerActor(new LedActor(GREEN_LED));
  ActorId blueLedActorId = actor_system.registerActor(new LedActor(BLUE_LED));
  ActorId serialActorId = actor_system.registerActor(new SerialActor(Serial1,greenLedActorId));
  (void)redLedActorId;
  (void)greenLedActorId;
  (void)blueLedActorId;
  (void)serialActorId;
  actor_system.send(redLedActorId, redLedActorId, new LedBlink{500});
  actor_system.send(greenLedActorId, greenLedActorId, new LedBlink{1000});
  actor_system.send(blueLedActorId, blueLedActorId, new LedBlink{1500});
}

void loop()
{
  actor_system.run();
}

extern "C" void __cxa_pure_virtual()
{
  Serial.println(F("Pure virtual function called!"));
  while (1)
    ;
}

extern "C" void _exit()
{
  Serial.println(F("Exit called!"));
  while (1)
    ;
}


