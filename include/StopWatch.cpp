#ifndef StopWatch_cpp
#define StopWatch_cpp

#include <Arduino.h>

class StopWatch
{
private:
    unsigned long millisCurrent;
    unsigned long millisLast = 0;
    unsigned long millisElapsed = 0;

public:
    StopWatch(){};

    unsigned long elapsedTime()
    {
        millisCurrent = millis();
        millisElapsed = millisCurrent - millisLast;

        return millisElapsed;
    }

    void reset()
    {
        millisLast = millisCurrent;
    }
};

#endif