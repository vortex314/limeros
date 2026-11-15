#pragma once
// UEXT connector abstraction for ESP32 (Arduino).
//
// Mirrors the akkaEsp32 `Connector` pinout for UEXT1/UEXT2 and exposes the
// I2C / SPI / UART buses plus GPIO accessors for each connector. Buses are
// configured lazily on first use, so constructing a Uext does not touch
// hardware and is safe at static-init time.
//
// UEXT1: TXD=19 RXD=36 SCL=25 SDA=26 MISO=34 MOSI=23 SCK=17 CS=32  (I2C -> Wire)
// UEXT2: TXD=18 RXD=39 SCL=27 SDA=14 MISO=35 MOSI=22 SCK=16 CS=33  (I2C -> Wire1)

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

class Uext {
public:
    enum LogicalPin : uint8_t {
        TXD = 0,
        RXD,
        SCL,
        SDA,
        MISO,
        MOSI,
        SCK,
        CS,
        PIN_COUNT
    };

    // index = 1 -> UEXT1, index = 2 -> UEXT2
    explicit Uext(uint8_t index);

    uint8_t index() const { return _index; }

    int pin(LogicalPin lp) const { return _pins[lp]; }
    int tx()   const { return _pins[TXD]; }
    int rx()   const { return _pins[RXD]; }
    int scl()  const { return _pins[SCL]; }
    int sda()  const { return _pins[SDA]; }
    int miso() const { return _pins[MISO]; }
    int mosi() const { return _pins[MOSI]; }
    int sck()  const { return _pins[SCK]; }
    int cs()   const { return _pins[CS]; }

    // Bus accessors (configured lazily on first use).
    TwoWire&        i2c(uint32_t freq = 400000);
    SPIClass&       spi();
    HardwareSerial& uart(uint32_t baud = 115200);

    // GPIO helpers that operate on this connector's logical pins.
    void pinMode(LogicalPin lp, uint8_t mode);
    void digitalWrite(LogicalPin lp, uint8_t val);
    int  digitalRead(LogicalPin lp);

    void dump();

private:
    void lock(LogicalPin lp);

    uint8_t  _index;
    int      _pins[PIN_COUNT];
    uint32_t _pinsUsed;

    TwoWire*        _i2c;
    SPIClass*       _spi;
    HardwareSerial* _uart;
    bool _i2cBegun;
    bool _spiBegun;
    bool _uartBegun;
};
