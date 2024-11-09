#include "Animations.h"
#include "BasicAnimations.h" // Include for access to BasicAnimations namespace

Animations::Animations(Adafruit_NeoPixel &strip, HelperFunctions &helperFunctions, boolean debug)
    : strip(strip), helperFunctions(helperFunctions), debug(debug) {}

void Animations::runCurrentAnimation(uint8_t wait)
{
  switch (currentAnimationIndex)
  {
  case 0:
    BasicAnimations::colorWipe(*this, currentColor, wait);
    break;
  case 1:
    BasicAnimations::theaterChase(*this, currentColor, wait);
    break;
  case 2:
    BasicAnimations::flash(*this, currentColor, 5, wait);
    break;
  case 3:
    AdvancedAnimations::rainbowCycle(*this, wait);
    break;
  case 4:
    AdvancedAnimations::theaterChaseRainbow(*this, wait);
    break;
  case 5:
    BasicAnimations::twinkle(*this, currentColor, 10, wait);
    break;
  case 6:
    BasicAnimations::runningLights(*this, currentColor, wait);
    break;
  case 7:
    AdvancedAnimations::meteorRain(*this, currentColor, wait, 3, 4, true);
    break;
  case 8:
    AdvancedAnimations::bouncingBalls(*this, 3, 1, 2);
    break;
  case 9:
    AdvancedAnimations::fireFlicker(*this, currentColor, 10, wait);
    break;
  case 10:
    BasicAnimations::colorChase(*this, currentColor, wait);
    break;
  case 11:
    AdvancedAnimations::randomFadeAnimation(*this, wait);
    break;
  case 12:
    AdvancedAnimations::fireFlicker(*this, wait);
    break;
  case 13:
    BasicAnimations::wave(*this, wait);
    break;
  case 14:
    AdvancedAnimations::meteorShower(*this, currentColor, 3, 4, wait);
    break;
  case 15:
    BasicAnimations::sparkle(*this, currentColor, wait);
    break;
  case 16:
    BasicAnimations::gradientSweep(*this, wait);
    break;
  case 17:
    AdvancedAnimations::fireflies(*this, wait);
    break;
  default:
    BasicAnimations::twinkle(*this, currentColor, 10, wait); // Fallback animation
    break;
  }

  if (debug)
  {
    Serial.print("Finished running animation index: ");
    Serial.println(currentAnimationIndex);
  }
}

void Animations::setRandomColor()
{
  setCurrentColor(helperFunctions.getRandomColor());
}

void Animations::cycleAnimation()
{
  helperFunctions.wipeStrip();
  animationIndex = (animationIndex + 1) % animation_count;
  setAnimation(animationIndex);
}

void Animations::setCurrentColor(uint8_t red, uint8_t green, uint8_t blue)
{
  currentColor = helperFunctions.getColor(red, green, blue);
}

void Animations::setCurrentColor(uint32_t color)
{
  currentColor = color;
}
// void Animations::setAnimationColor(uint32_t color) {
//   currentColor = color;
// };
// Set the animation and reset state
void Animations::setAnimation(uint8_t animationIndex)
{
  if (debug)
  {
    Serial.println("Setting animation index: " + String(animationIndex));
  }

  currentAnimationIndex = animationIndex;
  animationStates[animationIndex] = AnimationState(); // Reset state for the selected animation
}

uint8_t Animations::getTotalAnimations() const
{
  return animation_count;
}
