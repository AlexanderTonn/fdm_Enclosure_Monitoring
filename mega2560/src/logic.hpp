#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <Arduino.h>
#include <io.hpp>
#include <PID_v1.h>

#include "pinMapping.hpp"
#include "light.hpp"
#include "nextionHMI.hpp"

class Logic {
public:
    Logic(IO *io, NextionHMI *hmi);

private: 
    IO *mIo;
    NextionHMI *mHmi;
    lightController mLight;

public: 
    auto loop() -> void;

public:
    // must be public for accessing by hmi class
    struct pidValues
    {
        double Kp = 3.0;
        double Ki = 0.15;
        double Kd = 0.25;
        double setpoint = 25.0;
        double input;
        double output;
        uint16_t sampletime = 100; // sample time in ms
    } mInletValues, mOutletValues;

// functions for fan control
private:
    auto fanController(pidValues &, PID&, NextionHMI::hmiSettings::fanControl::speedLimits) -> uint16_t;
    auto hmiToIntern() -> void;
    auto updateHmiData() -> void;

    PID mInletPID; // PID controller for the inlet fans
    PID mOutletPID; // PID controller for the outlet fans

}; 

#endif // LOGIC_HPP