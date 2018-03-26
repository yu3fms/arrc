// ARRC v0.1 (Amateur Radio Repeater Controller)
// Copyright (C) 2018 Filip M. Stanic, YT7FMS

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

// DESCRIPTION
// This program can be loaded into ATMEGA328 and the like Arduino boards.
// It is written to be used for radio signal repearters whose receive radio station
// does not have dedicated digital COS/COR signal.
// When an audio signal is present on Arduino's analog input it sets one of its
// digital I/O's to HIGH state.
// This signal can then be led to a transistor or optocoupler to put
// a repeaters TX station in transmit mode.

const int CSQ = A0; // Analog Input PIN to serve as COS/COR signal
const int PTT = 3;  // PTT pin
const int TS = 20;  // Treshold for audio level.
                    // You should experiment with this for Your partucular radio.

const unsigned long TAIL = 700;  // Tail length (milliseconds) !! 300ms will be added to this value

int TX = 0;

void setup () {
  pinMode(CSQ, INPUT);
  pinMode(PTT, OUTPUT);
}

void endtx () {
  if (BEEP == 1) {
    tone(4, 1480, 150);
    delay(200);
    noTone(4);
  }
  digitalWrite(PTT, LOW);
  TX = 0;
  delay(300); // Prevent TX triggering in case the receive station pick up RF from TX at the end of TX
}

void loop () {
  if (analogRead(CSQ) > TS) {
    if (TX == 0) {
      TX = 1;
      digitalWrite(PTT, HIGH);
    }
  }
  else {
    if (TX == 1) {
      for (int i = 0; i <= TAIL; i++) {
        if (analogRead(CSQ) > TS) {
          break;
        }
        if (i >= TAIL) {
          endtx();
        }
      }
    }
  }
}
