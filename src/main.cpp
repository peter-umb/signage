#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "AudioProcessor.h"
#include "Animations.h"

// Define pin and number of LEDs
#define LED_PIN     18     // GPIO 18 for data
#define NUM_LEDS    17     // Number of WS2812B LEDs in the strip
#define BRIGHTNESS  50     // Set brightness (0 to 255)

// Create instances of AudioProcessor and Animations
AudioProcessor audioProcessor;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Animations animations(strip);

struct ToneRange {
  double minFreq;
  double maxFreq;
  std::function<void(uint8_t)> animation;
};

// Initialize toneMappings using lambda functions
ToneRange toneMappings[] = {
  {200.0, 300.0, [&](uint8_t wait) { animations.colorWipe(strip.Color(255, 0, 0), wait); }},
  {300.0, 400.0, [&](uint8_t wait) { animations.rainbowCycle(wait); }},
  {400.0, 500.0, [&](uint8_t wait) { animations.theaterChase(strip.Color(0, 255, 0), wait); }},
  {500.0, 600.0, [&](uint8_t wait) { animations.theaterChaseRainbow(wait); }},
  {600.0, 700.0, [&](uint8_t wait) { animations.flash(strip.Color(255, 255, 0), 5, wait); }}
};

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(BRIGHTNESS);  // Set LED brightness
  strip.show();                     // Initialize all pixels to 'off'
  audioProcessor.begin();           // Initialize audio processing
}

void loop() {
  // Listen for tones and get the detected frequency
  double detectedFrequency = audioProcessor.listenForTones();
  bool toneDetected = false;

  // Find the appropriate animation for the detected frequency
  for (ToneRange &tone : toneMappings) {
    if (detectedFrequency >= tone.minFreq && detectedFrequency <= tone.maxFreq) {
      tone.animation(20);  // Trigger the corresponding animation with a delay of 20 ms
      toneDetected = true;
      break;
    }
  }

  // If no tone was detected, use the twinkle animation as the default
  if (!toneDetected) {
    animations.twinkle(strip.Color(0, 0, 255), 10, 100); // Twinkle with blue color, 10 LEDs, delay 100 ms
  }

  delay(100);  // Adjust delay as needed for responsiveness
}
