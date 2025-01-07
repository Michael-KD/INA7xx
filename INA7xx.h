#ifndef INA7XX_H
#define INA7XX_H

#include <Wire.h>
#include <Arduino.h>

class INA {
public:
    INA(uint8_t address);

    void begin();
    String getBoardID();
    uint16_t readVoltage();
    uint16_t readCurrent();
    uint32_t readPower();
    uint64_t readEnergy();
    uint64_t readCharge();
    uint16_t readTemp();

private:
    uint8_t address;
    const uint8_t MANFID_ADDRESS = 0x3E;
    const uint8_t VOLTAGE_REGISTER = 0x05;
    const uint8_t CURRENT_REGISTER = 0x07;
    const uint8_t POWER_REGISTER = 0x08;
    const uint8_t ENERGY_REGISTER = 0x09;
    const uint8_t CHARGE_REGISTER = 0x0A;
    const uint8_t TEMP_REGISTER = 0x06;

    void readRegister(uint8_t reg, char* buffer, uint8_t length);
    uint16_t readRegister16(uint8_t reg);
    uint32_t readRegister24(uint8_t reg);
    uint64_t readRegister40(uint8_t reg);
};

#endif // INA7XX_H