#ifndef Debug_cpp
#define Debug_cpp

#include <Arduino.h>

class Debug
{
private:
    String _name;

public:
    Debug(String name)
    {
        _name = name;
    };

    void print(String msg)
    {
        Serial.print("# " + _name + ": " + msg);
    }

    void println(String msg)
    {
        print(msg);
        Serial.println();
    }
};

#endif