#ifndef MQ_135_HPP
#define MQ_135_HPP

#include <Arduino.h>
#include <timer.hpp>
#include <io.hpp>
#include "pinMapping.hpp"

class MQ_135
{
    private:
        timer mWarmUpTimer; //sensor needs to warm up
        uint16_t mValue; //scaled value
        uint16_t mMin = 0; // lower limit for ppm value
        uint16_t mMax = 1000; // upper limit for ppm value

    public: 
        auto setLimits(uint16_t, uint16_t) -> void;
        auto getConcentration(IO*) -> uint16_t;


};

#endif // MQ_135_HPP