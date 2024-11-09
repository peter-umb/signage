#ifndef BASIC_ANIMATIONS_H
#define BASIC_ANIMATIONS_H

#include <Adafruit_NeoPixel.h>

// Forward declare the Animations class
class Animations;

namespace BasicAnimations
{
    void colorWipe(Animations &animations, uint32_t color, uint8_t wait);
    void theaterChase(Animations &animations, uint32_t color, uint8_t wait);
    void flash(Animations &animations, uint32_t color, uint8_t flashes, uint8_t wait);
    void twinkle(Animations &animations, uint32_t color, uint8_t count, uint8_t wait);
    void runningLights(Animations &animations, uint32_t color, uint8_t wait);
    void colorChase(Animations &animations, uint32_t color, uint8_t wait);
    void wave(Animations &animations, uint8_t wait);
    void sparkle(Animations &animations, uint32_t color, uint8_t wait);
    void gradientSweep(Animations &animations, uint8_t wait);
}

#endif // BASIC_ANIMATIONS_H
