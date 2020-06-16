#ifndef PaletteProvider_cpp
#define PaletteProvider_cpp

#include <Arduino.h>
#include <Debug.cpp>
#include <StopWatch.cpp>

class PaletteProvider
{
private:
    unsigned int PALETTE_TRANSITION_MILLISECONDS = 20;

    Debug _debug;
    unsigned int _leds;
    uint32_t _red, _green, _blue, _purple, _yellow, _skyblue, _white;
    unsigned int _paletteTransitionPercent = 0;
    //uint32_t _fromPaletteTransition[10];
    uint32_t _toPaletteTransition[10];
    uint32_t _paletteBlueRed[10];
    uint32_t _paletteRedGreen[10];
    StopWatch _paleteTransitionStopWatch;

    static uint32_t getColor(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    static uint8_t getR(uint32_t color) { return (uint8_t)(color >> 16); }
    static uint8_t getG(uint32_t color) { return (uint8_t)(color >> 8); }
    static uint8_t getB(uint32_t color) { return (uint8_t)color; }

    static uint32_t fadeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2, int percent)
    {
        uint8_t _r = r + percent / 100 * (r2 - r);
        uint8_t _g = g + percent / 100 * (g2 - g);
        uint8_t _b = b + percent / 100 * (b2 - b);

        return getColor(_r, _g, _b);
    }

    static uint32_t fadeColor(uint32_t color, uint32_t color2, unsigned int percent)
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
        _red = getColor(255, 0, 0);
        _green = getColor(0, 255, 0);
        _blue = getColor(0, 0, 255);
        _purple = getColor(200, 0, 200);
        _yellow = getColor(200, 200, 0);
        _skyblue = getColor(0, 200, 200);
        _white = getColor(150, 150, 150);
    }

    void initPalettes()
    {
        initPaletteTransition();
        initPaletteBlueRed();
        initPaletteRedGreen();
    }

    void initPaletteTransition()
    {
        _toPaletteTransition[0] = fadeColor(0, 0, 0);
        _toPaletteTransition[1] = fadeColor(0, 0, 0);
        _toPaletteTransition[2] = fadeColor(0, 0, 0);
        _toPaletteTransition[3] = fadeColor(0, 0, 0);
        _toPaletteTransition[4] = fadeColor(0, 0, 0);
        _toPaletteTransition[5] = fadeColor(0, 0, 0);
        _toPaletteTransition[6] = fadeColor(0, 0, 0);
        _toPaletteTransition[7] = fadeColor(0, 0, 0);
        _toPaletteTransition[8] = fadeColor(0, 0, 0);
        _toPaletteTransition[9] = fadeColor(0, 0, 0);

        /*_fromPaletteTransition[0] = fadeColor(0, 0, 0);
        _fromPaletteTransition[1] = fadeColor(0, 0, 0);
        _fromPaletteTransition[2] = fadeColor(0, 0, 0);
        _fromPaletteTransition[3] = fadeColor(0, 0, 0);
        _fromPaletteTransition[4] = fadeColor(0, 0, 0);
        _fromPaletteTransition[5] = fadeColor(0, 0, 0);
        _fromPaletteTransition[6] = fadeColor(0, 0, 0);
        _fromPaletteTransition[7] = fadeColor(0, 0, 0);
        _fromPaletteTransition[8] = fadeColor(0, 0, 0);
        _fromPaletteTransition[9] = fadeColor(0, 0, 0);*/
    }

    void initPaletteBlueRed()
    {
        _paletteBlueRed[0] = fadeColor(_blue, _green, 0);
        _paletteBlueRed[1] = fadeColor(_blue, _green, 33);
        _paletteBlueRed[2] = fadeColor(_blue, _green, 66);
        _paletteBlueRed[3] = fadeColor(_blue, _green, 75);
        _paletteBlueRed[4] = fadeColor(_green, _yellow, 50);
        _paletteBlueRed[5] = fadeColor(_green, _yellow, 100);
        _paletteBlueRed[6] = fadeColor(_yellow, _red, 50);
        _paletteBlueRed[7] = fadeColor(_yellow, _red, 75);
        _paletteBlueRed[8] = fadeColor(_yellow, _red, 100);
        _paletteBlueRed[9] = fadeColor(_yellow, _red, 100);
    }
    void initPaletteRedGreen()
    {
        _paletteRedGreen[0] = fadeColor(_red, _purple, 0);
        _paletteRedGreen[1] = fadeColor(_red, _purple, 15);
        _paletteRedGreen[2] = fadeColor(_red, _purple, 66);
        _paletteRedGreen[3] = fadeColor(_red, _purple, 75);
        _paletteRedGreen[4] = fadeColor(_purple, _blue, 50);
        _paletteRedGreen[5] = fadeColor(_purple, _blue, 100);
        _paletteRedGreen[6] = fadeColor(_blue, _green, 50);
        _paletteRedGreen[7] = fadeColor(_blue, _green, 75);
        _paletteRedGreen[8] = fadeColor(_blue, _green, 100);
        _paletteRedGreen[9] = fadeColor(_blue, _green, 100);
    }

public:
    PaletteProvider(unsigned int leds) : _debug("PaletteProvider"), _paleteTransitionStopWatch()
    {
        _leds = leds;
        initPredefinedColors();
        initPalettes();
    };

    uint32_t *getPaletteBlueRed()
    {
        return _paletteBlueRed;
    }

    uint32_t *getPaletteRedGreen()
    {
        return _paletteRedGreen;
    }

    bool _isInPaletteTransition = false;
    uint32_t *setPaletteTransitioned(uint32_t *fromPalette, uint32_t *toPalette)
    {
        if (_paletteTransitionPercent <= 100)
        {
            _isInPaletteTransition = true;
            if (_paleteTransitionStopWatch.elapsedTime() > PALETTE_TRANSITION_MILLISECONDS)
            {
                if (_paletteTransitionPercent < 100)
                {
                    for (unsigned int i = 0; i < _leds; i++)
                    {
                        _toPaletteTransition[i] = fadeColor(fromPalette[i], toPalette[i], _paletteTransitionPercent);
                    }
                }
                else
                {
                    for (unsigned int i = 0; i < _leds; i++)
                    {
                        _toPaletteTransition[i] = toPalette[i];
                    }
                }

                //_debug.println("_paletteTransitionPercent: ", String(_paletteTransitionPercent));
                //_debug.println("_isInPaletteTransition: ", String(_isInPaletteTransition));
                _paletteTransitionPercent++;
                _paleteTransitionStopWatch.reset();
            }
        }
        else
        {
            _isInPaletteTransition = false;
        }

        return _toPaletteTransition;
    }

    void beginTransition()
    {
        _paletteTransitionPercent = 0;
    }

    bool isInPaletteTransition()
    {
        return _isInPaletteTransition;
    }

    static void assignPalette(uint32_t *palette, uint32_t *paletteDestination, unsigned int leds)
    {
        for (unsigned int i = 0; i < leds; i++)
        {
            paletteDestination[i] = palette[i];
        }
    }
};

#endif