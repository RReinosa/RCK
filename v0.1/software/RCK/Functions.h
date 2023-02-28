#define HID_CUSTOM_LAYOUT // set this flag to indicate that a custom layout is selected
#define LAYOUT_SPANISH    // set this flag after the above flag to indicate the custom input method is Spanish
#include <HID-Project.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#define rowPinLength 1      // Define the size of rows array
#define colPinLength 2      // Define the size of cols array
#define DELAYVAL 1000        // Define standard delay
#define LED_COUNT 2         // Define number of pixels
#define FNROW 0             // Define FN row
#define FNCOL 1             // Define FN col

const int rowPin[rowPinLength] = {A5};      // Define the pins used for the rows and columns of switches
const int colPin[colPinLength] = {A4, A3};
const int LED_PIN = 3;                      // Define the pin used for RGBWs

const KeyboardKeycode standard[rowPinLength][colPinLength] = {  // Define the keys and their corresponding functions
  {STD_1, STD_2}
};

boolean state[rowPinLength][colPinLength] {
  {0, 0}
};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800); // Initialize Adafruit module


uint32_t getColorEEPROM() {                      // read brightness value from the EEPROM
  uint8_t white = EEPROM.read(0);
  uint8_t red = EEPROM.read(1);
  uint8_t green = EEPROM.read(2);
  uint8_t blue = EEPROM.read(3);

  return ((uint32_t)white << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;
}

void updateColorEEPROM(uint32_t color) {
  uint8_t white = (color >> 24) & 0xFF;
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;

  EEPROM.update(0, white);
  EEPROM.update(1, red);
  EEPROM.update(2, green);
  EEPROM.update(3, blue);
}


void updateColor() {
  uint32_t color = getColorEEPROM();
  uint8_t white = (color >> 24) & 0xFF;
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;

  // Move the bits to alternate the colors
  uint8_t tmp;
  tmp = white;
  white = blue;
  blue = green;
  green = red;
  red = tmp;
  color = ((uint32_t)white << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;

  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color);          // Set the color of the all LEDs
    strip.show();                           // Update the LEDs with the new color
  }
  updateColorEEPROM(color);

  uint8_t currentColor = EEPROM.read(4);
  currentColor++;
  if (currentColor > 3) {
    currentColor = 0; //
  }
  EEPROM.update(4, currentColor);
}

void updateBrightness() {
  uint32_t color = getColorEEPROM();
  uint8_t white = (color >> 24) & 0xFF;
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;
  uint8_t currentColor = EEPROM.read(4);

  switch (currentColor) {
    case 0:
      white += 0x1F;
      if (white > 0x7F) {
        white = 0x0;
      }
      break;
    case 1:
      red += 0x1F;
      if (red > 0x7F) {
        red = 0x0;
      }
      break;
    case 2:
      green += 0x1F;
      if (green > 0x7F) {
        green = 0x0;
      }
      break;
    case 3:
      blue += 0x1F;
      if (blue > 0x7F) {
        blue = 0x0;
      }
      break;
    default:
      break;
  }

  color = ((uint32_t)white << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;

  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color);          // Set the color of the all LEDs
    strip.show();                           // Update the LEDs with the new color
  }
  updateColorEEPROM(color);
}

void fn(int key) {
  switch (key) {
    case 1:
      updateColor();
      break;
    case 2:
      updateBrightness();
    default:
      break;
  }
}
