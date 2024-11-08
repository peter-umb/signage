#include <Adafruit_NeoPixel.h>

class Animations {
public:
  // Constructor: Initializes the Animations object with the NeoPixel strip
  Animations(Adafruit_NeoPixel& strip) : strip(strip) {}

  // 1. Rainbow Cycle Animation
  void rainbowCycle(uint8_t wait) {
    for(uint16_t j = 0; j < 256; j++) {
      for(uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + j) & 255));
      }
      strip.show();
      delay(wait);
    }
  }

  // 2. Color Wipe Animation
  void colorWipe(uint32_t color, uint8_t wait) {
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
      strip.show();
      delay(wait);
    }
  }

  // 3. Theater Chase Animation
  void theaterChase(uint32_t color, uint8_t wait) {
    for(uint16_t j = 0; j < 10; j++) {  // Do 10 cycles of chasing
      for(uint16_t q = 0; q < 3; q++) {
        for(uint16_t i = 0; i < strip.numPixels(); i += 3) {
          strip.setPixelColor(i + q, color);    // Turn every third pixel on
        }
        strip.show();
        delay(wait);
        for(uint16_t i = 0; i < strip.numPixels(); i += 3) {
          strip.setPixelColor(i + q, 0);        // Turn every third pixel off
        }
      }
    }
  }

  // 4. Theater Chase Rainbow Animation
  void theaterChaseRainbow(uint8_t wait) {
    for(uint16_t j = 0; j < 256; j++) {  // Cycle all colors on the wheel
      for(uint16_t q = 0; q < 3; q++) {
        for(uint16_t i = 0; i < strip.numPixels(); i += 3) {
          strip.setPixelColor(i + q, Wheel((i + j) % 255)); // Set color
        }
        strip.show();
        delay(wait);
        for(uint16_t i = 0; i < strip.numPixels(); i += 3) {
          strip.setPixelColor(i + q, 0); // Turn off
        }
      }
    }
  }

  // 5. Twinkle Animation
  void twinkle(uint32_t color, uint8_t count, uint8_t wait) {
    for(uint8_t i = 0; i < count; i++) {
      int pixel = random(strip.numPixels());
      strip.setPixelColor(pixel, color);
      strip.show();
      delay(wait);
      strip.setPixelColor(pixel, 0);
    }
  }

  // 6. Flash Animation
  void flash(uint32_t color, uint8_t flashes, uint8_t wait) {
    for(uint8_t i = 0; i < flashes; i++) {
      setAllPixels(color);
      strip.show();
      delay(wait);
      setAllPixels(0); // Off
      strip.show();
      delay(wait);
    }
  }

private:
  Adafruit_NeoPixel& strip;

  // Helper function for smooth color transitions in the rainbow
  uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else if(WheelPos < 170) {
      WheelPos -= 85;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } else {
      WheelPos -= 170;
      return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
  }

  // Helper function to set all LEDs to a color
  void setAllPixels(uint32_t color) {
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
  }
};
