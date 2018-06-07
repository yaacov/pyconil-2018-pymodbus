# pyconil-2018-pymodbus

A PyCon talk pymodbus, talk about the pros and cons of using serial communication in our modern publish-subscribe messaging age. 

In this talk we demo two input devices, a kitty-detector and a thermometer and one output device (led strip).
We show how to query data from this devices and how to control them.

PyCon pymodbus lecture ( [Slides](https://github.com/yaacov/pyconil-2018-pymodbus/blob/master/PyModbus%20-%20PyCon%20IL%202018.pdf) )

## [kitty-detector](https://github.com/yaacov/pyconil-2018-pymodbus/tree/master/kitty-detector)

![kitty-detector](/kitty-detector/kitty-detector.jpg?raw=true "kitty-detector")

An input device based on a Raspberry pi, running Python opencv. This device outputs one integer modbus register
with posible values of 0, 1 or 2. 0 indecate no face in device field of view, 1 indecate a face and 2 indecate a cat face.

http://opencv-python-tutroals.readthedocs.io/en/latest/

#### Holding registers:
```
1 - [Read only] cat detection state.
```

## [mood-lights](https://github.com/yaacov/pyconil-2018-pymodbus/tree/master/mood-lights)

![mood-lights](/mood-lights/arduino.jpg?raw=true "mood-lights")

An output device based on an arduino. This device emit pulsating colorful lights, pulse rate, color and elumination
are controlled view modbus registers.

https://github.com/adafruit/Adafruit_NeoPixel

https://github.com/yaacov/ArduinoModbusSlave

#### Holding registers:
```
1 - [W/R] Light pulse delay in millisecs.
2 - [W/R] Light pulse flow direction, 0 - from left to right, 1 - from right to left.
3 - [W/R] Light pulse rate, how many delay steps it will take for the pulse to reach max value.
4 - [W/R] Light Red color ratio 0..255.
5 - [W/R] Light Green color ratio 0..255.
6 - [W/R] Light Blue color ratio 0..255.
```
## [thermo](https://github.com/yaacov/pyconil-2018-pymodbus/tree/master/thermo)

![thermo](/thermo/arduino.jpg?raw=true "thermo")

An input device based on an arduino. This device mesure temperature and relative humidity.

https://github.com/adafruit/DHT-sensor-library

https://github.com/yaacov/ArduinoModbusSlave

#### Holding registers:
```
1 - [Read only] Temp in C * 100 (e.g. 17.5 C will be 1750.0 ).
1 - [Read only] Relative humidity in % * 100 (e.g. 82.3 % will be 8230.0 ).
```
