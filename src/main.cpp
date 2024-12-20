#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Audio/AudioProcessor.h"
#include "Animations/Animations.h"
#include "LEDController.h"
#include "Animations/HelperFunctions.h"
#include "Config.h"

// Enable for debug serial output
boolean debug = true;

// Instantiate core objects
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
AudioProcessor audioProcessor(debug);
HelperFunctions helperFunctions(strip, debug);
Animations animations(strip, helperFunctions, debug);

// Instantiate LEDController with dependencies
LEDController ledController(strip, animations, audioProcessor, helperFunctions, debug);

void setup() {
  if (debug) {
    Serial.begin(115200);
  }

  ledController.setup();  // Setup LEDController (includes all LED and audio setup)
}

void loop() {
  ledController.loop();   // Run LEDController loop (includes button check, tone check, and animations)
}
