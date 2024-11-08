#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "AudioProcessor.h"
#include "Animations.h"

// Define pin and number of LEDs
#define LED_PIN     18     // GPIO 18 for data
#define NUM_LEDS    17     // Number of WS2812B LEDs in the strip

// Brightness levels array for cycling
uint8_t brightnessLevels[] = {64, 128, 192, 255};  // 25%, 50%, 75%, 100%
int brightnessIndex = 0;  // Start at the first brightness level

// Create instances of AudioProcessor and Animations
AudioProcessor audioProcessor;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Animations animations(strip);

// Define variables to track the current color and animation index
uint32_t currentColor = strip.Color(255, 0, 0);  // Initial color (red)
int animationIndex = 0;

volatile bool buttonPressed = false;  // Flag to indicate button press

bool test = true;

// Pointer to the currently selected animation function
std::function<void(uint8_t)> currentAnimation = nullptr;

// ISR for the BOOT button
void IRAM_ATTR handleButtonPress() {
  buttonPressed = true;  // Set flag to true when button is pressed
}

// Function pointers for cycling through animations
std::function<void(uint8_t)> cycleAnimations[] = {
  [&](uint8_t wait) { animations.colorWipe(currentColor, wait); },
  [&](uint8_t wait) { animations.theaterChase(currentColor, wait); },
  [&](uint8_t wait) { animations.flash(currentColor, 5, wait); },
  [&](uint8_t wait) { animations.rainbowCycle(wait); },
  [&](uint8_t wait) { animations.theaterChaseRainbow(wait); },
  [&](uint8_t wait) { animations.twinkle(currentColor, 10, wait); },
  [&](uint8_t wait) { animations.runningLights(currentColor, wait); },
  [&](uint8_t wait) { animations.meteorRain(currentColor, wait, 3, 4, true); },
  [&](uint8_t wait) { animations.bouncingBalls(3, 1, 2, 10); },
  [&](uint8_t wait) { animations.fireFlicker(currentColor, 1, wait); },
  [&](uint8_t wait) { animations.colorChase(currentColor, wait); }
};

// Function to cycle through brightness levels
void cycleBrightness() {
  brightnessIndex = (brightnessIndex + 1) % (sizeof(brightnessLevels) / sizeof(brightnessLevels[0]));
  strip.setBrightness(brightnessLevels[brightnessIndex]);
  strip.show();  // Update brightness immediately
}

// Function to wipe (clear) the LED strip before changing animations
void wipeStrip() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Set each LED to off
  }
  strip.show();  // Apply changes to turn off all LEDs
}

// Function to cycle animations and set the currentAnimation pointer
void cycleAnimation() {
  wipeStrip();  // Clear the strip before changing animation
  currentAnimation = cycleAnimations[animationIndex];
  animationIndex = (animationIndex + 1) % (sizeof(cycleAnimations) / sizeof(cycleAnimations[0]));
}

// Define tone ranges and map specific functions to each
struct ToneRange {
  double minFreq;
  double maxFreq;
  std::function<void(uint8_t)> animation;
};

// Initialize tone mappings
ToneRange toneMappings[] = {
  {200.0, 300.0, [&](uint8_t wait) { wipeStrip(); currentAnimation = [&](uint8_t wait) { animations.rainbowCycle(wait); }; }},
  {300.0, 400.0, [&](uint8_t wait) { wipeStrip(); currentAnimation = [&](uint8_t wait) { animations.theaterChaseRainbow(wait); }; }},
  {400.0, 500.0, [&](uint8_t wait) { wipeStrip(); cycleAnimation(); }},
  {500.0, 600.0, [&](uint8_t wait) { currentColor = strip.Color(random(0, 255), random(0, 255), random(0, 255)); }},
  {600.0, 700.0, [&](uint8_t) { cycleBrightness(); }}
};

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(brightnessLevels[brightnessIndex]);  // Set initial brightness level
  strip.show();                     // Initialize all pixels to 'off'
  audioProcessor.begin();           // Initialize audio processing

  if (test == true) {
    // Configure GPIO 0 (BOOT button) as input with interrupt
    pinMode(0, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(0), handleButtonPress, FALLING);
  }
}

void testButton() {
  if (buttonPressed) {
    buttonPressed = false;  // Reset flag
    cycleAnimation();       // Update animation using the button
  }
}

void checkForTones() {
  // Listen for tones and get the detected frequency
  double detectedFrequency = audioProcessor.listenForTones();
  bool toneDetected = false;

  // Find the appropriate animation for the detected frequency
  for (ToneRange &tone : toneMappings) {
    if (detectedFrequency >= tone.minFreq && detectedFrequency <= tone.maxFreq) {
      tone.animation(20);  // Set the corresponding animation
      toneDetected = true;
      break;
    }
  }

  // Optionally, handle a long pause with no tone to set a fallback animation
  if (!toneDetected && currentAnimation == nullptr) {
    currentAnimation = [&](uint8_t wait) { animations.twinkle(strip.Color(160, 32, 240), 10, 100); };
  }
}

void loop() {
  testButton();
  checkForTones();

  // Continuously run the current animation
  if (currentAnimation != nullptr) {
    currentAnimation(20);
  }

  delay(100);  // Adjust delay as needed for responsiveness
}
