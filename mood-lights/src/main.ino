// Modbus + NeoPixel example
// Using libraries:
//    Yaacov ModbusSlave
//    Adafruit NeoPixel
// Written by Yaacov Zamir <kobi.zamir@gamil.com>
// Released under an MIT license.

#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <ModbusSlave.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//slave id = 1, control-pin = 8, baud = 9600/
#define SLAVE_ID 2
#define CTRL_PIN 10
#define BAUDRATE 9600

//  Modbus object declaration./
Modbus slave(SLAVE_ID, CTRL_PIN);

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define LED_PIN        6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      30

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// default registers
uint16_t delayval = 100; // delay for half a second
uint8_t direction = 0; // direction of pixel flow
uint8_t steps = 10; // direction of pixel flow
uint8_t r = 150; // red color register
uint8_t g = 60; // green color register
uint8_t b = 0; // blue color register

void setup() {
  // register handler functions.
  slave.cbVector[CB_READ_REGISTERS] = readMemory;
  slave.cbVector[CB_WRITE_MULTIPLE_REGISTERS] = writeMemory;

  // set NeoPixel strip
  pixels.begin(); // This initializes the NeoPixel library.

  // set Serial and slave at baud 9600.
  Serial.begin( BAUDRATE );
  slave.begin( BAUDRATE );
}

void loop() {
  uint8_t i;
  uint8_t c;
  uint8_t pos;

  i = (millis() / delayval) % NUMPIXELS;
  if (direction == 0) {
    pos = i;
  } else {
    pos = NUMPIXELS - i - 1;
  }

  c = (millis() / (steps * delayval)) % steps;

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  pixels.setPixelColor(pos, pixels.Color(
    (r * c) / steps,
    (g * c) / steps,
    (b * c) / steps)
  );
  pixels.show(); // This sends the updated pixel color to the hardware.

  // listen for modbus commands con serial port.
  slave.poll();
}


// Handel Read Holding Registers (FC=03)
// write back the values from eeprom (holding registers)./
uint8_t readMemory(uint8_t fc, uint16_t address, uint16_t length) {
    uint16_t value;

    // read program memory.
    for (uint16_t i = 0; i < length; i++) {
        EEPROM.get((address + i) * 2, value);

        // write uint16_t value to the response buffer.
        slave.writeRegisterToBuffer(i, value);
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

        switch (registerIndex) {
          case 1:
            delayval = value;
            break;
          case 2:
            direction = value;
            break;
          case 3:
            steps = value;
            break;
          case 4:
            r = value;
            break;
          case 5:
            g = value;
            break;
          case 6:
            b = value;
            break;
        }
        EEPROM.put(registerIndex * 2, value);
    }

    return STATUS_OK;
}
