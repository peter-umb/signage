#include "LEDController.h"
LEDController* LEDController::instance = nullptr;  // Initialize to nullptr

LEDController::LEDController(Adafruit_NeoPixel& strip, Animations& animations, AudioProcessor& audioProcessor)
  : strip(strip), animations(animations), audioProcessor(audioProcessor), currentColor(strip.Color(255, 0, 255)), 
    toneMappings {  
      {200.0, 300.0, [&]() { wipeStrip(); animations.setAnimation(3); }},
      {300.0, 400.0, [&]() { wipeStrip(); animations.setAnimation(4); }},
      {400.0, 500.0, [&]() { wipeStrip(); cycleAnimation(); }},
      {500.0, 600.0, [&]() { setRandomColor(); }},
      {600.0, 700.0, [&]() { cycleBrightness(); }}
    } 
{
  instance = this;  // Set static instance pointer
}

void LEDController::setup() {
  strip.begin();
  strip.setBrightness(brightnessLevels[brightnessIndex]);
  strip.show();
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

void LEDController::cycleBrightness() {
  brightnessIndex = (brightnessIndex + 1) % 4;
  strip.setBrightness(brightnessLevels[brightnessIndex]);
  strip.show();
}

void LEDController::setCurrentColor(uint8_t red, uint8_t green, uint8_t blue) {
  currentColor = strip.Color(red, green, blue);
  animations.setAnimationColor(currentColor);
}

void LEDController::setCurrentColor(uint32_t color) {
  currentColor = color;
  animations.setAnimationColor(currentColor);
}

void LEDController::setRandomColor() {
  setCurrentColor(animations.Wheel(random(0, 255)));
}

void LEDController::cycleAnimation() {
  wipeStrip();
  animations.setAnimation(animationIndex);
  animationIndex = (animationIndex + 1) % total_animations;
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
    setRandomColor();
    cycleAnimation();
  }
}

void LEDController::wipeStrip() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

void IRAM_ATTR LEDController::onButtonPress() {
  if (instance != nullptr) {  // Ensure instance exists
    instance->buttonPressed = true;  // Set buttonPressed in the active instance
  }
}