// #ifndef ANIMATIONS_H
// #define ANIMATIONS_H

// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>
// #include "HelperFunctions.h"
// #include "BasicAnimations.h"
// // #include "AdvancedAnimations.h"
// // #include "UtilityFunctions.h"

// class Animations {
// public:
//   // Constructor
//   Animations(Adafruit_NeoPixel& strip, HelperFunctions& helperFunctions, boolean debug);

//   // Must be incremented when more animations are added or removed.
//   static const int animation_count = 20;

//   // Unified structure to hold animation state
//   struct AnimationState {
//     int pos = 0;     // Generic position
//     int step = 0;    // Step counter
//     int cycle = 0;   // Cycle counter for multi-cycle animations
//   };

//   void setAnimationColor(uint32_t color);

//   // Set the current animation and reset its state
//   void setAnimation(uint8_t animationIndex);

//   // Run the currently selected animation in incremental steps
//   void runCurrentAnimation(uint8_t wait);

//   // Animation functions (with incremental execution)
//   void colorWipe(uint32_t color, uint8_t wait);
//   void theaterChase(uint32_t color, uint8_t wait);
//   void flash(uint32_t color, uint8_t flashes, uint8_t wait);
//   void rainbowCycle(uint8_t wait);
//   void theaterChaseRainbow(uint8_t wait);
//   void twinkle(uint32_t color, uint8_t count, uint8_t wait);
//   void runningLights(uint32_t color, uint8_t wait);
//   void meteorRain(uint32_t color, uint8_t wait, uint8_t meteorSize, uint8_t meteorTrailDecay, bool randomDecay);
//   void bouncingBalls(uint8_t ballCount, float gravity, float dampening);
//   void fireFlicker(uint32_t baseColor, uint8_t flickerIntensity, uint8_t wait);
//   void colorChase(uint32_t color, uint8_t wait);
//   void randomFadeAnimation(uint8_t wait);
//   void fireFlicker(uint8_t wait);
//   void wave(uint8_t wait);
//   void meteorShower(uint32_t color, uint8_t meteorSize, uint8_t trailDecay, uint8_t wait);
//   void sparkle(uint32_t color, uint8_t wait);
//   void fireflies(uint8_t wait);
//   void colorWaves(uint8_t wait);
//   void randomSparkles(uint8_t wait);

//   // Helper functions
//   uint8_t getTotalAnimations() const;  // Method to return the total number of animations
//   void setRandomColor();
//   void cycleAnimation();

// private:
//   boolean debug;
//   Adafruit_NeoPixel& strip;
//   HelperFunctions& helperFunctions;

//   // Animation state
//   uint32_t currentColor;  // Add currentColor as a member variable
//   AnimationState animationStates[animation_count];  // State for each animation
//   AnimationState state;
//   uint8_t currentAnimationIndex = 0;  // Index of the current animation
//   int animationIndex = 0;

// };

// #endif  // ANIMATIONS_H

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "HelperFunctions.h"
#include "BasicAnimations.h"
#include "AdvancedAnimations.h"

class Animations
{
public:
  boolean debug;
  static const int animation_count = 20;
  Animations(Adafruit_NeoPixel &strip, HelperFunctions &helperFunctions, boolean debug);

  void setAnimation(uint8_t animationIndex);
  void runCurrentAnimation(uint8_t wait);
  uint8_t getTotalAnimations() const;
  void setRandomColor();
  void cycleAnimation();

  Adafruit_NeoPixel &strip; // Make strip accessible to BasicAnimations
  HelperFunctions &helperFunctions;
  struct AnimationState
  {
    int pos = 0;
    int step = 0;
    int cycle = 0;
  } state;
  AnimationState animationStates[animation_count];

  // Animation state
  uint32_t currentColor;
  int animationIndex = 0;
  uint8_t currentAnimationIndex = 0; // Index of the current animation
  // Helper functions
  void setCurrentColor(uint8_t red, uint8_t green, uint8_t blue);
  void setCurrentColor(uint32_t color);
};

#endif // ANIMATIONS_H