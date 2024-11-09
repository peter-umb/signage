#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <Adafruit_NeoPixel.h>

class HelperFunctions {
public:
    HelperFunctions(Adafruit_NeoPixel& ledStrip, boolean debug);

    int min(int a, uint32_t b);

    void wipeStrip();
    void cycleBrightness();
    void setPixelColorWithFade(int pixel, uint32_t color, uint8_t fadeAmount);
    void setAllPixels(uint32_t color);

    uint32_t dimColor(uint32_t color, uint8_t factor);
    uint32_t Wheel(byte WheelPos);
    uint32_t getColor(uint8_t red, uint8_t green, uint8_t blue);
    uint32_t getRandomColor();
    void cycleBrightness(int brightnessIndex);
    void setupLEDStrip();


private:
    boolean debug;
    Adafruit_NeoPixel& strip;
    
    uint8_t brightnessLevels[4] = {64, 128, 192, 255};
    int brightnessIndex = 1;
};

#endif // HELPERFUNCTIONS_H