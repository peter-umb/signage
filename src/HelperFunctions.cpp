#include "HelperFunctions.h"

HelperFunctions::HelperFunctions(Adafruit_NeoPixel& ledStrip, boolean debug) : strip(ledStrip), debug(debug) {}

uint32_t HelperFunctions::dimColor(uint32_t color, uint8_t factor) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    return strip.Color(r / factor, g / factor, b / factor);
}

uint32_t HelperFunctions::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}

void HelperFunctions::setPixelColorWithFade(int pixel, uint32_t color, uint8_t fadeAmount) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    r = r * fadeAmount / 256;
    g = g * fadeAmount / 256;
    b = b * fadeAmount / 256;
    strip.setPixelColor(pixel, strip.Color(r, g, b));
}

void HelperFunctions::setAllPixels(uint32_t color) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
}

uint32_t HelperFunctions::getColor(uint8_t red, uint8_t green, uint8_t blue) {
    return strip.Color(red, green, blue);
}

uint32_t HelperFunctions::getRandomColor() {
    return Wheel(random(0, 255));
}

int HelperFunctions::min(int a, uint32_t b) {
    return (a < b) ? a : b;
}

void HelperFunctions::wipeStrip() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

void HelperFunctions::cycleBrightness() {
  brightnessIndex = (brightnessIndex + 1) % 4;
  strip.setBrightness(brightnessLevels[brightnessIndex]);
  strip.show();
}

void HelperFunctions::setupLEDStrip() {
  strip.begin();
  strip.setBrightness(brightnessLevels[brightnessIndex]);
  strip.show();
}