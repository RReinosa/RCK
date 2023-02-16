#include "KeyAliases.h"

#define HID_CUSTOM_LAYOUT // set this flag to indicate that a custom layout is selected
#define LAYOUT_SPANISH    // set this flag after the above flag to indicate the custom input method is Spanish

#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>

// Define the size of rows and cols array
#define rowPinLength 7
#define colPinLength 16
// Define number of pixels
#define NUMPIXELS 97
// Define standard delay
#define DELAYVAL 500

// Define the pins used for the rows and columns of switches
const int rowPin[rowPinLength] = {A5};
const int colPin[colPinLength] = {A4,A3};

// Define the pin used for RGBWs
const int RGBW = A2;

// Define the keys and their corresponding functions
const KeyboardKeycode standard[rowPinLength][colPinLength] = {
  {STD_1,STD_2,STD_3,STD_4,STD_5,STD_6,STD_7,STD_8,STD_9,STD_10,STD_11,STD_12,STD_13,STD_14,STD_15},
  {STD_16,STD_17,STD_18,STD_19,STD_20,STD_21,STD_22,STD_23,STD_24,STD_25,STD_26,STD_27,STD_28,STD_29,STD_30,STD_31},
  {STD_32,STD_33,STD_34,STD_35,STD_36,STD_37,STD_38,STD_39,STD_40,STD_41,STD_42,STD_43,STD_44,STD_45,STD_46,STD_47},
  {STD_48,STD_49,STD_50,STD_51,STD_52,STD_53,STD_54,STD_55,STD_56,STD_57,STD_58,STD_59,STD_60,STD_61,STD_62,STD_63},
  {STD_64,STD_65,STD_66,STD_67,STD_68,STD_69,STD_70,STD_71,STD_72,STD_73,STD_74,STD_75,STD_76,STD_77,STD_78,STD_79},
  {STD_80,STD_81,STD_82,STD_83,STD_84,STD_85,STD_86,STD_87,STD_88,STD_89,STD_90,STD_91,STD_92,STD_93,STD_94,STD_95},
  {STD_96,STD_97}
};

// Variables to store brightness level and brightness increment
uint8_t brightness = 0;
uint8_t brightnessInc = 31;

Adafruit_NeoPixel pixels(NUMPIXELS, RGBW, NEO_GRB + NEO_KHZ800);

const uint32_t WHITE = strip.Color(255, 255, 255);
const uint32_t BLUE = strip.Color(0, 0, 255);
const uint32_t RED = strip.Color(255, 0, 0);
const uint32_t YELLOW = strip.Color(255, 255, 0);
const 

void restoreLeds(){
  pixels.clear();
  pixels.setBrightness(brightness);
  pixels.show();
  delay(DELAYVAL);
}

void updateColor(){
  
}

void updateBrightness(){
  brightness += brightnessInc;
  if(brightness>127) brightness = 0;
  pixels.setBrightness(brightness);
  pixels.show();
  delay(DELAYVAL);
}

void setup() {
  for (int i=0; i<rowPinLength; i++){
    pinMode(rowPin[i], OUTPUT);         // Configure rowsPins as input with pull-up resistor
    digitalWrite(rowPin[i],LOW);
  }
  for (int i=0; i<colPinLength; i++){
    pinMode(colPin[i], INPUT_PULLUP);   // Configure colsPins as output with pull-up resistor
  }  
  Keyboard.begin();                     // Sends a clean report to the host. This is important on any Arduino type.
  pixels.begin();
  restoreLeds();
}

void loop() {

}
