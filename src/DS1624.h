/*****************************************************************************
Arduino Library for Maxim DS1624 temperature sensor and memory

MIT License

Copyright (c) 2017 Alessio Leoncini, https://www.technologytourist.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#ifndef DS1624_Arduino_Library
#define DS1624_Arduino_Library

#include <Wire.h>

class DS1624
{
  private:
  // Stores the actual address
  uint8_t _address;

  // Flag that stores if last temperature value was valid
  bool _temperatureValueValid;

  // Receive from sensor the last converted temperature value
  float ReadConvertedValue();

  public:
  // Default constructor. Use address pins a2,a1,a0 connected to ground.
  DS1624();

  // Class constructor, accepting a 3-bit code a2|a1|a0 corresponding to
  // physical address pin connections
  DS1624( uint8_t addressByPins );

  // First init of present class
  void begin();

  // Gets temperature in centigrade degrees
  void getTemperature( float& readValue, bool& isValid );
  bool getTemperature( float& readValue );
  float getTemperature( void );

  // Read count bytes from EEPROM into buffer "data"
  // Return number of bytes read
  int readMemory( uint8_t startAddr, int count, uint8_t* data );

  // Write count bytes from buffer "data" to EEPROM
  // Return number of bytes written
  int writeMemory( uint8_t startAddr, int count, uint8_t* data );
};

#endif //DS1624_Arduino_Library
