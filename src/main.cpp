#include <Arduino.h>
#include <SPI.h>
#include <stdlib.h>
#include "board_config.h"
#include "Tli49012.h"

// STM32duino argument order: MOSI, MISO, SCK; software CS.
SPIClass encoderSpi(ENC_MOSI, ENC_MISO, ENC_CLK);
Tli49012 encoder(encoderSpi, ENC_CS);
int selectedAddress = Tli49012::AngleAddress;
char command[24];
size_t commandLength = 0;
bool overflow = false;
uint32_t crcErrors = 0;
uint32_t recoveredReads = 0;
uint32_t failedReads = 0;

void help() {
  Serial.println("TLI49012 SPI1 Mode1 1MHz; sample 100Hz, log 10Hz; SYNC LOW");
  Serial.println("a: angle | r HEX: raw register | d: diagnostic registers | c: clear latched status | s: stop | h: help");
}

Tli49012::Result checkedRead(uint8_t address) {
  auto r = encoder.readRegister(address);
  if (!r.crcOk) {
    ++crcErrors;
    r = encoder.readRegister(address); // Manual 7.1.1: discard and repeat.
    if (r.crcOk) ++recoveredReads;
    else { ++crcErrors; ++failedReads; }
  }
  return r;
}

void handleCommand() {
  command[commandLength] = '\0';
  if (overflow) Serial.println("Command too long.");
  else if (commandLength == 1) {
    switch (command[0]) {
      case 'a': selectedAddress = Tli49012::AngleAddress; break;
      case 's': selectedAddress = -1; Serial.println("Stopped."); break;
      case 'h': help(); break;
      case 'c': {
        // OFC, DIAG, MAG, SUP, RST, OT. Preserve evidence until requested.
        const auto r = encoder.readRegister(Tli49012::AngleAddress, 0xbe);
        Serial.println(r.crcOk ? "Clear requested; next read shows resulting status."
                              : "Clear response CRC failed; clear outcome unknown.");
        break;
      }
      case 'd': {
        const uint8_t addresses[] = {0x00, 0x01, 0x03, 0x05, 0x15};
        for (uint8_t address : addresses) {
          const auto r = checkedRead(address);
          char line[80];
          snprintf(line, sizeof(line), "diag addr=%02X data=%04X status=%02X crc=%s",
                   address, r.data, r.status, r.crcOk ? "OK" : "FAIL/DISCARD");
          Serial.println(line);
        }
        break;
      }
      default: help(); break;
    }
  } else if (commandLength >= 3 && command[0] == 'r' && command[1] == ' ') {
    char *end = nullptr;
    const long a = strtol(command + 2, &end, 16);
    if (end != command + 2 && *end == '\0' && a >= 0 && a <= 0x79)
      selectedAddress = static_cast<int>(a);
    else Serial.println("Invalid in-frame address (00..79).");
  } else if (commandLength) help();
  commandLength = 0;
  overflow = false;
}

void setup() {
  Serial.begin(115200);
  digitalWrite(ENC_SYNC, LOW);
  pinMode(ENC_SYNC, OUTPUT);
  encoder.begin();
  delay(2); // Datasheet: first measurement within 1.7 ms after power/reset.
}

void loop() {
  static bool announced = false;
  if (!announced && millis() >= 1500) { help(); announced = true; }
  while (Serial.available()) {
    const char c = static_cast<char>(Serial.read());
    if (c == '\n' || c == '\r') handleCommand();
    else if (!overflow && commandLength < sizeof(command) - 1)
      command[commandLength++] = c;
    else overflow = true;
  }
  static uint32_t lastRead = 0;
  static uint32_t lastLog = 0;
  if (selectedAddress < 0 || millis() - lastRead < 10) return;
  lastRead = millis();
  const auto r = checkedRead(static_cast<uint8_t>(selectedAddress));
  if (millis() - lastLog < 100) return;
  lastLog = millis();
  char line[240];
  // Format angle without depending on embedded printf float support.
  char angle[20] = "INVALID";
  // LFOK describes the previous MOSI frame; OFC/RST can be historical latches.
  const bool healthy = r.crcOk && (r.status & 0x40) && !(r.status & 0x3a);
  if (selectedAddress == Tli49012::AngleAddress && healthy)
    dtostrf(Tli49012::toDegrees(r.data), 1, 3, angle);
  else if (selectedAddress != Tli49012::AngleAddress) strcpy(angle, "RAW_MODE");
  snprintf(line, sizeof(line),
    "addr=%02X angle_deg=%s raw=%04X status=%02X crc=%s OFC=%u LFOK=%u DIAG=%u MAG=%u SUP=%u RST=%u OT=%u CAL=%u crc_errors=%lu recovered=%lu failed=%lu",
    selectedAddress, angle, r.data, r.status, r.crcOk ? "OK" : "FAIL/DISCARD",
    !!(r.status & 0x80), !!(r.status & 0x40), !!(r.status & 0x20), !!(r.status & 0x10),
    !!(r.status & 8), !!(r.status & 4), !!(r.status & 2), !!(r.status & 1),
    static_cast<unsigned long>(crcErrors), static_cast<unsigned long>(recoveredReads),
    static_cast<unsigned long>(failedReads));
  Serial.println(line);
}
