#include "LEDController.h"
LEDController* LEDController::instance = nullptr;  // Initialize to nullptr

LEDController::LEDController(Adafruit_NeoPixel& strip, Animations& animations, AudioProcessor& audioProcessor, HelperFunctions& helperFunctions, boolean debug)
  : strip(strip), animations(animations), audioProcessor(audioProcessor), 
    toneMappings {  
      {200.0, 300.0, [&]() { helperFunctions.wipeStrip(); animations.setAnimation(3); }},
      {300.0, 400.0, [&]() { helperFunctions.wipeStrip(); animations.setAnimation(4); }},
      {400.0, 500.0, [&]() { helperFunctions.wipeStrip(); animations.cycleAnimation(); }},
      {500.0, 600.0, [&]() { animations.setRandomColor(); }},
      {600.0, 700.0, [&]() { helperFunctions.cycleBrightness(); }}
    }, debug(debug), helperFunctions(helperFunctions)
{
  instance = this;  // Set static instance pointer
}

void LEDController::setup() {
  helperFunctions.setupLEDStrip();
  audioProcessor.begin();
  total_animations = animations.getTotalAnimations();

  // Attach the interrupt to a static function
  pinMode(0, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(0), LEDController::onButtonPress, FALLING);
}

void LEDController::loop() {
  handleButtonPress();
  checkForTones();
  animations.runCurrentAnimation(20);
}

void LEDController::checkForTones() {
  double detectedFrequency = audioProcessor.listenForTones();
  for (const ToneRange& tone : toneMappings) {
    if (detectedFrequency >= tone.minFreq && detectedFrequency <= tone.maxFreq) {
      tone.action();
      break;
    }
  }
}

void LEDController::handleButtonPress() {
  if (buttonPressed) {
    buttonPressed = false;
    animations.setRandomColor();
    animations.cycleAnimation();
  }
}

void IRAM_ATTR LEDController::onButtonPress() {
  if (instance != nullptr) {  // Ensure instance exists
    instance->buttonPressed = true;  // Set buttonPressed in the active instance
  }
}