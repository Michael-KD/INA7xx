#include "INA7xx.h"

INA::INA(uint8_t address, uint16_t chipNumber, uint16_t dividerScaling) {
    if (chipNumber != 780 && chipNumber != 740 && chipNumber != 745) {
        Serial.println("Error: Invalid chip number. Allowed values are 780, 740, or 745. Setting to default 780.");
        chipNumber = 780;
    }
    this->address = address;
    this->dividerScaling = dividerScaling;
    this->chipNumber = chipNumber;
    switch (chipNumber) {
        case 780:
            chipScale = 1;
            break;
        case 740:
        case 745:
            chipScale = 0.5;
            break;
    }
}

String INA::getBoardID() {
    char boardID[3] = {0};
    readRegister(MANFID_ADDRESS, boardID, 2);
    return String(boardID);
}

double INA::readVoltage() { // 2s complement, always positive
    uint16_t voltage = readRegister16(VOLTAGE_REGISTER);
    return voltage * 0.003125 * dividerScaling; // 3.125 mV/LSB
}

double INA::readTemp() {
    int16_t rawValue = static_cast<int16_t>(readRegister16(TEMP_REGISTER));
    rawValue >>= 4; // 12-bit resolution

    return rawValue * 0.125 ; // 125 m°C/LSB
}

double INA::readCurrent() { // 2s complement
    int16_t current = static_cast<int16_t>(readRegister16(CURRENT_REGISTER));
    return current * 0.0024 * dividerScaling * chipScale; // 2.4 mA/LSB
}

double INA::readPower() { // unsigned, always positive
    uint32_t power = readRegister24(POWER_REGISTER);
    return power * 0.000480 * dividerScaling * chipScale; // 480 µW/LSB
}

double INA::readEnergy() { // unsigned, always positive
    uint64_t energy = readRegister40(ENERGY_REGISTER);
    return energy * 0.00768 * dividerScaling * chipScale; // 7.68 mJ/LSB
}

double INA::readCharge() { // 2s complement
    int64_t charge = static_cast<int64_t>(readRegister40(CHARGE_REGISTER));
    return charge * 0.00015 * dividerScaling * chipScale; // 150 µC/LSB
}

INAValues INA::readAll() {
    INAValues values;
    values.voltage = readVoltage();
    values.current = readCurrent();
    values.power = readPower();
    values.energy = readEnergy();
    values.charge = readCharge();
    values.temp = readTemp();
    return values;
}

Diagnostics INA::getDiagnostics() {
    uint16_t diag = readRegister16(DIAG_ALRT_REGISTER);
    Diagnostics diagnostics;
    diagnostics.alatch = diag & (1 << 15);
    diagnostics.cnvr = diag & (1 << 14);
    diagnostics.slowAlert = diag & (1 << 13);
    diagnostics.apol = diag & (1 << 12);
    diagnostics.energyOF = diag & (1 << 11);
    diagnostics.chargeOF = diag & (1 << 10);
    diagnostics.mathOF = diag & (1 << 9);
    diagnostics.tmpOL = diag & (1 << 7);
    diagnostics.currentOL = diag & (1 << 6);
    diagnostics.currentUL = diag & (1 << 5);
    diagnostics.busOL = diag & (1 << 4);
    diagnostics.busUL = diag & (1 << 3);
    diagnostics.pol = diag & (1 << 2);
    diagnostics.cnvrF = diag & (1 << 1);
    diagnostics.memStat = diag & (1 << 0);
    return diagnostics;
}


void INA::readRegister(uint8_t reg, char* buffer, uint8_t length) {
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

uint16_t INA::readRegister16(uint8_t reg) {
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

uint32_t INA::readRegister24(uint8_t reg) {
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

uint64_t INA::readRegister40(uint8_t reg) {
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