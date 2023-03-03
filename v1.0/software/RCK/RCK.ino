#include "KeyAliases.h"
#include "Functions.h"

void setup() {
  strip.begin();                            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                             // Turn OFF all pixels ASAP
  strip.setBrightness(255);                 // Set BRIGHTNESS to about 1/5 (max = 255)

  EEPROM.begin();
  uint32_t color = getColorEEPROM();

  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color);          // Set the color of the all LEDs
    strip.show();                           // Update the LEDs with the new color
  }

  for (int i = 0; i < rowPinLength; i++) {
    pinMode(rowPin[i], OUTPUT);             // Configure rowsPins as input with pull-up resistor
    digitalWrite(rowPin[i], LOW);
  }
  for (int i = 0; i < muxPinLength; i++) {
    pinMode(muxPin[i], OUTPUT);             // Configure rowsPins as input with pull-up resistor
  }
  pinMode(colPin, INPUT_PULLUP);            // Configure colsPins as output with pull-up resistor
  BootKeyboard.begin();                     // Sends a clean report to the host. This is important on any Arduino type.
}

void loop() {
  delayMicroseconds(DELAYVAL);
  for (byte i = 0; i < rowPinLength; i++) {
    digitalWrite(rowPin[i], LOW);
    for (byte j = 0; j < colPinLength; j++) {
      for (byte k = 0; k < muxPinLength - 1; k++) {
        digitalWrite(muxPin[k], bitRead(j, k));
      }
      digitalWrite(muxPin[4], 0);
      delayMicroseconds(200);  // Debouncing delay
      if (digitalRead(colPin) == LOW) {
        if (state[i][j] == false) {
          state[i][j] = true;
          if ( i == 0 && j > 0 && j < 13  && state[FNROW][FNCOL]) {
            delayMicroseconds(200);  // Debouncing delay
            fn(j);
          } else {
            Keyboard.press(standard[i][j]);
            delayMicroseconds(DELAYVAL);
          }
        }
      } else {
        if (state[i][j] == true) {
          Keyboard.release(standard[i][j]);
          state[i][j] = false;
        }
      }
    }
    digitalWrite(rowPin[i], HIGH);
  }
}
