#ifndef INA7XX_H
#define INA7XX_H

#include <Wire.h>
#include <Arduino.h>

#define MANFID_ADDRESS 0x3E
#define VOLTAGE_REGISTER 0x05
#define CURRENT_REGISTER 0x07
#define POWER_REGISTER 0x08
#define ENERGY_REGISTER 0x09
#define CHARGE_REGISTER 0x0A
#define TEMP_REGISTER 0x06
#define DIAG_ALRT_REGISTER 0x0B

struct INAValues {
    double voltage;
    double current;
    double power;
    double energy;
    double charge;
    double temp;
};

struct Diagnostics {
    bool alatch;
    bool cnvr;
    bool slowAlert;
    bool apol;
    bool energyOF;
    bool chargeOF;
    bool mathOF;
    bool tmpOL;
    bool currentOL;
    bool currentUL;
    bool busOL;
    bool busUL;
    bool pol;
    bool cnvrF;
    bool memStat;
};


class INA {
public:
    INA(uint8_t address, uint16_t chipNumber, uint16_t dividerScaling = 1);

    // void begin();
    String getBoardID();
    double readVoltage();
    double readTemp();
    double readCurrent();
    double readPower();
    double readEnergy();
    double readCharge();
    INAValues readAll();
    Diagnostics getDiagnostics();



private:
    uint8_t address;
    uint16_t dividerScaling;
    uint16_t chipNumber;
    uint8_t chipScale;

    void readRegister(uint8_t reg, char* buffer, uint8_t length);
    uint16_t readRegister16(uint8_t reg);
    uint32_t readRegister24(uint8_t reg);
    uint64_t readRegister40(uint8_t reg);
};

#endif // INA7XX_H