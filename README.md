# pyconil-2018-pymodbus

PyCon pymodbus lecture ( [Slides PDF](https://github.com/yaacov/pyconil-2018-pymodbus/blob/master/PyModbus%20-%20PyCon%20IL%202018.pdf) )

## [kitty-detector](https://github.com/yaacov/pyconil-2018-pymodbus/tree/master/kitty-detector)

An input device based on a Raspberry pi, running Python opencv. This device outputs one integer modbus register
with posible values of 0, 1 or 2. 0 indecate no face in device field of view, 1 indecate a face and 2 indecate a cat face.

## [mood-lights](https://github.com/yaacov/pyconil-2018-pymodbus/tree/master/mood-lights)

An output device based on an arduino. This device emit pulsating colorful lights, pulse rate, color and elumination
are controlled view modbus registers.

## [thermo](https://github.com/yaacov/pyconil-2018-pymodbus/tree/master/thermo)

An input device based on an arduino. This device mesure temperature and relative humidity.
