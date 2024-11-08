#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Adafruit_NeoPixel.h>

class Animations {
public:
  // Constructor
  Animations(Adafruit_NeoPixel& strip) : strip(strip) {}

  // Unified structure to hold animation state
  struct AnimationState {
    int pos = 0;     // Generic position
    int step = 0;    // Step counter
    int cycle = 0;   // Cycle counter for multi-cycle animations
  };

  void setAnimationColor(uint32_t color) {
    currentColor = color;
  };

  // Set the current animation and reset its state
  void setAnimation(uint8_t animationIndex);

  // Run the currently selected animation in incremental steps
  void runCurrentAnimation(uint8_t wait);

  // Animation functions (with incremental execution)
  void colorWipe(uint32_t color, uint8_t wait);
  void theaterChase(uint32_t color, uint8_t wait);
  void flash(uint32_t color, uint8_t flashes, uint8_t wait);
  void rainbowCycle(uint8_t wait);
  void theaterChaseRainbow(uint8_t wait);
  void twinkle(uint32_t color, uint8_t count, uint8_t wait);
  void runningLights(uint32_t color, uint8_t wait);
  void meteorRain(uint32_t color, uint8_t wait, uint8_t meteorSize, uint8_t meteorTrailDecay, bool randomDecay);
  void bouncingBalls(uint8_t ballCount, uint8_t gravity, uint8_t dampening);
  void fireFlicker(uint32_t baseColor, uint8_t flickerIntensity, uint8_t wait);
  void colorChase(uint32_t color, uint8_t wait);

private:
  Adafruit_NeoPixel& strip;

  uint32_t currentColor;  // Add currentColor as a member variable

  AnimationState animationStates[10];  // State for each animation
  AnimationState state;

  uint8_t currentAnimationIndex = 0;  // Index of the current animation


  // Helper functions
  uint32_t Wheel(byte WheelPos);
  void setPixelColorWithFade(int pixel, uint32_t color, uint8_t fadeAmount);
  void setAllPixels(uint32_t color);
};

// Set the animation and reset state
void Animations::setAnimation(uint8_t animationIndex) {
  Serial.println("Animations::setAnimation animationIndex: " + String(animationIndex) );

  currentAnimationIndex = animationIndex;
  animationStates[animationIndex] = AnimationState();  // Reset state for the selected animation
}

// Run the current animation incrementally
void Animations::runCurrentAnimation(uint8_t wait) {
    Serial.print("Running animation index: ");
    Serial.println(currentAnimationIndex);

    switch (currentAnimationIndex) {
        case 0:
            colorWipe(currentColor, wait);
            break;
        case 1:
            theaterChase(currentColor, wait);
            break;
        case 2:
            flash(currentColor, 5, wait);
            break;
        case 3:
            rainbowCycle(wait);
            break;
        case 4:
            theaterChaseRainbow(wait);
            break;
        case 5:
            twinkle(currentColor, 10, wait);
            break;
        case 6:
            runningLights(currentColor, wait);
            break;
        case 7:
            meteorRain(currentColor, wait, 3, 4, true);
            break;
        case 8:
            bouncingBalls(3, 1, 2);
            break;
        case 9:
            fireFlicker(currentColor, 10, wait);
            break;
        case 10:
            colorChase(currentColor, wait);
            break;
        default:
            twinkle(currentColor, 10, wait);  // Fallback animation
            break;
    }

    Serial.print("Finished running animation index: ");
    Serial.println(currentAnimationIndex);
}

// Non-blocking colorWipe
void Animations::colorWipe(uint32_t color, uint8_t wait) {
  if (state.pos < strip.numPixels()) {
    strip.setPixelColor(state.pos, color);
    strip.show();
    delay(wait);
    state.pos++;
  } else {
    state.pos = 0;  // Reset to allow cycling
  }
}

// Non-blocking theaterChase
void Animations::theaterChase(uint32_t color, uint8_t wait) {
  if (state.cycle < 10) {
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, state.step % 3 == q ? color : 0);
      }
    }
    strip.show();
    delay(wait);
    state.step++;
    if (state.step % 3 == 0) state.cycle++;
  } else {
    state.cycle = 0;
    state.step = 0;
  }
}

// Flash animation
void Animations::flash(uint32_t color, uint8_t flashes, uint8_t wait) {
  if (state.cycle < flashes) {
    setAllPixels(state.cycle % 2 == 0 ? color : 0);
    strip.show();
    delay(wait);
    state.cycle++;
  } else {
    state.cycle = 0;
  }
}

// Non-blocking rainbowCycle
void Animations::rainbowCycle(uint8_t wait) {
  if (state.step < 256) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + state.step) & 255));
    }
    strip.show();
    delay(wait);
    state.step++;
  } else {
    state.step = 0;  // Reset to allow cycling
  }
}

// Non-blocking theaterChaseRainbow
void Animations::theaterChaseRainbow(uint8_t wait) {
  if (state.step < 256) {
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, Wheel((i + state.step) % 255));
      }
    }
    strip.show();
    delay(wait);
    state.step++;
  } else {
    state.step = 0;  // Reset to allow cycling
  }
}

// Twinkle Animation
void Animations::twinkle(uint32_t color, uint8_t count, uint8_t wait) {
  if (state.step < count) {
    int pixel = random(strip.numPixels());
    strip.setPixelColor(pixel, color);
    strip.show();
    delay(wait);
    strip.setPixelColor(pixel, 0);
    state.step++;
  } else {
    state.step = 0;  // Reset to allow cycling
  }
}

// Running Lights Animation
void Animations::runningLights(uint32_t color, uint8_t wait) {
  if (state.step < 256 * 2) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, ((sin(i + state.step) * 127 + 128) / 255) * color);
    }
    strip.show();
    delay(wait);
    state.step++;
  } else {
    state.step = 0;  // Reset for the next cycle
  }
}

// Meteor Rain Animation
void Animations::meteorRain(uint32_t color, uint8_t wait, uint8_t meteorSize, uint8_t meteorTrailDecay, bool randomDecay) {
  if (state.step < strip.numPixels() * 2) {
    for (int j = 0; j < strip.numPixels(); j++) {
      if (!randomDecay || random(10) > 5) {
        setPixelColorWithFade(j, strip.getPixelColor(j), meteorTrailDecay);
      }
    }
    for (int j = 0; j < meteorSize; j++) {
      if ((state.step - j) < strip.numPixels() && (state.step - j) >= 0) {
        strip.setPixelColor(state.step - j, color);
      }
    }
    strip.show();
    delay(wait);
    state.step++;
  } else {
    state.step = 0;  // Reset to allow cycling
  }
}

// Bouncing Balls Animation
void Animations::bouncingBalls(uint8_t ballCount, uint8_t gravity, uint8_t dampening) {
  static float height[10];
  static float impactVelocity[10];
  static bool initialized = false;

  if (!initialized) {
    float impactVelocityStart = sqrt(-2 * gravity);
    for (int i = 0; i < ballCount; i++) {
      height[i] = 1;
      impactVelocity[i] = impactVelocityStart;
    }
    initialized = true;
  }

  if (state.step < ballCount) {
    height[state.step] += impactVelocity[state.step];
    impactVelocity[state.step] += gravity;
    if (height[state.step] < 0) {
      height[state.step] = 0;
      impactVelocity[state.step] *= -dampening;
    }
    strip.setPixelColor(state.step, strip.Color(255, 0, 0));  // Customize color as needed
    state.step++;
  } else {
    state.step = 0;
  }
  strip.show();
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
  if (state.pos < strip.numPixels()) {
    strip.setPixelColor(state.pos, color);
    strip.show();
    delay(wait);
    strip.setPixelColor(state.pos, 0);  // Turn off the current pixel before moving to the next
    state.pos++;
  } else {
    state.pos = 0;  // Reset to allow cycling
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

// Color Wheel function for rainbow effects
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

// Set all pixels to a specific color
void Animations::setAllPixels(uint32_t color) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
}

#endif  // ANIMATIONS_H
