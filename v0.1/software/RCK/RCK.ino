#include "KeyAliases.h"

#define HID_CUSTOM_LAYOUT // set this flag to indicate that a custom layout is selected
#define LAYOUT_SPANISH    // set this flag after the above flag to indicate the custom input method is Spanish

#include <HID-Project.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#define rowPinLength 1      // Define the size of rows array
#define colPinLength 2      // Define the size of cols array
#define DELAYVAL 100        // Define standard delay
#define FNROW 1             // Define FN row
#define FNCOL 0             // Define FN col
#define LED_COUNT 2         // Define number of pixels


// Define the pins used for the rows and columns of switches
const int rowPin[rowPinLength] = {A5};
const int colPin[colPinLength] = {A4, A3};
// Define the pin used for RGBWs
const int LED_PIN = 3;
// Define variables to store brightness level and brightness increment
int brightness = 0;
int brightnessInc = 31;

// Define the keys and their corresponding functions
const KeyboardKeycode standard[rowPinLength][colPinLength] = {
  {STD_1, STD_2}
};

// Initialize Adafruit module
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  
  strip.begin();                            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                             // Turn OFF all pixels ASAP
  strip.setBrightness(50);                  // Set BRIGHTNESS to about 1/5 (max = 255)

  uint8_t green = EEPROM.read(0);
  uint8_t red = EEPROM.read(1);
  uint8_t blue = EEPROM.read(2);
  uint8_t white = EEPROM.read(3);           // read brightness value from the EEPROM
  uint32_t color = ((uint32_t)green << 24) | ((uint32_t)red << 16) | ((uint32_t)blue << 8) | (uint32_t)white;
  
  for (int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, color);          // Set the color of the all LEDs
    strip.show();                           // Update the LEDs with the new color
  }
  
  for (int i = 0; i < rowPinLength; i++) {
    pinMode(rowPin[i], OUTPUT);             // Configure rowsPins as input with pull-up resistor
    digitalWrite(rowPin[i], LOW);
  }
  for (int i = 0; i < colPinLength; i++) {
    pinMode(colPin[i], INPUT_PULLUP);       // Configure colsPins as output with pull-up resistor
  }
  BootKeyboard.begin();                     // Sends a clean report to the host. This is important on any Arduino type.
}

boolean isPulsedfn() {
  digitalWrite(rowPin[FNROW], HIGH);
  if (digitalRead(colPin[FNCOL]) == LOW) {
    digitalWrite(rowPin[FNROW], LOW);
    return true;
  }
  else {
    digitalWrite(rowPin[FNROW], LOW);
    return false;
  }
}

void fn(int key) {
  switch (key) {
    case 1:
      //updateBrightness();
      break;
    default:
      break;
  }
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

/*
uint8_t green = (color >> 24) & 0xFF;
uint8_t red = (color >> 16) & 0xFF;
uint8_t blue = (color >> 8) & 0xFF;
uint8_t white = color & 0xFF;

EEPROM.write(0, green);
EEPROM.write(1, red);
EEPROM.write(2, blue);
EEPROM.write(3, white);
*/
