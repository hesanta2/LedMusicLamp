#ifndef LedLamp_cpp
#define LedLamp_cpp

#include <Arduino.h>
#include <Debug.cpp>
#include <Adafruit_NeoPixel.h>
#include <PaletteProvider.cpp>

class LedLamp
{
private:
    Debug _debug;
    Adafruit_NeoPixel _strip;
    PaletteProvider _paletteProvider;
    int _leds;
    int _lastPercentage = 0;
    int _lastMaxPulse = 0;
    long _frames = 0;

    uint32_t getColor(int led)
    {
        //uint32_t *redPalette = _paletteProvider.getPalette(_leds);
        uint32_t color = _paletteProvider.getPaletteLed(_leds, led);

        return color;
    }

public:
    LedLamp(int leds) : _debug("LedLamp"), _strip(leds, 3, NEO_GRB + NEO_KHZ800), _paletteProvider(_leds)
    {
        _leds = leds;
    };

    void begin()
    {
        _strip.begin();
        _strip.show();
    }

    void setBrightness(int brightness)
    {
        _strip.setBrightness(brightness);
    }

    void setPulse(int percentage)
    {
        int pulse = _leds * percentage / 100;

        if (_frames % 2 != 0 && _lastPercentage >= percentage)
        {
            _lastPercentage--;
            return;
        }

        if (_frames % 12 == 0 && _lastMaxPulse > 0 && _lastMaxPulse > pulse)
        {
            _lastMaxPulse--;
        }

        _strip.clear();
        if (pulse > 0)
        {
            for (int i = 0; i <= pulse; i++)
            {
                _strip.setPixelColor(i, getColor(i));
            }
        }

        if (_lastMaxPulse <= pulse && pulse < _leds)
        {
            _lastMaxPulse = pulse;
        }

        _strip.setPixelColor(_lastMaxPulse, _strip.Color(255, 255, 255));
        _strip.show();

        _lastPercentage = percentage;

        _frames++;
        if (_frames > 65000)
        {
            _frames = 0;
        }
    }
};

#endif