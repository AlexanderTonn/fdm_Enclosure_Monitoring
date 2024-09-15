#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Arduino.h>

class lightController
{
    public:
        auto adjust(uint8_t) -> byte;

};

#endif // LIGHT_HPP