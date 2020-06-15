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

    void print(String msg, String msg2)
    {
        String _msg = msg + msg2;
        print(_msg);
    }

    void println(String msg, String msg2)
    {
        String _msg = msg + msg2;
        println(_msg);
    }

    /*void _print(String msg)
    {
        Serial.print(msg);
    }

    void _println(String msg)
    {
        print(msg);
        Serial.println();
    }*/
};

#endif