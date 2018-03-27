# arrc (Amateur Radio Repeater Controller)

This program can be loaded into ATMEGA328 and the like Arduino boards.
It is written to be used for radio signal repearters whose receive radio station
does not have dedicated digital COS/COR signal.
When an audio signal is present on Arduino's analog input it sets one of its
digital I/O's to HIGH state.
This signal can then be led to a transistor or optocoupler to put
a repeaters TX station in transmit mode.

## Features

- Time Out Timer <br>
  Overall time for which the repeater is allowed to be in TX state.
  It resets on every clean TX stop.

- Cool Off Time <br>
  Time for which the repeater will be in idle state and will not TX after the
  Time Out Time value has been reached.

- Beep sound on TX end <br>
  Play 1480Hz tone for the duration of 150ms at the end of TX

## To Be Added

- Reset itself before approx 50 days so that millis() do not overflow to ensure
  the program works properly.
