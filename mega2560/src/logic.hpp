#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <Arduino.h>
#include <io.hpp>
#include <PID_v1.h>

class Logic {
public:
    Logic(IO *io);

private: 
    IO *mIo;

public: 
    auto loop() -> void;

private:
    auto controlTemperature() -> void;
    struct pidValues
    {
        float Kp;
        float Ki;
        float Kd;
        float setpoint;
        float input;
        float output;
    } mPidValues;

    PID mPID(&mPidValues.input, &mPidValues.output, &mPidValues.setpoint, mPidValues.Kp, mPidValues.Ki, mPidValues.Kd, DIRECT);

};

#endif // LOGIC_HPP