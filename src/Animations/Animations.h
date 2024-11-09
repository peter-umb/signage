#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "HelperFunctions.h"
#include "BasicAnimations.h"
#include "AdvancedAnimations.h"

// Forward declaration of AnimationFunction type
class Animations
{
public:
    using AnimationFunction = void (*)(Animations&, uint8_t);
    
    Animations(Adafruit_NeoPixel &strip, HelperFunctions &helperFunctions, boolean debug);
    
    void setAnimation(uint8_t animationIndex);
    void runCurrentAnimation(uint8_t wait);
    uint8_t getTotalAnimations() const;
    void setRandomColor();
    void cycleAnimation();

    Adafruit_NeoPixel &strip;
    HelperFunctions &helperFunctions;

    uint32_t currentColor;
    uint8_t currentAnimationIndex = 0;
    bool debug;

    void setCurrentColor(uint8_t red, uint8_t green, uint8_t blue);
    void setCurrentColor(uint32_t color);

    struct AnimationState
    {
        int pos = 0;
        int step = 0;
        int cycle = 0;
        unsigned long lastUpdate = 0;
    };

    // Define a placeholder for animation states
    std::vector<AnimationState> animationStates;

private:
    static const AnimationFunction animations[];
};

#endif // ANIMATIONS_H
