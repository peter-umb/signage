#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>  // Include vector header
#include "AudioProcessor.h"
#include "Animations.h"

class LEDController {
public:
  LEDController(Adafruit_NeoPixel& strip, Animations& animations, AudioProcessor& audioProcessor);

  void setup();
  void loop();

private:
  int total_animations;
  static LEDController* instance;  // Static pointer to the active instance
  static void IRAM_ATTR onButtonPress();  // Interrupt handler
  Adafruit_NeoPixel& strip;
  Animations& animations;
  AudioProcessor& audioProcessor;

  // Variables for managing animations and colors
  uint8_t brightnessLevels[4] = {64, 128, 192, 255};
  int brightnessIndex = 1;
  int animationIndex = 0;
  uint32_t currentColor;
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
  void setCurrentColor(uint8_t red, uint8_t green, uint8_t blue);
  void setCurrentColor(uint32_t color);
  void setRandomColor();
  void cycleAnimation();
  void checkForTones();
  void handleButtonPress();
  void wipeStrip();
};

#endif
