#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>  // Include vector header
#include "Audio/AudioProcessor.h"
#include "Animations/Animations.h"
#include "Animations/HelperFunctions.h"

class LEDController {
public:
  LEDController(Adafruit_NeoPixel& strip, Animations& animations, AudioProcessor& audioProcessor, HelperFunctions& helperFunctions, boolean debug);

  void setup();
  void loop();

private:
  boolean debug;
  static LEDController* instance;  // Static pointer to the active instance
  static void IRAM_ATTR onButtonPress();  // Interrupt handler
  Adafruit_NeoPixel& strip;
  Animations& animations;
  AudioProcessor& audioProcessor;
  HelperFunctions& helperFunctions;

  // Variables for managing animations and colors
  volatile bool buttonPressed = false;

  // Tone mappings and functions
  struct ToneRange {
    double minFreq;
    double maxFreq;
    std::function<void()> action;
  };

  std::vector<ToneRange> toneMappings;  // Declare toneMappings here

  // Helper functions
  void checkForTones();
  void handleButtonPress();
};

#endif
