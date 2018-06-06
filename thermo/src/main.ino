// Modbus + NeoPixel example
// Using libraries:
//    Yaacov ModbusSlave
//    Adafruit NeoPixel
// Written by Yaacov Zamir <kobi.zamir@gamil.com>
// Released under an MIT license.

#include <EEPROM.h>
#include <ModbusSlave.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN   6      // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE  DHT11  // DHT 11

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

//slave id = 1, control-pin = 8, baud = 9600/
#define SLAVE_ID 1
#define CTRL_PIN 10
#define BAUDRATE 9600

//  Modbus object declaration./
Modbus slave(SLAVE_ID, CTRL_PIN);

void setup() {
  // register handler functions.
  slave.cbVector[CB_READ_REGISTERS] = readMemory;
  slave.cbVector[CB_WRITE_MULTIPLE_REGISTERS] = writeMemory;

  // init dht sensor
  dht.begin();
  sensor_t sensor;
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  // set Serial and slave at baud 9600.
  Serial.begin( BAUDRATE );
  slave.begin( BAUDRATE );
}

void loop() {
  // listen for modbus commands con serial port.
  slave.poll();
}


// Handel Read Holding Registers (FC=03)
// write back the values from eeprom (holding registers)./
uint8_t readMemory(uint8_t fc, uint16_t address, uint16_t length) {
    uint16_t value;

    // read program memory.
    for (uint16_t i = 0; i < length; i++) {


      switch (i) {
        case 0:
          slave.writeRegisterToBuffer(i, get_temp());
          break;
        case 1:
          slave.writeRegisterToBuffer(i, get_humidity());
          break;
        default:
          EEPROM.get((address + i) * 2, value);

          // write uint16_t value to the response buffer.
          slave.writeRegisterToBuffer(i, value);
      }
    }

    return STATUS_OK;
}

// Handel Write Holding Registers (FC=16)
// write data into eeprom./
uint8_t writeMemory(uint8_t fc, uint16_t address, uint16_t length) {
    uint16_t value;
    uint16_t registerIndex;

    // write to eeprom.
    for (uint16_t i = 0; i < length; i++) {
        registerIndex = address + i;

        // get uint16_t value from the request buffer.
        value = slave.readRegisterFromBuffer(i);

        EEPROM.put(registerIndex * 2, value);
    }

    return STATUS_OK;
}

uint16_t get_temp() {
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    return (uint16_t)(999.99 * 100.0);
  } else {
    return (uint16_t)(event.temperature * 100.0);
  }
}

uint16_t get_humidity() {
  // Get humidity event and print its value.
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    return (uint16_t)(999.99 * 100.0);
  } else {
    return (uint16_t)(event.relative_humidity * 100.0);
  }
}
