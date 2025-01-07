#include <Wire.h>

class INA {
public:
    INA(uint8_t address) : address(address) {}

    // void begin() {
    //     Wire.begin();
    //     Serial.begin(115200);
    //     Serial.println("I2C Communication Started");
    // }

    String getBoardID() {
        char boardID[3] = {0};
        readRegister(MANFID_ADDRESS, boardID, 2);
        return String(boardID);
    }

    uint16_t readVoltage() {
        uint16_t voltage = readRegister16(VOLTAGE_REGISTER);
        return voltage * 0.003125;
    }

    uint16_t readCurrent() {
        uint16_t current = readRegister16(CURRENT_REGISTER);
        return current * 0.0024;
    }

    uint32_t readPower() {
        uint32_t power = readRegister24(POWER_REGISTER);
        return power * 0.000480;
    }

    uint64_t readEnergy() {
        uint64_t energy = readRegister40(ENERGY_REGISTER);
        return energy * 0.00768;
    }

    uint64_t readCharge() {
        uint64_t charge = readRegister40(CHARGE_REGISTER);
        return charge * 0.00015;
    }

    float readTemp() {
        uint16_t rawValue = readRegister16(TEMP_REGISTER);
        rawValue = rawValue >> 4;
        float temperature = rawValue * 0.125;
        return temperature;
    }

private:
    uint8_t address;
    const uint8_t MANFID_ADDRESS = 0x3E;
    const uint8_t VOLTAGE_REGISTER = 0x05;
    const uint8_t CURRENT_REGISTER = 0x07;
    const uint8_t POWER_REGISTER = 0x08;
    const uint8_t ENERGY_REGISTER = 0x09;
    const uint8_t CHARGE_REGISTER = 0x0A;
    const uint8_t TEMP_REGISTER = 0x06;

    void readRegister(uint8_t reg, char* buffer, uint8_t length) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(address, length);
        for (uint8_t i = 0; i < length; i++) {
            if (Wire.available()) {
                buffer[i] = Wire.read();
            }
        }
    }

    uint16_t readRegister16(uint8_t reg) {
        uint16_t value = 0;
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(address, (uint8_t)2);
        if (Wire.available() >= 2) {
            value = (Wire.read() << 8) | Wire.read();
        }
        return value;
    }

    uint32_t readRegister24(uint8_t reg) {
        uint32_t value = 0;
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(address, (uint8_t)3);
        if (Wire.available() >= 3) {
            value = (Wire.read() << 16) | (Wire.read() << 8) | Wire.read();
        }
        return value;
    }

    uint64_t readRegister40(uint8_t reg) {
        uint64_t value = 0;
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(address, (uint8_t)5);
        if (Wire.available() >= 5) {
            value = ((uint64_t)Wire.read() << 32) | ((uint64_t)Wire.read() << 24) | (Wire.read() << 16) | (Wire.read() << 8) | Wire.read();
        }
        return value;
    }
};