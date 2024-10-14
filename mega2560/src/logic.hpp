#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <Arduino.h>
#include <io.hpp>
#include <PID_v1.h>

#include "pinMapping.hpp"
#include "light.hpp"
#include "nextionHMI.hpp"
#include "sensor_dht.hpp"
#include "timer.hpp"

class Logic {
public:
    Logic(IO *io, NextionHMI *hmi);

private: 
    IO *mIo;
    NextionHMI *mHmi;
    lightController mLight;
    dhtSensor *mDhtIndoor; // Object for sensor inside the enclosure
    dhtSensor *mDhtOutdoor; // Object for sensor outside the enclosure

public: 
    auto init() -> void;
    auto loop() -> void;

public:
    // must be public for accessing by hmi class
    struct pidValues
    {
        double Kp = 1.0;
        double Ki = 0.15;
        double Kd = 0.25;
        double setpoint = 25.0;
        double input;
        double output;
        uint16_t sampletime = 100; // sample time in ms
    }mInletValues, mOutletValues;

// functions for fan control
private:
    auto fanController(pidValues&, PID&, NextionHMI::hmiSettings::fanControl::speedLimits&) -> byte;
    auto hmiToIntern() -> void;
    auto updateHmiData() -> void;
    auto autoMinTemperature() -> void;

    PID mInletPID; // PID controller for the inlet fans
    PID mOutletPID; // PID controller for the outlet fans

// Temperature & Humidity Control
private:
    timer mTempHumTimer;



}; 

#endif // LOGIC_HPP