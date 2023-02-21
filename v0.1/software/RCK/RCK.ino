#include "KeyAliases.h"
#include "Functions.h"

void setup() {
  strip.begin();                            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                             // Turn OFF all pixels ASAP
  strip.setBrightness(50);                  // Set BRIGHTNESS to about 1/5 (max = 255)

  uint32_t color = getEEPROM();
  updateEEPROM(color);
  
  for (int i = 0; i < rowPinLength; i++) {
    pinMode(rowPin[i], OUTPUT);             // Configure rowsPins as input with pull-up resistor
    digitalWrite(rowPin[i], LOW);
  }
  for (int i = 0; i < colPinLength; i++) {
    pinMode(colPin[i], INPUT_PULLUP);       // Configure colsPins as output with pull-up resistor
  }
  BootKeyboard.begin();                     // Sends a clean report to the host. This is important on any Arduino type.
}

void loop() {
  delay(DELAYVAL);
  for (int i = 0; i < rowPinLength; i++) {
    digitalWrite(rowPin[i], HIGH);
    for (int j = 0; j < colPinLength; j++) {
      if (digitalRead(colPin[j]) == LOW) {
        if ( i == 0 && j >= 1 && j <= 1 && isPulsedfn()) {
          delay(20);  // Debouncing delay
          fn(j);
        } else {
          Keyboard.press(standard[i][j]);
        }
      }
    }
    digitalWrite(rowPin[i], LOW);
  }
  Keyboard.releaseAll();
}
