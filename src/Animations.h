#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Adafruit_NeoPixel.h>
#include <vector>
#include "HelperFunctions.h"


class Animations {
public:
  // Constructor
  Animations(Adafruit_NeoPixel& strip, HelperFunctions& helperFunctions, boolean debug);

  // Must be incremented when more animations are added or removed.
  static const int animation_count = 20;

  // Unified structure to hold animation state
  struct AnimationState {
    int pos = 0;     // Generic position
    int step = 0;    // Step counter
    int cycle = 0;   // Cycle counter for multi-cycle animations
  };

  void setAnimationColor(uint32_t color);

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
  void randomFadeAnimation(uint8_t wait);
  void fireFlicker(uint8_t wait);
  void wave(uint8_t wait);
  void meteorShower(uint32_t color, uint8_t meteorSize, uint8_t trailDecay, uint8_t wait);
  void sparkle(uint32_t color, uint8_t wait);
  void fireflies(uint8_t wait);
  void gradientSweep(uint8_t wait);
  void colorWaves(uint8_t wait);
  void randomSparkles(uint8_t wait);

  // Helper functions
  uint8_t getTotalAnimations() const;  // Method to return the total number of animations
  void setRandomColor();
  void cycleAnimation();

private:
  boolean debug;
  Adafruit_NeoPixel& strip;
  HelperFunctions& helperFunctions;

  // Animation state
  uint32_t currentColor;  // Add currentColor as a member variable
  AnimationState animationStates[animation_count];  // State for each animation
  AnimationState state;
  uint8_t currentAnimationIndex = 0;  // Index of the current animation
  int animationIndex = 0;

  // Helper functions
  void setCurrentColor(uint8_t red, uint8_t green, uint8_t blue);
  void setCurrentColor(uint32_t color);

};

#endif  // ANIMATIONS_H
