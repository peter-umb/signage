#ifndef ADVANCED_ANIMATIONS_H
#define ADVANCED_ANIMATIONS_H

#include <Adafruit_NeoPixel.h>
#include <algorithm> // Ensure you include this to use std::max

// Forward declare the Animations class
class Animations;

namespace AdvancedAnimations {
    void rainbowCycle(Animations& animations, uint8_t wait);
    void theaterChaseRainbow(Animations& animations, uint8_t wait);
    void meteorRain(Animations& animations, uint8_t wait);
    void bouncingBalls(Animations& animations, uint8_t wait);
    void fireFlicker(Animations& animations, uint8_t wait);
    void fireFlicker2(Animations& animations, uint8_t wait);
    void randomFadeAnimation(Animations& animations, uint8_t wait);
    void meteorShower(Animations& animations, uint8_t wait);
    void fireflies(Animations& animations, uint8_t wait);
    void colorWaves(Animations& animations, uint8_t wait);
    void randomSparkles(Animations& animations, uint8_t wait);
}

#endif // ADVANCED_ANIMATIONS_H
