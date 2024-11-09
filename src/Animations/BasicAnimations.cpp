#include "BasicAnimations.h"
#include "Animations.h" // Include the full definition of Animations here

namespace BasicAnimations
{
  void colorWipe(Animations &animations, uint32_t color, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;

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

  void theaterChase(Animations &animations, uint32_t color, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;

    if (state.cycle < 10)
    {
      for (int q = 0; q < 3; q++)
      {
        for (int i = 0; i < strip.numPixels(); i += 3)
        {
          strip.setPixelColor(i + q, state.step % 3 == q ? color : 0);
        }
      }
      strip.show();
      delay(wait);
      state.step++;
      if (state.step % 3 == 0)
        state.cycle++;
    }
    else
    {
      state.cycle = 0;
      state.step = 0;
    }
  }

  void flash(Animations &animations, uint32_t color, uint8_t flashes, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;
    HelperFunctions &helperFunctions = animations.helperFunctions;

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

  // Twinkle Animation
  void twinkle(Animations &animations, uint32_t color, uint8_t count, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;
    HelperFunctions &helperFunctions = animations.helperFunctions;
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

  void runningLights(Animations &animations, uint32_t color, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;
    HelperFunctions &helperFunctions = animations.helperFunctions;
    if (state.step < 256 * 2)
    {
      for (int i = 0; i < strip.numPixels(); i++)
      {
        strip.setPixelColor(i, ((sin(i + state.step) * 127 + 128) / 255) * color);
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

  void colorChase(Animations &animations, uint32_t color, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;
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
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;
    HelperFunctions &helperFunctions = animations.helperFunctions;
    static uint16_t waveOffset = 0;

    for (int i = 0; i < strip.numPixels(); i++)
    {
      uint8_t brightness = (sin((i + waveOffset) * 0.2) * 127) + 128;   // Adjust wave speed with multiplier
      strip.setPixelColor(i, strip.Color(brightness, brightness, 255)); // Adjust color as needed
    }

    waveOffset += 1; // Control wave speed
    strip.show();
    delay(wait);
  }

  void sparkle(Animations &animations, uint32_t color, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    Animations::AnimationState &state = animations.state;
    HelperFunctions &helperFunctions = animations.helperFunctions;
    for (int i = 0; i < 3; i++)
    {
      int pixel = random(0, strip.numPixels());
      strip.setPixelColor(pixel, color); // Set random LED to color
      strip.show();
      delay(wait);
      strip.setPixelColor(pixel, 0); // Turn it off
    }
    strip.show();
  }

  void gradientSweep(Animations &animations, uint8_t wait)
  {
    Adafruit_NeoPixel &strip = animations.strip;
    static uint16_t hue = 0;
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, strip.ColorHSV((hue + (i * 65536L / strip.numPixels())) % 65536, 255, 255));
    }
    hue += 256; // Change this for speed of color change
    strip.show();
    delay(wait);
  }
}
