#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Arduino.h>

class lightController
{
    public:
        auto adjust(byte*) -> byte;

};

#endif // LIGHT_HPP