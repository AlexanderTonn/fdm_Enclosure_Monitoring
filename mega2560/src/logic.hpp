#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <Arduino.h>
#include <io.hpp>

#include "pinMapping.hpp"
#include "light.hpp"
#include "nextionHMI.hpp"
#include "sensor_dht.hpp"
#include "timer.hpp"
#include "pidController.hpp"
#include "sensor_mq135.hpp"
class Logic {
public:
    Logic(IO *io, NextionHMI *hmi);

private: 
    IO *mIo;
    NextionHMI *mHmi;
    lightController mLight;
    dhtSensor *mDhtIndoor; // Object for sensor inside the enclosure
    dhtSensor *mDhtOutdoor; // Object for sensor outside the enclosure
    MQ_135 mMQ135; // Object for the MQ135 sensor

public: 
    auto init() -> void;
    auto loop() -> void;

// functions for fan control
private:
    auto fanController() -> byte;
    auto updateHmiData(byte,byte,float,float) -> void;
    auto autoMinTemperature() -> void;

    PID mPID; // PID controller for the fans

// Temperature & Humidity Control
private:
    timer mTempHumTimer;



}; 

#endif // LOGIC_HPP