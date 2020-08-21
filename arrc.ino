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
// Added comment test line

const int CSQ = A0; // Analog Input PIN to serve as COS/COR signal
const int PTT = 3;  // PTT pin
const int TS = 20;  // Treshold for audio level.
                    // You should experiment with this for Your partucular radio.

const unsigned long TAIL = 700;  // Tail length (milliseconds) !! 300ms will be added to this value

// Play "beep" at the end of TX (true / false)
#define BEEP true

// Use timeout timer (true / false)
#define TOTACT true

const unsigned long TOT = 60000;   // TX timeout time (milliseconds)
const unsigned long COT = 5000;   // TX Station forced COOL OFF TIME after a timeout (milliseconds)
const int XLOOP = 300;  // Set delay time after TX so we do not fall in RX-TX loop (milliseconds)

int TX = 0;          // Are we in TX (do not edit)
int BRK = 0;         // BRK (do not edit)
unsigned long TXST = 0;       // TX start time in millis (do not edit)
unsigned long TAIL_START = 0; // Tail start count time (do not edit)
unsigned long MILS = 0;       // (do not edit)
int STOPTX = 0;     // (do not edit)

void setup () {
  pinMode(CSQ, INPUT);
  pinMode(PTT, OUTPUT);
}

void endtx () {
  #if BEEP == true
    tone(4, 1480, 150);
    delay(200);
    noTone(4);
  #endif
  digitalWrite(PTT, LOW);
  TX = 0;
  BRK = 0;
  delay(XLOOP); // Prevent TX triggering in case the receive station pick up RF from TX at the end of TX
}

void loop () {
  if (analogRead(CSQ) > TS) {
    BRK = 0;
    if (TX == 0) {
      #if TOTACT == true
        TXST = millis();
      #endif
      TX = 1;
      digitalWrite(PTT, HIGH);
    }
    #if TOTACT == true
      MILS = millis();
      if ((MILS - TXST) >= TOT) {
        endtx();
        TXST = millis();
        delay(COT);
      }
    #endif
  }
  else {
    if (TX == 1 && BRK == 0) {
      delay(300); // Prevent immidiate tail brake
      TAIL_START = millis();
      while ((millis() - TAIL) < TAIL_START) {
        if (analogRead(CSQ) > TS) {
          BRK = 1;
          break;
        }
      }
      if (TX == 1 && BRK == 0) {
        endtx();
      }
    }
  }
}
