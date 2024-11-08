#include "Animations.h"

// Constructor
Animations::Animations(Adafruit_NeoPixel& strip) : strip(strip) {}

// Set the animation and reset state
void Animations::setAnimation(uint8_t animationIndex) {
  Serial.println("Animations::setAnimation animationIndex: " + String(animationIndex) );

  currentAnimationIndex = animationIndex;
  animationStates[animationIndex] = AnimationState();  // Reset state for the selected animation
}

uint8_t Animations::getTotalAnimations() const {
    return animation_count;
}

// Run the current animation incrementally
void Animations::runCurrentAnimation(uint8_t wait) {
    // Serial.print("Running animation index: ");
    // Serial.println(currentAnimationIndex);

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

    // Serial.print("Finished running animation index: ");
    // Serial.println(currentAnimationIndex);
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
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    // Apply modulo 256 directly in the color calculation to wrap smoothly
    strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + state.step) % 256));
  }
  strip.show();
  delay(wait);

  // Increment state.step continuously without resetting
  state.step++;
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
    state.step = (state.step + 1) % 256;
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
// Meteor Rain Animation
void Animations::meteorRain(uint32_t color, uint8_t wait, uint8_t meteorSize, uint8_t meteorTrailDecay, bool randomDecay) {
  if (state.step < strip.numPixels() * 2) {
    // Apply fade to all LEDs to create trailing effect
    for (int j = 0; j < strip.numPixels(); j++) {
      if (!randomDecay || random(10) > 5) {  // Randomized decay for a "sparkling" effect
        uint32_t pixelColor = strip.getPixelColor(j);
        setPixelColorWithFade(j, pixelColor, meteorTrailDecay);
      }
    }

    // Light up the meteor's "head" and trail
    for (int j = 0; j < meteorSize; j++) {
      int pixelPos = state.step - j;
      if (pixelPos < strip.numPixels() && pixelPos >= 0) {  // Ensure within strip bounds
        strip.setPixelColor(pixelPos, color);  // Set color for the meteor
      }
    }

    // Display changes and move the meteor head forward
    strip.show();
    delay(wait);
    state.step++;
  } else {
    // Reset to allow cycling
    state.step = 0;
  }
}

// Bouncing Balls Animation
void Animations::bouncingBalls(uint8_t ballCount, float gravity, float dampening) {
    static float height[10];
    static float impactVelocity[10];
    static bool initialized = false;

    if (!initialized) {
        float impactVelocityStart = sqrt(2.0 * abs(gravity));  // Ensure positive value here
        for (int i = 0; i < ballCount; i++) {
            height[i] = 16.0;  // Set initial height
            impactVelocity[i] = impactVelocityStart;  // Use calculated starting velocity
            Serial.print("Initial height: ");
            Serial.print(height[i]);
            Serial.print(", Initial velocity: ");
            Serial.println(impactVelocity[i]);
        }
        initialized = true;
    }

    for (int i = 0; i < ballCount; i++) {
        height[i] += impactVelocity[i];
        impactVelocity[i] += gravity;

        if (height[i] < 0) {
            height[i] = 0;
            impactVelocity[i] *= -dampening;  // Reverse direction with dampening
        }

        // Set color for each ball
        strip.setPixelColor(i, strip.Color(255, 0, 0));  // Customize color as needed
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