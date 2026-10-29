#pragma once
#include <Arduino.h>
#include <SPI.h>

class Tli49012 {
public:
  // User manual rev. 1.00: sections 8.1 and 8.2.11.
  static constexpr uint8_t AngleAddress = 0x0c;
  static constexpr float toDegrees(uint16_t raw) {
    return raw * (360.0f / 65536.0f);
  }
  struct Result {
    uint16_t data;
    uint8_t status;
    bool crcOk;
    uint8_t rx[4];
  };
  Tli49012(SPIClass &spi, uint32_t cs) : spi_(spi), cs_(cs) {}
  void begin() {
    digitalWrite(cs_, HIGH);
    pinMode(cs_, OUTPUT);
    spi_.begin();
  }
  Result readAngle() { return readRegister(AngleAddress); }
  // clearMask: explicit write-one-to-clear device status request (not NVM).
  Result readRegister(uint8_t address, uint8_t clearMask = 0) {
    Result r{};
    if (address > 0x79) return r;
    uint8_t tx[] = {static_cast<uint8_t>(address << 1), 0, clearMask, 0};
    tx[3] = crc8(tx, 3);
    spi_.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
    delayMicroseconds(1); // CS high >= 500 ns
    digitalWrite(cs_, LOW);
    delayMicroseconds(1); // lead >= 100 ns
    for (size_t i = 0; i < 4; ++i) r.rx[i] = spi_.transfer(tx[i]);
    delayMicroseconds(1); // lag >= 100 ns
    digitalWrite(cs_, HIGH);
    spi_.endTransaction();
    const uint8_t payload[] = {static_cast<uint8_t>(tx[0] >> 1), r.rx[0], r.rx[1], r.rx[2]};
    r.crcOk = crc8(payload, 4) == r.rx[3];
    r.status = r.rx[0];
    r.data = (static_cast<uint16_t>(r.rx[1]) << 8) | r.rx[2];
    return r;
  }
private:
  static uint8_t crc8(const uint8_t *data, size_t count) {
    uint8_t crc = 0xff;
    for (size_t i = 0; i < count; ++i) {
      crc ^= data[i];
      for (uint8_t bit = 0; bit < 8; ++bit)
        crc = (crc & 0x80) ? static_cast<uint8_t>((crc << 1) ^ 0x1d)
                           : static_cast<uint8_t>(crc << 1);
    }
    return crc ^ 0xff;
  }
  SPIClass &spi_;
  uint32_t cs_;
};
