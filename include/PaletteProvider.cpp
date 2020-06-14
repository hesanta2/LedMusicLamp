#ifndef PaletteProvider_cpp
#define PaletteProvider_cpp

#include <Arduino.h>
#include <Debug.cpp>

class PaletteProvider
{
private:
    Debug _debug;
    int _leds;
    uint32_t red, green, blue, purple, yellow, skyblue, white;

    uint32_t getColor(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    /*uint8_t getBits(uint32_t number, uint8_t position, uint8_t count)
    {
        return MID(number, position, count);
    }*/
    uint8_t getR(uint32_t color) { return (uint8_t)(color >> 16); }
    uint8_t getG(uint32_t color) { return (uint8_t)(color >> 8); }
    uint8_t getB(uint32_t color) { return (uint8_t)color; }

    uint32_t fadeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2, int percent)
    {
        uint8_t _r = r + percent / 100 * (r2 - r);
        uint8_t _g = g + percent / 100 * (g2 - g);
        uint8_t _b = b + percent / 100 * (b2 - b);

        return getColor(_r, _g, _b);
    }

    uint32_t fadeColor(uint32_t color, uint32_t color2, int percent)
    {
        uint8_t r = getR(color);
        uint8_t g = getG(color);
        uint8_t b = getB(color);

        uint8_t r2 = getR(color2);
        uint8_t g2 = getG(color2);
        uint8_t b2 = getB(color2);

        uint8_t _r = round(r + (r2 - r) * percent / 100);
        uint8_t _g = round(g + (g2 - g) * percent / 100);
        uint8_t _b = round(b + (b2 - b) * percent / 100);

        return getColor(_r, _g, _b);
    }

    void initPredefinedColors()
    {
        red = getColor(255, 0, 0);
        green = getColor(0, 255, 0);
        blue = getColor(0, 0, 255);
        purple = getColor(200, 0, 200);
        yellow = getColor(200, 200, 0);
        skyblue = getColor(0, 200, 200);
        white = getColor(150, 150, 150);
    }

public:
    PaletteProvider(int leds) : _debug("PaletteProvider")
    {
        _leds = leds;
        initPredefinedColors();
    };

    uint32_t *getPalette(uint8_t leds)
    {
        uint32_t palette[leds];
        palette[0] = fadeColor(blue, green, 0);
        palette[1] = fadeColor(blue, green, 33);
        palette[2] = fadeColor(blue, green, 66);
        palette[3] = fadeColor(blue, green, 75);
        palette[4] = fadeColor(green, yellow, 50);
        palette[5] = fadeColor(green, yellow, 100);
        palette[6] = fadeColor(yellow, red, 50);
        palette[7] = fadeColor(yellow, red, 75);
        palette[8] = fadeColor(yellow, red, 100);
        palette[9] = fadeColor(yellow, red, 100);

        return palette;
    }

    uint32_t getPaletteLed(uint8_t leds, int led)
    {
        uint32_t palette[leds];
        palette[0] = fadeColor(blue, green, 0);
        palette[1] = fadeColor(blue, green, 33);
        palette[2] = fadeColor(blue, green, 66);
        palette[3] = fadeColor(blue, green, 75);
        palette[4] = fadeColor(green, yellow, 50);
        palette[5] = fadeColor(green, yellow, 100);
        palette[6] = fadeColor(yellow, red, 50);
        palette[7] = fadeColor(yellow, red, 75);
        palette[8] = fadeColor(yellow, red, 100);
        palette[9] = fadeColor(yellow, red, 100);

        return palette[led];
    }
};

#endif