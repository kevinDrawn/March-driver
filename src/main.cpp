#include <Arduino.h>
#include "board_config.h"
#include "Tli49012.h"

SPIClass encoderSpi(ENC_MOSI, ENC_MISO, ENC_CLK);
Tli49012 encoder(encoderSpi, ENC_CS, ENC_SYNC);

void setup() {
  Serial.begin(115200);
  delay(1000);

  const bool initialized = encoder.begin();
  Serial.println(initialized
      ? "TLI49012 initialization OK"
      : "TLI49012 initialization FAIL");
}

void loop() {
  const auto result = encoder.readAngle();
  if (result.crcOk){
      Serial.print("raw=0x");
      Serial.print(result.data, HEX); // HEX - 16진수
      Serial.print(" angle_deg=");
      Serial.print(result.angleDeg, 3);
      Serial.print(" status=0x");
      Serial.println(result.status, HEX);
  } else{
      Serial.println("Angle CRC FAIL");
  }
  delay(100);
}
