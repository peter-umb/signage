#include "Animations.h"
#include "BasicAnimations.h"
#include "AdvancedAnimations.h"

// Define the array of animation functions
const Animations::AnimationFunction Animations::animations[] = {
  BasicAnimations::solidColor,
    BasicAnimations::colorWipe,
    BasicAnimations::theaterChase,
    BasicAnimations::flash,
    BasicAnimations::twinkle,
    BasicAnimations::runningLights,
    BasicAnimations::colorChase,
    BasicAnimations::wave,
    BasicAnimations::sparkle,
    BasicAnimations::gradientSweep,
    AdvancedAnimations::randomFadeAnimation,
    AdvancedAnimations::fireFlicker,
    AdvancedAnimations::rainbowCycle,
    AdvancedAnimations::theaterChaseRainbow,
    AdvancedAnimations::meteorRain,
    AdvancedAnimations::bouncingBalls,
    AdvancedAnimations::meteorShower,
    AdvancedAnimations::fireFlicker2,
    AdvancedAnimations::fireflies,
};

// Constructor
Animations::Animations(Adafruit_NeoPixel &strip, HelperFunctions &helperFunctions, boolean debug)
    : strip(strip), helperFunctions(helperFunctions), debug(debug)
{
    // Initialize animation states based on the number of animations
    animationStates.resize(sizeof(animations) / sizeof(animations[0]), AnimationState());
}

void Animations::runCurrentAnimation(uint8_t wait)
{
    if (currentAnimationIndex < (sizeof(animations) / sizeof(animations[0]))) {
        animations[currentAnimationIndex](*this, wait);
    }

    if (debug) {
        Serial.print("Finished running animation index: ");
        Serial.println(currentAnimationIndex);
    }
}

uint8_t Animations::getTotalAnimations() const
{
    return sizeof(animations) / sizeof(animations[0]);
}

void Animations::setRandomColor()
{
    setCurrentColor(helperFunctions.getRandomColor());
}

void Animations::cycleAnimation()
{
    helperFunctions.wipeStrip();
    currentAnimationIndex = (currentAnimationIndex + 1) % getTotalAnimations();
    setAnimation(currentAnimationIndex);
}

void Animations::setCurrentColor(uint8_t red, uint8_t green, uint8_t blue)
{
    currentColor = helperFunctions.getColor(red, green, blue);
}

void Animations::setCurrentColor(uint32_t color)
{
    currentColor = color;
}

void Animations::setAnimation(uint8_t animationIndex)
{
    if (debug)
    {
        Serial.print("Setting animation index: ");
        Serial.println(animationIndex);
    }

    currentAnimationIndex = animationIndex;
    animationStates[animationIndex] = AnimationState(); // Reset state for the selected animation
}
