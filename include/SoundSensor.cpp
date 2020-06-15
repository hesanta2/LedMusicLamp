#ifndef SoundSensor_cpp
#define SoundSensor_cpp

#include <Arduino.h>
#include <StopWatch.cpp>

class SoundSensor
{
private:
    unsigned int _pin;
    unsigned int soundValue;
    unsigned int _sampleMilliseconds;
    StopWatch _stopWatch;

public:
    SoundSensor(unsigned int pin, unsigned int sampleMilliseconds) : _stopWatch()
    {
        _pin = pin;
        _sampleMilliseconds = sampleMilliseconds;
    };

    unsigned int readValue()
    {
        if (digitalRead(_pin) == LOW)
        {
            soundValue++;
        }

        if (_stopWatch.elapsedTime() > _sampleMilliseconds)
        {
            //debug.println("soundValue: ", String(soundValue));
            soundValue = 0;

            _stopWatch.reset();
        }

        return soundValue;
    }
};

#endif