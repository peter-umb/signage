#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>  // Include vector header
#include "AudioProcessor.h"
#include "Animations.h"
#include "HelperFunctions.h"

class LEDController {
public:
  LEDController(Adafruit_NeoPixel& strip, Animations& animations, AudioProcessor& audioProcessor, HelperFunctions& helperFunctions, boolean debug);

  void setup();
  void loop();

private:
  boolean debug;
  int total_animations;
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
  void cycleBrightness();
  // void setCurrentColor(uint8_t red, uint8_t green, uint8_t blue);
  // void setCurrentColor(uint32_t color);
  void cycleAnimation();
  void checkForTones();
  void handleButtonPress();
};

#endif
