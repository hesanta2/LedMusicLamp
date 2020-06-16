#ifndef LedLamp_cpp
#define LedLamp_cpp

#include <Arduino.h>
#include <Debug.cpp>
#include <Adafruit_NeoPixel.h>
#include <PaletteProvider.cpp>
#include <StopWatch.cpp>

class LedLamp
{
private:
    Debug _debug;
    Adafruit_NeoPixel _strip;
    PaletteProvider _paletteProvider;
    StopWatch _pulseStopWatch;
    StopWatch _maxPulseStopWatch;
    uint32_t _currentPalette[10];

    const unsigned long MAX_PULSE_DECREASE_TIME = 250;

    unsigned int _leds;
    unsigned int _pulse = 0;
    unsigned int _maxPulse = 0;
    unsigned int _lastPulse = 0;
    bool _animatingPulse = false;
    unsigned int _decreasePulseFactor = 0;
    unsigned int _animPulse = 0;
    unsigned int _fromPulse = 0;
    unsigned int _toPulse = 0;

    unsigned long _maxPulseDecreaseTime = 500;

    uint32_t getColor(int led)
    {
        uint32_t color = _currentPalette[led];

        return color;
    }

    void setMaxPulse(unsigned int maxPulse)
    {
        _maxPulse = maxPulse;
        _maxPulseDecreaseTime = MAX_PULSE_DECREASE_TIME;
    }

    void renderPulse(unsigned int pulse)
    {
        for (unsigned int i = 0; i < pulse; i++)
        {
            _strip.setPixelColor(i, getColor(i));
        }
    }

    void renderMaxPulse(unsigned int maxPulse)
    {
        _strip.setPixelColor(maxPulse - 1, _strip.Color(255, 255, 255));
    }

    void updatePulse()
    {
        if (!_animatingPulse && _pulse < _lastPulse)
        {
            _toPulse = _pulse;
            _fromPulse = _lastPulse;
            _animPulse = _fromPulse;
            _pulseStopWatch.reset();
            _animatingPulse = true;
        }

        if (_pulseStopWatch.elapsedTime() > 5 && _animatingPulse)
        {
            /*if (_toPulse > _fromPulse)
            {
                _animPulse++;
            }*/

            if (_toPulse < _fromPulse && _decreasePulseFactor % 5 == 0)
            {
                _animPulse--;
                _decreasePulseFactor = 0;
            }
            _decreasePulseFactor++;

            //_debug.print("_lastPulse: ", String(_lastPulse));
            //_debug.print("_pulse: ", String(_pulse));
            //_debug.print("_fromPulse: ", String(_fromPulse));
            //_debug.print(" _toPulse: ", String(_toPulse));
            //_debug.println(" _animPulse: ", String(_animPulse));

            if (_animPulse == _toPulse)
            {
                _animatingPulse = false;
            }

            _pulseStopWatch.reset();
        }

        if (_animatingPulse)
        {
            _pulse = _animPulse;
        }
    }

    void updateMaxPulse(unsigned int pulse)
    {
        if (_maxPulseStopWatch.elapsedTime() > _maxPulseDecreaseTime && _maxPulse > 1)
        {
            _maxPulseDecreaseTime *= 0.75;
            _maxPulse--;
            //_debug.println("lastMaxPulse: ", String(_maxPulse));
            _maxPulseStopWatch.reset();
        }

        if (_maxPulse <= pulse)
        {
            setMaxPulse(pulse);
        }
    }

public:
    LedLamp(unsigned int leds) : _debug("LedLamp"), _strip(leds, 3, NEO_GRB + NEO_KHZ800), _paletteProvider(leds), _pulseStopWatch(), _maxPulseStopWatch()
    {
        initCarrouselColors();
        _leds = leds;
        PaletteProvider::assignPalette(_paletteProvider.getPaletteBlueRed(), _currentPalette, _leds);
    };

    void begin()
    {
        _strip.begin();
        _strip.show();
    }

    PaletteProvider &getPaletteProvider()
    {
        return _paletteProvider;
    }

    uint32_t *carrouselColors[2];
    void initCarrouselColors()
    {
        carrouselColors[0] = _paletteProvider.getPaletteRedGreen();
        carrouselColors[1] = _paletteProvider.getPaletteBlueRed();
    }

    unsigned int carrouselCount = -1;
    void beginCarrousel()
    {
        if (!_paletteProvider.isInPaletteTransition())
        {
            _paletteProvider.beginTransition();
            carrouselCount++;
            if (carrouselCount > 1)
            {
                carrouselCount = 0;
            }
        }
        setPalette(carrouselColors[carrouselCount]);
    }

    void setBrightness(unsigned int brightness)
    {
        _strip.setBrightness(brightness);
    }

    void setPulse(unsigned int percentage)
    {
        if (percentage > 100)
        {
            percentage = 100;
        }

        if (!_animatingPulse)
        {
            _pulse = _leds * percentage / 100;
            //_debug.println("_pulse: ", String(_pulse));
        }

        updatePulse();
        updateMaxPulse(_pulse);

        _strip.clear();
        renderPulse(_pulse);
        renderMaxPulse(_maxPulse);
        _strip.show();

        _lastPulse = _pulse;
    }

    uint32_t fromTransitionPalette[10];
    void setPalette(uint32_t *palette)
    {
        if (!_paletteProvider.isInPaletteTransition())
        {
            PaletteProvider::assignPalette(_currentPalette, fromTransitionPalette, _leds);
        }
        uint32_t *transitioningPalette = _paletteProvider.setPaletteTransitioned(fromTransitionPalette, palette);

        PaletteProvider::assignPalette(transitioningPalette, _currentPalette, _leds);
    }
};

#endif