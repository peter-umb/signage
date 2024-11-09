#ifndef ADVANCED_ANIMATIONS_H
#define ADVANCED_ANIMATIONS_H

#include <Adafruit_NeoPixel.h>
#include <algorithm> // Ensure you include this to use std::max

// Forward declare the Animations class
class Animations;

namespace AdvancedAnimations {
    void rainbowCycle(Animations& animations, uint8_t wait);
    void theaterChaseRainbow(Animations& animations, uint8_t wait);
    void meteorRain(Animations& animations, uint32_t color, uint8_t wait, uint8_t meteorSize, uint8_t meteorTrailDecay, bool randomDecay);
    void bouncingBalls(Animations& animations, uint8_t ballCount, float gravity, float dampening);
    void fireFlicker(Animations& animations, uint32_t baseColor, uint8_t flickerIntensity, uint8_t wait);
    void fireFlicker(Animations& animations, uint8_t wait);
    void randomFadeAnimation(Animations& animations, uint8_t wait);
    void meteorShower(Animations& animations, uint32_t color, uint8_t meteorSize, uint8_t trailDecay, uint8_t wait);
    void fireflies(Animations& animations, uint8_t wait);
    void colorWaves(Animations& animations, uint8_t wait);
    void randomSparkles(Animations& animations, uint8_t wait);
}

#endif // ADVANCED_ANIMATIONS_H
