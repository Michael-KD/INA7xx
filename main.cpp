// https://download.mikroe.com/documents/datasheets/INA780A_datasheet.pdf
// datasheet for INA780x chip, register map starts on page 30

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDRESS 0x40
#define MANFID_ADDRESS 0x3E
#define VOLTAGE_REGISTER 0x05
#define CURRENT_REGISTER 0x07
#define POWER_REGISTER 0x08
#define ENERGY_REGISTER 0x09
#define CHARGE_REGISTER 0x0A
#define TEMP_REGISTER 0x06


void setup() {
  // Code here will run once:
  Serial.begin(115200); // Initialize serial communication, to view messages in the serial monitor tab
  Wire.begin(); // Initialize I2C communication
  Serial.println("I2C Communication Started");
}

void loop() {
  // Will run repeatedly:
  char boardID[3] = {0}; // Array to store the ASCII characters

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(MANFID_ADDRESS); // Set the register to read from
  Wire.endTransmission(false); // Send the address and register, but keep the connection active

  Wire.requestFrom(I2C_ADDRESS, 2); // Request 2 bytes (16 bits)

  if (Wire.available() >= 2) {
    boardID[0] = Wire.read(); // Read the first byte
    boardID[1] = Wire.read(); // Read the second byte
  }

  Serial.print("Board ID: ");
  Serial.println(boardID);

  // Read Voltage
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(VOLTAGE_REGISTER); // Set the register to read from
  Wire.endTransmission(false); // Send the address and register, but keep the connection active

  Wire.requestFrom(I2C_ADDRESS, 2); // Request 2 bytes (16 bits)

  int voltage = 0;
  if (Wire.available() >= 2) {
    voltage = Wire.read() << 8; // Read the first byte and shift it to the high byte
    voltage |= Wire.read(); // Read the second byte and combine with the first byte
  }

  Serial.print("Voltage: ");
  Serial.println(voltage * 0.003125);

  // Read Current
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(CURRENT_REGISTER); // Set the register to read from
  Wire.endTransmission(false); // Send the address and register, but keep the connection active

  Wire.requestFrom(I2C_ADDRESS, 2); // Request 2 bytes (16 bits)

  int current = 0;
  if (Wire.available() >= 2) {
    current = Wire.read() << 8; // Read the first byte and shift it to the high byte
    current |= Wire.read(); // Read the second byte and combine with the first byte
  }

  Serial.print("Current: ");
  Serial.println(current * 0.0024);

  // Read Power (24 bits)
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(POWER_REGISTER); // Set the register to read from
  Wire.endTransmission(false); // Send the address and register, but keep the connection active

  Wire.requestFrom(I2C_ADDRESS, 3); // Request 3 bytes (24 bits)

  long power = 0;
  if (Wire.available() >= 3) {
    power = Wire.read() << 16; // Read the first byte and shift it to the high byte
    power |= Wire.read() << 8; // Read the second byte and shift it to the middle byte
    power |= Wire.read(); // Read the third byte and combine with the first two bytes
  }

  Serial.print("Power: ");
  Serial.println(power * 0.000480);

  // Read Energy (40 bits)
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(ENERGY_REGISTER); // Set the register to read from
  Wire.endTransmission(false); // Send the address and register, but keep the connection active

  Wire.requestFrom(I2C_ADDRESS, 5); // Request 5 bytes (40 bits)

  long long energy = 0;
  if (Wire.available() >= 5) {
    energy = (long long)Wire.read() << 32; // Read the first byte and shift it to the high byte
    energy |= (long long)Wire.read() << 24; // Read the second byte and shift it to the next byte
    energy |= Wire.read() << 16; // Read the third byte and shift it to the middle byte
    energy |= Wire.read() << 8; // Read the fourth byte and shift it to the next byte
    energy |= Wire.read(); // Read the fifth byte and combine with the first four bytes
  }

  Serial.print("Energy: ");
  Serial.println(energy * 0.00768);

  // Read Charge (40 bits)
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(CHARGE_REGISTER); // Set the register to read from
  Wire.endTransmission(false); // Send the address and register, but keep the connection active

  Wire.requestFrom(I2C_ADDRESS, 5); // Request 5 bytes (40 bits)

  long long charge = 0;
  if (Wire.available() >= 5) {
    charge = (long long)Wire.read() << 32; // Read the first byte and shift it to the high byte
    charge |= (long long)Wire.read() << 24; // Read the second byte and shift it to the next byte
    charge |= Wire.read() << 16; // Read the third byte and shift it to the middle byte
    charge |= Wire.read() << 8; // Read the fourth byte and shift it to the next byte
    charge |= Wire.read(); // Read the fifth byte and combine with the first four bytes
  }

  Serial.print("Charge: ");
  Serial.println(charge * 0.00015);


  // Read Temp
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(TEMP_REGISTER); // Set the register to read from
  Wire.endTransmission(false); // Send the address and register, but keep the connection active

  Wire.requestFrom(I2C_ADDRESS, 2); // Request 2 bytes (16 bits)

  int voltage = 0;
  if (Wire.available() >= 2) {
    voltage = Wire.read() << 8; // Read the first byte and shift it to the high byte
    voltage |= Wire.read(); // Read the second byte and combine with the first byte
  }

  Serial.print("Temp: ");
  Serial.println(voltage * 0.003125);




  Serial.println("------------------------");


  delay(1000);
}