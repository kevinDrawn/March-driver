#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "Tli49012_registers.h"

class Tli49012 {
    public:
        Tli49012(SPIClass &spi, uint32_t cs, uint32_t sync) : spi_(spi), cs_(cs), sync_(sync) {};
    
        bool begin(){
            digitalWrite(cs_, HIGH);
            digitalWrite(sync_, LOW);
            pinMode(cs_, OUTPUT);
            pinMode(sync_, OUTPUT);
            spi_.begin();
            delay(2); 
            
            // WHO AM I
            const Result result = readRegister(TLI49012_REG_HW_FW_ID);
            return result.crcOk && result.data == TLI49012_EXPECTED_HW_FW_ID; 
        }
        
        struct Result {
            uint8_t status=0;
            uint16_t data=0;
            bool crcOk = false;
            float angleDeg = 0.0f;
        };

        Result readRegister(uint16_t address){
            Result result{};
            if (address > 0x79) return result;
            uint8_t tx[4] ={
                static_cast<uint8_t>(address << 1),
                0x00,
                0x00,
                0x00
            };
            tx[3] = crc8(tx, 3);
            uint8_t rx[4]{};
            transferFrame(tx, rx);
            result.status = rx[0];
            result.data = (static_cast<uint16_t>(rx[1]) << 8) | rx[2];
            const uint8_t payload[4] = {
                static_cast<uint8_t>(tx[0] >> 1),
                rx[0], rx[1], rx[2]
            };
            result.crcOk = crc8(payload, 4) == rx[3];
            return result;
        }

        Result readAngle() {
            
            Result result = readRegister(TLI49012_REG_ANGLE_PRED);
            if (result.crcOk){
                result.angleDeg = toDegress(result.data);
            }

            return result;
        }

    private:
        SPIClass &spi_;
        uint32_t cs_;
        uint32_t sync_;

        static uint8_t crc8(const uint8_t *data, size_t length){
            uint8_t crc=0xFF;
            for(size_t i=0; i<length; ++i){
                crc ^= data[i];
                for(uint8_t bit =0; bit < 8; ++bit){
                    if(crc & 0x80){
                        crc = static_cast<uint8_t>((crc << 1) ^ 0x1D);
                    } else {
                        crc = static_cast<uint8_t>((crc << 1));
                    }
                }
            }
            return static_cast<uint8_t>(crc ^0xFF);
        }

        void transferFrame(const uint8_t *tx, uint8_t *rx){
            spi_.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
            delayMicroseconds(1);
            digitalWrite(cs_, LOW);
            delayMicroseconds(1);

            spi_.transfer(tx, rx, 4);
            delayMicroseconds(1);
            digitalWrite(cs_, HIGH);
            spi_.endTransaction();
        }

        static float toDegress(uint16_t raw){
            return static_cast<float>(raw) * 360.0f / 65536.0f;
        }
};