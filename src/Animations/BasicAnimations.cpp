#include "BasicAnimations.h"
#include "Animations.h" // Include the full definition of Animations here

namespace BasicAnimations
{
  void colorWipe(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    auto color = animations.currentColor;

    if (state.pos < strip.numPixels())
    {
      strip.setPixelColor(state.pos, color);
      strip.show();
      delay(wait);
      state.pos++;
    }
    else
    {
      state.pos = 0; // Reset to allow cycling
    }
  }

  void theaterChase(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    auto color = animations.currentColor;
    int numPixels = strip.numPixels();

    if (state.cycle < 10)
    {
      for (int q = 0; q < 3; q++)
      {
        for (int i = 0; i < numPixels; i += 3)
        {
          strip.setPixelColor(i + q, state.step % 3 == q ? color : 0);
        }
      }
      strip.show();
      delay(wait);
      state.step++;
      if (state.step % 3 == 0) state.cycle++;
    }
    else
    {
      state.cycle = 0;
      state.step = 0;
    }
  }

  void flash(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    auto &helperFunctions = animations.helperFunctions;
    auto color = animations.currentColor;
    const uint8_t flashes = 3;

    if (state.cycle < flashes)
    {
      helperFunctions.setAllPixels(state.cycle % 2 == 0 ? color : 0);
      strip.show();
      delay(wait);
      state.cycle++;
    }
    else
    {
      state.cycle = 0;
    }
  }

  void twinkle(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    auto color = animations.currentColor;
    const uint8_t count = 5;

    if (state.step < count)
    {
      int pixel = random(strip.numPixels());
      strip.setPixelColor(pixel, color);
      strip.show();
      delay(wait);
      strip.setPixelColor(pixel, 0);
      state.step++;
    }
    else
    {
      state.step = 0; // Reset to allow cycling
    }
  }

  void runningLights(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    auto color = animations.currentColor;
    int numPixels = strip.numPixels();

    if (state.step < 256 * 2)
    {
      for (int i = 0; i < numPixels; i++)
      {
        uint8_t brightness = (sin((i + state.step) * 0.1) * 127 + 128); // Adjusted speed for smoother effect
        strip.setPixelColor(i, strip.Color((color >> 16) * brightness / 255, 
                                           (color >> 8 & 0xFF) * brightness / 255, 
                                           (color & 0xFF) * brightness / 255));
      }
      strip.show();
      delay(wait);
      state.step++;
    }
    else
    {
      state.step = 0; // Reset for the next cycle
    }
  }

  void colorChase(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    auto color = animations.currentColor;

    if (state.pos < strip.numPixels())
    {
      strip.setPixelColor(state.pos, color);
      strip.show();
      delay(wait);
      strip.setPixelColor(state.pos, 0); // Turn off the current pixel before moving to the next
      state.pos++;
    }
    else
    {
      state.pos = 0; // Reset to allow cycling
    }
  }

  void wave(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    int numPixels = strip.numPixels();

    for (int i = 0; i < numPixels; i++)
    {
      uint8_t brightness = (sin((i + state.step) * 0.2) * 127) + 128;
      strip.setPixelColor(i, strip.Color(brightness, brightness, 255)); // Adjust color as needed
    }

    state.step += 1;
    strip.show();
    delay(wait);
  }

  void sparkle(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto color = animations.currentColor;
    int numPixels = strip.numPixels();

    for (int i = 0; i < 3; i++)
    {
      int pixel = random(numPixels);
      strip.setPixelColor(pixel, color); // Set random LED to color
      strip.show();
      delay(wait);
      strip.setPixelColor(pixel, 0); // Turn it off
    }
    strip.show();
  }

  void gradientSweep(Animations &animations, uint8_t wait)
  {
    auto &strip = animations.strip;
    auto &state = animations.animationStates[animations.currentAnimationIndex];
    int numPixels = strip.numPixels();

    for (int i = 0; i < numPixels; i++)
    {
      strip.setPixelColor(i, strip.ColorHSV((state.step + (i * 65536L / numPixels)) % 65536, 255, 255));
    }
    state.step += 256; // Change this for speed of color change
    strip.show();
    delay(wait);
  }

  void solidColor(Animations& animations, uint8_t wait)
    {
        auto &strip = animations.strip;
        uint32_t color = animations.currentColor;

        for (int i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, color); // Set each LED to the current color
        }
        strip.show();

        // Delay to simulate wait time if needed
        delay(wait);
    }
}
