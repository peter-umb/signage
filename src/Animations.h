#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Adafruit_NeoPixel.h>

class Animations {
public:
  // Constructor
  Animations(Adafruit_NeoPixel& strip);

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
  void bouncingBalls(uint8_t ballCount, float gravity, float dampening);
  void fireFlicker(uint32_t baseColor, uint8_t flickerIntensity, uint8_t wait);
  void colorChase(uint32_t color, uint8_t wait);
  uint8_t getTotalAnimations() const;  // Method to return the total number of animations
  uint32_t Wheel(byte WheelPos);

private:
  Adafruit_NeoPixel& strip;

  uint32_t currentColor;  // Add currentColor as a member variable

  AnimationState animationStates[11];  // State for each animation
  AnimationState state;

  uint8_t currentAnimationIndex = 0;  // Index of the current animation


  // Helper functions
  void setPixelColorWithFade(int pixel, uint32_t color, uint8_t fadeAmount);
  void setAllPixels(uint32_t color);
};


#endif  // ANIMATIONS_H
