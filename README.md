# arrc
Amateur Radio Repeater Controller

This program can be loaded into ATMEGA328 and the like Arduino boards.
It is written to be used for radio signal repearters whose receive radio station
does not have dedicated digital COS/COR signal.
When an audio signal is present on Arduino's analog input it sets one of its
digital I/O's to HIGH state.
This signal can then be led to a transistor or optocoupler to put
a repeaters TX station in transmit mode.
