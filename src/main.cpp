#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "AudioProcessor.h"
#include "Animations.h"

// Define pin and number of LEDs
#define LED_PIN     18     // GPIO 18 for data
#define NUM_LEDS    17     // Number of WS2812B LEDs in the strip

// Brightness levels array for cycling
uint8_t brightnessLevels[] = {64, 128, 192, 255};  // 25%, 50%, 75%, 100%
int brightnessIndex = 1;  // Start at the first brightness level

// Create instances of AudioProcessor and Animations
AudioProcessor audioProcessor;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Animations animations(strip);

// Define variables to track the current color and animation index
uint32_t currentColor = strip.Color(255, 0, 255);
int animationIndex = 0;

volatile bool buttonPressed = false;  // Flag to indicate button press

bool test = true;

// ISR for the BOOT button
void IRAM_ATTR handleButtonPress() {
  buttonPressed = true;  // Set flag to true when button is pressed
}

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

// Function to cycle animations and set the animation in Animations class
void cycleAnimation() {
  wipeStrip();  // Clear the strip before changing animation

  // Set new animation based on the current animationIndex
  Serial.print("cycleAnimation - Current animationIndex: ");
  Serial.println(animationIndex);

  animations.setAnimation(animationIndex);

  // Increment and wrap animationIndex for next cycle
  animationIndex = (animationIndex + 1) % 11;  // Assuming 11 animations
  Serial.print("cycleAnimation - Next animationIndex: ");
  Serial.println(animationIndex);
}

// Define tone ranges and map specific functions to each
struct ToneRange {
  double minFreq;
  double maxFreq;
  std::function<void()> action;
};

void setCurrentColor(uint8_t _r, uint8_t _g, uint8_t _b) {
  currentColor = strip.Color(_r, _g, _b);
  animations.setAnimationColor(currentColor);
}

void setRandomColor() {
  setCurrentColor(random(0, 255), random(0, 255), random(0, 255));
}

// Initialize tone mappings
ToneRange toneMappings[] = {
  {200.0, 300.0, [&]() { wipeStrip(); animations.setAnimation(3); }},            // RainbowCycle animation
  {300.0, 400.0, [&]() { wipeStrip(); animations.setAnimation(4); }},            // TheaterChaseRainbow animation
  {400.0, 500.0, [&]() { wipeStrip(); cycleAnimation(); }},                      // Cycle through animations
  {500.0, 600.0, [&]() { setRandomColor(); }},  // Change color
  {600.0, 700.0, [&]() { cycleBrightness(); }}                                  // Adjust brightness level
};

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(brightnessLevels[brightnessIndex]);  // Set initial brightness level
  strip.show();                     // Initialize all pixels to 'off'
  Serial.println("Setup complete. LEDs initialized.");  // Debug: Confirm setup

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
    setCurrentColor(random(0, 255), random(0, 255), random(0, 255));
    cycleAnimation();       // Update animation using the button
  }
}

void checkForTones() {
  // Listen for tones and get the detected frequency
  double detectedFrequency = audioProcessor.listenForTones();
  bool toneDetected = false;

  // Find the appropriate action for the detected frequency
  for (ToneRange &tone : toneMappings) {
    if (detectedFrequency >= tone.minFreq && detectedFrequency <= tone.maxFreq) {
      tone.action();  // Execute the action (e.g., change animation)
      toneDetected = true;
      break;
    }
  }

  // Fallback animation if no tone is detected
  // if (!toneDetected) {
  //   animations.setAnimation(5);  // Default to twinkle animation
  // }
}

void loop() {
  testButton();          // Check for button press to change animation
  checkForTones();       // Check for tone to adjust animation

  // Continuously run the current animation in incremental steps
  animations.runCurrentAnimation(20);

  delay(150);  // Adjust delay as needed for responsiveness
}
