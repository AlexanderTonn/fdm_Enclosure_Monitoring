#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <Arduino.h>
#include <io.hpp>
#include <PID_v1.h>
#include "pinMapping.hpp"

class Logic {
public:
    Logic(IO *io);

private: 
    IO *mIo;

public: 
    auto loop() -> void;

private:
    struct pidValues
    {
        double Kp = 3.0;
        double Ki = 0.15;
        double Kd = 0.25;
        double setpoint = 25.0;
        double input;
        double output;
        bool initialized = false; // flag to check if the PID controller has been inizialized
    } mInletValues, mOutletValues;

    auto fanController(uint16_t, pidValues &, PID&) -> uint16_t;

    PID mInletPID; // PID controller for the inlet fans
    PID mOutletPID; // PID controller for the outlet fans

}; 

#endif // LOGIC_HPP