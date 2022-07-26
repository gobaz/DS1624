# DS1624 Library
Based on the previous work of [@bluemurder](https://github.com/bluemurder) at https://github.com/bluemurder/DS1624.git.

Arduino library for Maxim DS1624 temperature sensor and memory.

The DS1624 consists of two separate functional units: a 256-byte nonvolatile memory and a direct-to-digital temperature sensor.

The nonvolatile memory is made up of 256 bytes, accessed through the 2-wire serial bus.

The direct-to-digital temperature sensor allows the DS1624 to measure the ambient temperature and report the temperature in a 12-bit word with 0.0625°C resolution. The temperature sensor and its related registers are accessed through the 2-wire serial interface.

https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS1624.html

## Library features
* Temperature reading in continuous mode.
* EEPROM bulk read/write.
