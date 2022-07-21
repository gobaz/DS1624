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

#include "DS1624.h"
#include <Arduino.h>

DS1624::DS1624()
{   
  // Default true
  _temperatureValueValid = true;
  
  // a2 <- ground; a1 <- ground; a0 <- ground
  DS1624(0x00);
}

DS1624::DS1624(uint8_t addressByPins)
{  
  // Default true
  _temperatureValueValid = true;
  
  // Base address least significant bits will be a2, a1, a0 respectively 
  _address = 0x48 | (addressByPins & 0x07);
}

void DS1624::begin()
{
  // Start I2C communication on default SCK, SDA ports for I2C master
  Wire.begin();
  Wire.setWireTimeout(3000, true); // 3ms

  // Configure sensor
  Wire.beginTransmission(_address);
  // Command "Access config"
  Wire.write(0xAC);
  // lsb of configuration register is ONESHOT bit. Set it to zero to enable continuous conversion
  Wire.write(0x00);
  Wire.endTransmission();
    
  // Minimum time needed to store the configuration is 10ms
  // So wait for 100ms
  delay(100);
  
  // Start conversion
  Wire.beginTransmission(_address);
  Wire.write(0xEE);
  Wire.endTransmission();
}

void DS1624::getTemperature(float & readValue, bool & isValid)
{  
  readValue = ReadConvertedValue();
  isValid = _temperatureValueValid;
}

float DS1624::ReadConvertedValue()
{
  uint8_t msw = 0x00;
  uint8_t lsw = 0x00;
  _temperatureValueValid = true;
    
  // Request to read last converted temperature value
  Wire.beginTransmission(_address);
  Wire.write(0xAA);
  if (Wire.endTransmission(false) != 0) // Do not send STOP condition. But it is needed to start communication.
  {
    _temperatureValueValid = false;
    return 0.0;
  }
  Wire.requestFrom(_address, (uint8_t)2);
  
  // Read most significant word
  if(Wire.available())
  {
    msw = Wire.read();
  }
  else
  {
    _temperatureValueValid = false;
  }
  
  // Read least significant word
  if(Wire.available())
  {
    lsw = Wire.read();
  }
  else
  {
    _temperatureValueValid = false;
  }
  
  // Read possible other data
  while(Wire.available()) Wire.read();
  
  // If negative temperature, apply two's complement
  if(msw & 0x80)
  {
    msw = ~msw + 0x01;
    lsw = ~lsw + 0x01;
  }
  
  // Decimal part of the temperature is stored on 4 most significant bits
  // of the lsw value, so shift right 4 bits
  lsw >>= 4;

  // Convert to float and handle negative numbers
  float temperature = (float)msw;
  temperature += ((float)lsw) / 16.0; 
  return temperature;
}

int DS1624::readEEPROM(uint8_t startAddr, int count, uint8_t *data)
{
    int remaining = count;
    uint8_t address = startAddr;
    uint8_t index = 0;

    // Arduino I2C input buffer is limited to 32 bytes. Read in small chunks.
    while (remaining > 0)
    {
        uint8_t step = 16;
        if (remaining / 16 == 0)
            step = remaining % 16;

        Wire.beginTransmission(_address);
        Wire.write(0x17);
        Wire.write(address);
        if (Wire.endTransmission(false) != 0) // Do not send STOP signal
            return 0;

        uint8_t bytesRead = Wire.requestFrom((int)_address, (int)step, true);
        if (bytesRead > 0)
        {
            address += bytesRead;
            remaining -= bytesRead;
            while (Wire.available())
                data[index++] = Wire.read();
        }
    }

    return count;
}

int DS1624::writeEEPROM(uint8_t startAddr, int count, uint8_t *data)
{
    int remaining = count;
    uint8_t index = 0;

    // Page write is limited to 8 bytes. Write in chunks.
    while (remaining > 0)
    {
        uint8_t step = 8;
        if (remaining / 8 == 0)
            step = remaining % 8;

        Wire.beginTransmission(_address);
        Wire.write(0x17);
        Wire.write(startAddr + index);

        for (uint8_t i = 0; i < step; ++i)
            Wire.write(data[index + i]);
        int result;
        if ((result = Wire.endTransmission()) != 0)
            return count - remaining;
        // Needed delay after each write to EEPROM
        delay(50);

        remaining -= step;
        index += step;
    }

    return count;
}
