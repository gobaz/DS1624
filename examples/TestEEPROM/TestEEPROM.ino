#include <DS1624.h>

// All address pins to VCC
DS1624 ds1624(7);
void setup() {
  Serial.begin(9600);
  ds1624.begin();

  randomSeed(analogRead(A0));

  static uint8_t dataOut[256] = { 0 };
  static uint8_t dataIn[256] = { 0 };

  // Fill the buffer with pseudo-random numbers
  for (int i = 0; i < 256; ++i)
    dataOut[i] = random(0, 256);

  // Write them to EEPROM
  if (ds1624.writeMemory(0, 256, dataOut) != 256) {
    Serial.println("EEPROM write error!");
    return;
  }

  // Read data back into another buffer
  if (ds1624.readMemory(0, 256, dataIn) != 256) {
    Serial.println("EEPROM read error!");
    return;
  }

  // Check if buffers match
  if (memcmp(dataOut, dataIn, 256) == 0) {
    Serial.println("EEPROM write/read succesfull");
  } else {
    Serial.println("Invalid data read from EEPROM!");
  }
}

void loop() {

}