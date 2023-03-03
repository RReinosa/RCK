#define HID_CUSTOM_LAYOUT // set this flag to indicate that a custom layout is selected
#define LAYOUT_SPANISH    // set this flag after the above flag to indicate the custom input method is Spanish
#include <HID-Project.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#define rowPinLength 7        // Define the size of rows array
#define colPinLength 16        // Define the size of cols array
#define muxPinLength 5
#define DELAYVAL 1000         // Define standard delay
#define LED_COUNT 97          // Define number of pixels
#define FNROW 6               // Define FN row
#define FNCOL 11              // Define FN col

const int rowPin[rowPinLength] = {A5, A4, A3, A2, A1, A0, 5};      // Define the pins used for the rows and columns of switches
const int colPin = 12;
const int muxPin[muxPinLength] = {6, 8, 9, 10, 4};
const int LED_PIN = 3;                                             // Define the pin used for RGBWs

const KeyboardKeycode standard[rowPinLength][colPinLength] = {
  {STD_01, STD_02, STD_03, STD_04, STD_05, STD_06, STD_07, STD_08, STD_09, STD_10, STD_11, STD_12, STD_13, STD_14, STD_15, XXXXXX},
  {STD_16, STD_17, STD_18, STD_19, STD_20, STD_21, STD_22, STD_23, STD_24, STD_25, STD_26, STD_27, STD_28, STD_29, STD_30, STD_31},
  {STD_32, STD_33, STD_34, STD_35, STD_36, STD_37, STD_38, STD_39, STD_40, STD_41, STD_42, STD_43, STD_44, STD_45, STD_46, STD_47},
  {STD_48, STD_49, STD_50, STD_51, STD_52, STD_53, STD_54, STD_55, STD_56, STD_57, STD_58, STD_59, STD_60, STD_61, STD_62, STD_63},
  {STD_64, STD_65, STD_66, STD_67, STD_68, STD_69, STD_70, STD_71, STD_72, STD_73, STD_74, STD_75, STD_76, STD_77, STD_78, STD_79},
  {STD_80, STD_81, STD_82, STD_83, STD_84, STD_85, STD_86, STD_87, STD_88, STD_89, STD_90, STD_91, STD_92, STD_93, STD_94, STD_95},
  {STD_96, STD_97, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX}
};

boolean state[rowPinLength][colPinLength] {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800); // Initialize Adafruit module


uint32_t getColorEEPROM() {                      // read brightness value from the EEPROM
  uint8_t white = EEPROM.read(0);
  uint8_t red   = EEPROM.read(1);
  uint8_t green = EEPROM.read(2);
  uint8_t blue  = EEPROM.read(3);

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
    case 3:
      updateBrightness();
    case 4:
      updateBrightness();
    case 5:
      Keyboard.press(MEDIA_PREVIOUS);
      delayMicroseconds(DELAYVAL);
    case 6:
      Keyboard.press(MEDIA_NEXT);
      delayMicroseconds(DELAYVAL);
    case 7:
      Keyboard.press(MEDIA_PLAY_PAUSE);
      delayMicroseconds(DELAYVAL);
    case 8:
      Keyboard.press(MEDIA_STOP);
      delayMicroseconds(DELAYVAL);
    case 9:
      Keyboard.press(MEDIA_VOLUME_MUTE);
      delayMicroseconds(DELAYVAL);
    case 10:
      Keyboard.press(MEDIA_VOLUME_DOWN);
      delayMicroseconds(DELAYVAL);
    case 11:
      Keyboard.press(MEDIA_VOLUME_UP);
      delayMicroseconds(DELAYVAL);
    case 12:
      Keyboard.press(KEY_LEFT_CTRL);
      delayMicroseconds(DELAYVAL);
      Keyboard.press(KEY_LEFT_SHIFT);
      delayMicroseconds(DELAYVAL);
      Keyboard.press(KEY_S);
    default:
      break;
  }
}
