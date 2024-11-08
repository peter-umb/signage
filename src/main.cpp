#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "AudioProcessor.h"
#include "Animations.h"
#include "LEDController.h"

// Define pin and number of LEDs
#define LED_PIN     18     // GPIO 18 for data
#define NUM_LEDS    17     // Number of WS2812B LEDs in the strip

// Instantiate core objects
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
AudioProcessor audioProcessor;
Animations animations(strip);

// Instantiate LEDController with dependencies
LEDController ledController(strip, animations, audioProcessor);

void setup() {
  Serial.begin(115200);
  ledController.setup();  // Setup LEDController (includes all LED and audio setup)
}

void loop() {
  ledController.loop();   // Run LEDController loop (includes button check, tone check, and animations)
}
