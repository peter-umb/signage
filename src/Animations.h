#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Adafruit_NeoPixel.h>

class Animations {
public:
  // Constructor
  Animations(Adafruit_NeoPixel& strip) : strip(strip) {}

  // Primary animations
  void colorWipe(uint32_t color, uint8_t wait);
  void theaterChase(uint32_t color, uint8_t wait);
  void flash(uint32_t color, uint8_t flashes, uint8_t wait);
  void rainbowCycle(uint8_t wait);
  void theaterChaseRainbow(uint8_t wait);
  void twinkle(uint32_t color, uint8_t count, uint8_t wait);
  void runningLights(uint32_t color, uint8_t wait);
  void meteorRain(uint32_t color, uint8_t wait, uint8_t meteorSize, uint8_t meteorTrailDecay, bool randomDecay);
  void bouncingBalls(uint8_t ballCount, uint8_t gravity, uint8_t dampening, int iterations);
  void fireFlicker(uint32_t baseColor, uint8_t flickerIntensity, uint8_t wait);
  void colorChase(uint32_t color, uint8_t wait);

private:
  Adafruit_NeoPixel& strip;

  // Helper functions
  void setPixelColorWithFade(int pixel, uint32_t color, uint8_t fadeAmount);
  uint32_t Wheel(byte WheelPos);
  void setAllPixels(uint32_t color);
};

// Color Wipe Animation
void Animations::colorWipe(uint32_t color, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

// Theater Chase Animation
void Animations::theaterChase(uint32_t color, uint8_t wait) {
  for (uint16_t j = 0; j < 10; j++) { 
    for (uint16_t q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, color);
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, 0);
      }
    }
  }
}

// Flash Animation
void Animations::flash(uint32_t color, uint8_t flashes, uint8_t wait) {
  for (uint8_t i = 0; i < flashes; i++) {
    setAllPixels(color);
    strip.show();
    delay(wait);
    setAllPixels(0);
    strip.show();
    delay(wait);
  }
}

// Rainbow Cycle Animation
void Animations::rainbowCycle(uint8_t wait) {
  for (uint16_t j = 0; j < 256; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Theater Chase Rainbow Animation
void Animations::theaterChaseRainbow(uint8_t wait) {
  for (uint16_t j = 0; j < 256; j++) {
    for (uint16_t q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, Wheel((i + j) % 255));
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, 0);
      }
    }
  }
}

// Twinkle Animation
void Animations::twinkle(uint32_t color, uint8_t count, uint8_t wait) {
  for (uint8_t i = 0; i < count; i++) {
    int pixel = random(strip.numPixels());
    strip.setPixelColor(pixel, color);
    strip.show();
    delay(wait);
    strip.setPixelColor(pixel, 0);
  }
}

// Running Lights Animation
void Animations::runningLights(uint32_t color, uint8_t wait) {
  for (int j = 0; j < 256 * 2; j++) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, ((sin(i + j) * 127 + 128) / 255) * color);
    }
    strip.show();
    delay(wait);
  }
}

// Meteor Rain Animation
void Animations::meteorRain(uint32_t color, uint8_t wait, uint8_t meteorSize, uint8_t meteorTrailDecay, bool randomDecay) {
  for (int i = 0; i < strip.numPixels() * 2; i++) {
    for (int j = 0; j < strip.numPixels(); j++) {
      if (!randomDecay || random(10) > 5) {
        setPixelColorWithFade(j, strip.getPixelColor(j), meteorTrailDecay);
      }
    }
    for (int j = 0; j < meteorSize; j++) {
      if ((i - j) < strip.numPixels() && (i - j) >= 0) {
        strip.setPixelColor(i - j, color);
      }
    }
    strip.show();
    delay(wait);
  }
}

// Bouncing Balls Animation
void Animations::bouncingBalls(uint8_t ballCount, uint8_t gravity, uint8_t dampening, int iterations) {
  float height[ballCount];
  float impactVelocityStart = sqrt(-2 * gravity);
  float impactVelocity[ballCount];
  
  for (int i = 0; i < ballCount; i++) {
    height[i] = 1;
    impactVelocity[i] = impactVelocityStart;
  }

  for (int t = 0; t < iterations; t++) {  // Run for a limited number of iterations
    for (int i = 0; i < ballCount; i++) {
      height[i] += impactVelocity[i];
      impactVelocity[i] += gravity;
      if (height[i] < 0) {
        height[i] = 0;
        impactVelocity[i] = impactVelocity[i] * -dampening;
      }
      strip.setPixelColor(i, strip.Color(255, 0, 0));  // Customize color as needed
    }
    strip.show();
    delay(10);
  }
}

// Fire Flicker Animation
void Animations::fireFlicker(uint32_t baseColor, uint8_t flickerIntensity, uint8_t wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    int flicker = random(-flickerIntensity, flickerIntensity);
    uint8_t r = constrain((baseColor >> 16 & 0xFF) + flicker, 0, 255);
    uint8_t g = constrain((baseColor >> 8 & 0xFF) + flicker, 0, 255);
    uint8_t b = constrain((baseColor & 0xFF) + flicker, 0, 255);
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
  delay(wait);
}

// Color Chase Animation
void Animations::colorChase(uint32_t color, uint8_t wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
    strip.setPixelColor(i, 0);
  }
}

// Helper function to set color with fade
void Animations::setPixelColorWithFade(int pixel, uint32_t color, uint8_t fadeAmount) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;
  r = r * fadeAmount / 256;
  g = g * fadeAmount / 256;
  b = b * fadeAmount / 256;
  strip.setPixelColor(pixel, strip.Color(r, g, b));
}

// Helper function for smooth color transitions in rainbow
uint32_t Animations::Wheel(byte WheelPos) {
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

// Helper function to set all LEDs to a color
void Animations::setAllPixels(uint32_t color) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
}

#endif  // ANIMATIONS_H
