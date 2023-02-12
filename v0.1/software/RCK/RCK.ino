#include "KeyAliases.h"
#define HID_CUSTOM_LAYOUT // set this flag to indicate that a custom layout is selected
#define LAYOUT_SPANISH    // set this flag after the above flag to indicate the custom input method is Spanish
#include <HID-Project.h>

// Define the size of rows and cols array
#define rowPinLength 1
#define colPinLength 2

// Define the pins used for the rows and columns of switches
const int rowPin[rowPinLength] = {A5};
const int colPin[colPinLength] = {A4,A3};

// Define the pin used for RGBWs
const int rgbw = A2;

// Define the keys and their corresponding functions
const KeyboardKeycode keys[rowPinLength][colPinLength] = {
  {SW1,SW2}
};

// Variables to store brightness level and brightness increment
int brightness = 0;
int brightnessInc = 51;

// Modify brightness intensity 
void updateBrightness() {                     
  brightness += brightnessInc;
  if (brightness == 0) {
    brightness = brightnessInc;
    analogWrite(rgbw, brightness);
  } else if (brightness > 255) {
    brightness = 0;
    digitalWrite(rgbw, LOW);
  } else {
    brightness += brightnessInc;
    analogWrite(rgbw, brightness);
  }
}

void setup() {
  for (int i=0; i<rowPinLength; i++){
    pinMode(rowPin[i], OUTPUT);         // Configure rowsPins as input with pull-up resistor
    digitalWrite(rowPin[i],LOW);
  }
   for (int i=0; i<colPinLength; i++){
    pinMode(colPin[i], INPUT_PULLUP);   // Configure colsPins as output with pull-up resistor
  }
  pinMode(rgbw, OUTPUT);                // Configure RGB pin as an output
  digitalWrite(rgbw, LOW);              // By default, LEDs are off    
  Keyboard.begin();                     // Sends a clean report to the host. This is important on any Arduino type.
}

void loop() {
  delay(5000);
  for (int i=0; i<rowPinLength; i++){
    digitalWrite(rowPin[i], HIGH);
    for (int j=0; j<colPinLength; j++){
      if (digitalRead(colPin[j]) == LOW){
        updateBrightness();
        delay(500);
      }
    }
  }
}
