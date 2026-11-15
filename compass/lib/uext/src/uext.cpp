#include "uext.h"

// Olimex UEXT pinout (matches akkaEsp32 Hardware_ESP32.cpp Connector mapping).
static const int UEXT1_PINS[Uext::PIN_COUNT] = { 19, 36, 25, 26, 34, 23, 17, 32 };
static const int UEXT2_PINS[Uext::PIN_COUNT] = { 18, 39, 27, 14, 35, 22, 16, 33 };

static const char* const PIN_NAMES[Uext::PIN_COUNT] = {
    "TXD", "RXD", "SCL", "SDA", "MISO", "MOSI", "SCK", "CS"
};

Uext::Uext(uint8_t index)
    : _index(index), _pinsUsed(0),
      _i2c(nullptr), _spi(nullptr), _uart(nullptr),
      _i2cBegun(false), _spiBegun(false), _uartBegun(false)
{
    const int* src = (index == 1) ? UEXT1_PINS : UEXT2_PINS;
    for (uint8_t i = 0; i < PIN_COUNT; i++) _pins[i] = src[i];
}

TwoWire& Uext::i2c(uint32_t freq)
{
    if (!_i2cBegun) {
        lock(SCL);
        lock(SDA);
        _i2c = (_index == 1) ? &Wire : &Wire1;
        _i2c->begin(sda(), scl(), freq);
        _i2cBegun = true;
        Serial.printf("UEXT%d: I2C on SDA=%d SCL=%d @%lu Hz\n",
                      _index, sda(), scl(), (unsigned long)freq);
    }
    return *_i2c;
}

SPIClass& Uext::spi()
{
    if (!_spiBegun) {
        lock(SCK);
        lock(MISO);
        lock(MOSI);
        lock(CS);
        if (_index == 1) {
            _spi = &SPI;                // VSPI (bus 3), the standard global
        } else {
            _spi = new SPIClass(HSPI);  // HSPI (bus 2)
        }
        _spi->begin(sck(), miso(), mosi(), cs());
        _spiBegun = true;
        Serial.printf("UEXT%d: SPI on SCK=%d MISO=%d MOSI=%d CS=%d\n",
                      _index, sck(), miso(), mosi(), cs());
    }
    return *_spi;
}

HardwareSerial& Uext::uart(uint32_t baud)
{
    if (!_uartBegun) {
        lock(TXD);
        lock(RXD);
        _uart = (_index == 1) ? &Serial1 : &Serial2;
        _uart->begin(baud, SERIAL_8N1, rx(), tx());
        _uartBegun = true;
        Serial.printf("UEXT%d: UART on TXD=%d RXD=%d @%lu baud\n",
                      _index, tx(), rx(), (unsigned long)baud);
    }
    return *_uart;
}

void Uext::pinMode(LogicalPin lp, uint8_t mode)
{
    ::pinMode((uint8_t)pin(lp), mode);
}

void Uext::digitalWrite(LogicalPin lp, uint8_t val)
{
    ::digitalWrite((uint8_t)pin(lp), val);
}

int Uext::digitalRead(LogicalPin lp)
{
    return ::digitalRead((uint8_t)pin(lp));
}

void Uext::lock(LogicalPin lp)
{
    uint32_t mask = (1u << lp);
    if (_pinsUsed & mask) {
        Serial.printf("UEXT%d: WARNING pin %s already in use\n", _index, PIN_NAMES[lp]);
    }
    _pinsUsed |= mask;
}

void Uext::dump()
{
    Serial.printf("UEXT%d: ", _index);
    for (uint8_t i = 0; i < PIN_COUNT; i++) {
        Serial.printf("%s=%d ", PIN_NAMES[i], _pins[i]);
    }
    Serial.println();
}
