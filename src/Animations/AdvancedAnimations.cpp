#include "AdvancedAnimations.h"
#include "Animations.h" // Include the full definition of Animations here

namespace AdvancedAnimations
{
    // Non-blocking rainbowCycle
    void rainbowCycle(Animations& animations, uint8_t wait)
    {
        auto &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        auto &helperFunctions = animations.helperFunctions;

        for (uint16_t i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, helperFunctions.Wheel((i * 256 / strip.numPixels() + state.step) % 256));
        }
        strip.show();

        // Update timing for non-blocking delay
        if (millis() - state.lastUpdate > wait) {
            state.step++;
            state.lastUpdate = millis();
        }
    }

    void theaterChaseRainbow(Animations& animations, uint8_t wait)
    {
        auto &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        auto &helperFunctions = animations.helperFunctions;

        if (state.step < 256)
        {
            for (int q = 0; q < 3; q++)
            {
                for (int i = 0; i < strip.numPixels(); i += 3)
                {
                    strip.setPixelColor(i + q, helperFunctions.Wheel((i + state.step) % 255));
                }
            }
            strip.show();

            if (millis() - state.lastUpdate > wait) {
                state.step = (state.step + 1) % 256;
                state.lastUpdate = millis();
            }
        }
        else
        {
            state.step = 0;
        }
    }

    void meteorRain(Animations& animations, uint8_t wait)
    {
        auto &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        auto &helperFunctions = animations.helperFunctions;
        uint32_t &color = animations.currentColor;

        const uint8_t meteorSize = 3;
        const uint8_t meteorTrailDecay = 4;
        const bool randomDecay = true;

        for (int j = 0; j < strip.numPixels(); j++)
        {
            if (!randomDecay || random(10) > 5)
            {
                uint32_t pixelColor = strip.getPixelColor(j);
                helperFunctions.setPixelColorWithFade(j, pixelColor, meteorTrailDecay);
            }
        }

        for (int j = 0; j < meteorSize; j++)
        {
            int pixelPos = state.step - j;
            if (pixelPos >= 0 && pixelPos < strip.numPixels())
            {
                strip.setPixelColor(pixelPos, color);
            }
        }
        strip.show();

        if (millis() - state.lastUpdate > wait) {
            state.step = (state.step + 1) % (strip.numPixels() * 2);
            state.lastUpdate = millis();
        }
    }

    void bouncingBalls(Animations& animations, uint8_t wait)
    {
        auto &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        const uint8_t ballCount = 5;
        const float gravity = 0.4;
        const float dampening = 0.6;
        static float height[10];
        static float impactVelocity[10];
        static bool initialized = false;

        if (!initialized)
        {
            float impactVelocityStart = sqrt(2.0 * abs(gravity));
            for (int i = 0; i < ballCount; i++)
            {
                height[i] = 16.0;
                impactVelocity[i] = impactVelocityStart;
            }
            initialized = true;
        }

        for (int i = 0; i < ballCount; i++)
        {
            height[i] += impactVelocity[i];
            impactVelocity[i] += gravity;

            if (height[i] < 0)
            {
                height[i] = 0;
                impactVelocity[i] *= -dampening;
            }

            strip.setPixelColor(i, strip.Color(255, 0, 0)); // Customize color as needed
        }
        strip.show();
    }

    void randomFadeAnimation(Animations& animations, uint8_t wait)
    {
        auto &strip = animations.strip;
        auto &helperFunctions = animations.helperFunctions;
        struct Color
        {
            uint32_t color;
            uint8_t ledStart;
            uint8_t ledCount;
            uint16_t fadeDuration;
            uint16_t holdDuration;
            int fadeDirection;
            float brightness;
        };

        static std::vector<Color> activeColors;
        const int maxColors = 5;

        if (random(1, 5) == 1 || activeColors.size() < maxColors)
        {
            Color newColor;
            newColor.color = strip.Color(random(0, 256), random(0, 256), random(0, 256));
            newColor.ledStart = random(0, strip.numPixels());
            newColor.ledCount = random(1, 5);
            newColor.fadeDuration = random(500, 3000);
            newColor.holdDuration = random(500, 3000);
            newColor.fadeDirection = 1;
            newColor.brightness = 0.0;
            activeColors.push_back(newColor);
        }

        for (auto it = activeColors.begin(); it != activeColors.end();)
        {
            Color &color = *it;
            uint32_t blendedColor = strip.Color(
                static_cast<uint8_t>((color.color >> 16 & 0xFF) * color.brightness),
                static_cast<uint8_t>((color.color >> 8 & 0xFF) * color.brightness),
                static_cast<uint8_t>((color.color & 0xFF) * color.brightness));

            for (uint8_t i = color.ledStart; i < color.ledStart + color.ledCount && i < strip.numPixels(); i++)
            {
                uint32_t existingColor = strip.getPixelColor(i);
                uint8_t r = helperFunctions.min(255, (existingColor >> 16 & 0xFF) + (blendedColor >> 16 & 0xFF));
                uint8_t g = helperFunctions.min(255, (existingColor >> 8 & 0xFF) + (blendedColor >> 8 & 0xFF));
                uint8_t b = helperFunctions.min(255, (existingColor & 0xFF) + (blendedColor & 0xFF));
                strip.setPixelColor(i, strip.Color(r, g, b));
            }

            float fadeStep = (float)wait / color.fadeDuration;
            color.brightness += fadeStep * color.fadeDirection;

            if (color.brightness >= 1.0)
            {
                color.brightness = 1.0;
                color.fadeDirection = -1;
                color.holdDuration -= wait;
            }
            else if (color.brightness <= 0.0)
            {
                color.brightness = 0.0;
                it = activeColors.erase(it);
                continue;
            }
            else if (color.fadeDirection == -1 && color.holdDuration <= 0)
            {
                color.fadeDirection = -1;
            }

            ++it;
        }

        strip.show();
    }
    
    void fireFlicker(Animations& animations, uint8_t wait)
    {
        Adafruit_NeoPixel &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        HelperFunctions &helperFunctions = animations.helperFunctions;
        uint32_t &baseColor = animations.currentColor;
        uint8_t flickerIntensity = 4;
        const int numLeds = animations.strip.numPixels();
     
        for (int i = 0; i < numLeds; i++)
        {
            int flicker = random(-flickerIntensity, flickerIntensity);
            uint8_t r = constrain((baseColor >> 16 & 0xFF) + flicker, 0, 255);
            uint8_t g = constrain((baseColor >> 8 & 0xFF) + flicker, 0, 255);
            uint8_t b = constrain((baseColor & 0xFF) + flicker, 0, 255);
            strip.setPixelColor(i, strip.Color(r, g, b));
        }
        strip.show();
        delay(wait);
    }

    void fireFlicker2(Animations& animations, uint8_t wait)
    {
        Adafruit_NeoPixel &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        HelperFunctions &helperFunctions = animations.helperFunctions;
        // Define base colors for fire effect
        const uint8_t baseRed = 255;
        const uint8_t baseGreen = 100;
        const uint8_t baseBlue = 0;

        const int numLeds = animations.strip.numPixels();

        // Iterate over each LED and apply random flicker
        for (int i = 0; i < numLeds; i++)
        {
            // Generate a random flicker intensity
            int flicker = random(-50, 50); // Random value to simulate flicker

            // Calculate color values with flicker
            uint8_t red = constrain(baseRed + flicker, 0, 255);
            uint8_t green = constrain(baseGreen + flicker / 2, 0, 255); // Green is affected less
            uint8_t blue = baseBlue;                                    // Blue stays constant for a warm fire

            // Set the flickering color to the current LED
            strip.setPixelColor(i, strip.Color(red, green, blue));
        }

        strip.show();
        delay(wait); // Control flicker speed
    }

    void meteorShower(Animations& animations, uint8_t wait)
    {
        Adafruit_NeoPixel &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        HelperFunctions &helperFunctions = animations.helperFunctions;
        uint32_t &color = animations.currentColor;

        uint8_t meteorSize = 3;
        uint8_t trailDecay = 5;
    
        const int numLeds = animations.strip.numPixels();
      
        for (int j = numLeds + meteorSize; j >= 0; j--)
        {
            // Fade all LEDs
            for (int k = 0; k < numLeds; k++)
            {
                uint32_t currentColor = strip.getPixelColor(k);
                strip.setPixelColor(k, helperFunctions.dimColor(currentColor, trailDecay));
            }

            // Draw meteor
            for (int k = 0; k < meteorSize; k++)
            {
                if ((j - k >= 0) && (j - k < numLeds))
                {
                    strip.setPixelColor(j - k, color);
                }
            }

            strip.show();
            delay(wait);
        }
    }

    void fireflies(Animations& animations, uint8_t wait)
    {
        Adafruit_NeoPixel &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        HelperFunctions &helperFunctions = animations.helperFunctions;
        struct Color
        {
            uint32_t color;
            uint8_t led;
            float brightness;
            int fadeDirection; // 1 for fade-in, -1 for fade-out
        };

        static std::vector<Color> activeFireflies;

        // Randomly add a new firefly if under the maximum
        if (activeFireflies.size() < 10 && random(1, 10) == 1)
        { // Adjust probability as needed
            Color newFirefly;
            newFirefly.color = strip.Color(random(100, 255), random(100, 255), random(0, 50)); // Warm yellowish colors
            newFirefly.led = random(0, animations.strip.numPixels());
            newFirefly.brightness = 0.0;
            newFirefly.fadeDirection = 1;
            activeFireflies.push_back(newFirefly);
        }

        // Process each firefly
        for (auto it = activeFireflies.begin(); it != activeFireflies.end();)
        {
            Color &firefly = *it;

            uint32_t existingColor = strip.getPixelColor(firefly.led);

            // Blend current brightness level with existing color on the LED
            uint8_t r = std::max(0, (int)((existingColor >> 16 & 0xFF) + ((firefly.color >> 16 & 0xFF) * firefly.brightness)));
            uint8_t g = std::max(0, (int)((existingColor >> 8 & 0xFF) + ((firefly.color >> 8 & 0xFF) * firefly.brightness)));
            uint8_t b = std::max(0, (int)((existingColor & 0xFF) + ((firefly.color & 0xFF) * firefly.brightness)));

            strip.setPixelColor(firefly.led, strip.Color(r, g, b));

            // Adjust brightness for fade in or out
            firefly.brightness += 0.05 * firefly.fadeDirection;

            // Check if the firefly should start fading out or be removed
            if (firefly.brightness >= 1.0)
            {
                firefly.brightness = 1.0;
                firefly.fadeDirection = -1;
            }
            else if (firefly.brightness <= 0.0)
            {
                firefly.brightness = 0.0;
                it = activeFireflies.erase(it); // Remove fully faded-out firefly
                continue;
            }

            ++it;
        }

        strip.show();
        delay(wait);
    }

    void colorWaves(Animations& animations, uint8_t wait)
    {
        Adafruit_NeoPixel &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        HelperFunctions &helperFunctions = animations.helperFunctions;
        static float position = 0.0;
        float speed = 0.1; // Adjust for faster or slower waves
        position += speed;

        const int numLeds = animations.strip.numPixels();

        for (uint16_t i = 0; i < numLeds; i++)
        {
            float wavePosition = (position + (float)i / numLeds * 10.0); // Spread out the waves
            uint8_t red = (sin(wavePosition) + 1.0) * 127.5;
            uint8_t green = (sin(wavePosition + 2.0) + 1.0) * 127.5;
            uint8_t blue = (sin(wavePosition + 4.0) + 1.0) * 127.5;
            strip.setPixelColor(i, strip.Color(red, green, blue));
        }

        strip.show();
        delay(wait);
    }

    void randomSparkles(Animations& animations, uint8_t wait)
    {
        Adafruit_NeoPixel &strip = animations.strip;
        auto &state = animations.animationStates[animations.currentAnimationIndex];
        HelperFunctions &helperFunctions = animations.helperFunctions;
        struct Sparkle
        {
            uint8_t ledIndex;
            float brightness;
            int fadeDirection; // 1 for fade-in, -1 for fade-out
        };

        static std::vector<Sparkle> activeSparkles;
        const int numLeds = animations.strip.numPixels();

        // Randomly add a new sparkle if under the maximum
        if (activeSparkles.size() < 10 && random(1, 20) == 1)
        { // Adjust probability as needed
            Sparkle newSparkle;
            newSparkle.ledIndex = random(0, numLeds);
            newSparkle.brightness = 0.0;
            newSparkle.fadeDirection = 1;
            activeSparkles.push_back(newSparkle);
        }

        // Process each sparkle
        for (auto it = activeSparkles.begin(); it != activeSparkles.end();)
        {
            Sparkle &sparkle = *it;

            // Set brightness of sparkle color (white)
            uint8_t brightnessValue = (uint8_t)(sparkle.brightness * 255);
            strip.setPixelColor(sparkle.ledIndex, strip.Color(brightnessValue, brightnessValue, brightnessValue));

            // Adjust brightness for fade in or out
            sparkle.brightness += 0.05 * sparkle.fadeDirection;

            // Check if the sparkle should start fading out or be removed
            if (sparkle.brightness >= 1.0)
            {
                sparkle.brightness = 1.0;
                sparkle.fadeDirection = -1;
            }
            else if (sparkle.brightness <= 0.0)
            {
                sparkle.brightness = 0.0;
                it = activeSparkles.erase(it); // Remove fully faded-out sparkle
                continue;
            }

            ++it;
        }

        strip.show();
        delay(wait);
    }
}
