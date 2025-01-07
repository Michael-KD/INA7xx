#include <Wire.h>

class INA {
public:
    INA(uint8_t address) : address(address) {}

    void begin() {
        Wire.begin();
        Serial.begin(115200);
        Serial.println("I2C Communication Started");
    }

    String getBoardID() {
        char boardID[3] = {0};
        readRegister(MANFID_ADDRESS, boardID, 2);
        return String(boardID);
    }

    uint16_t readVoltage() {
        return readRegister16(VOLTAGE_REGISTER);
    }

    uint16_t readCurrent() {
        return readRegister16(CURRENT_REGISTER);
    }

    uint32_t readPower() {
        return readRegister24(POWER_REGISTER);
    }

    uint64_t readEnergy() {
        return readRegister40(ENERGY_REGISTER);
    }

    uint64_t readCharge() {
        return readRegister40(CHARGE_REGISTER);
    }

    uint16_t readTemp() {
        return readRegister16(TEMP_REGISTER);
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

INA ina(I2C_ADDRESS);

void setup() {
    ina.begin();
    Serial.print("Board ID: ");
    Serial.println(ina.getBoardID());
}

void loop() {
    uint16_t voltage = ina.readVoltage();
    Serial.print("Voltage: ");
    Serial.println(voltage * 0.003125);

    uint16_t current = ina.readCurrent();
    Serial.print("Current: ");
    Serial.println(current * 0.0024);

    uint32_t power = ina.readPower();
    Serial.print("Power: ");
    Serial.println(power * 0.000480);

    uint64_t energy = ina.readEnergy();
    Serial.print("Energy: ");
    Serial.println(energy * 0.00768);

    uint64_t charge = ina.readCharge();
    Serial.print("Charge: ");
    Serial.println(charge * 0.00015);

    uint16_t temp = ina.readTemp();
    Serial.print("Temp: ");
    Serial.println(temp * 0.003125);

    Serial.println("------------------------");

    delay(1000);
}