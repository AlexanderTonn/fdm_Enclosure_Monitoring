#ifndef PID_HPP
#define PID_HPP

#include <Arduino.h>

class PID
{
    public: 
        auto setTunings(double Kp, double Ki, double Kd, double dt) -> void;
        auto calc(double setpoint, double actual) -> double;
        auto reset() -> void;

    enum class Direction
    {
        DIRECT = 0,
        REVERSE = 1
    };
        auto setDirection(Direction) -> void;
        auto setLimits(double min, double max) -> void;
        auto setSampletime(double sampletime) -> void;
        auto setSetpoint(double setpoint) -> void;
        

    private: 
        double mIntegral, mPrev_error;
        double mKp = 0.15; 
        double mKi = 0.01; 
        double mKd = 0.01;
        double mSampletime = 1000.0;
        double mInvertSignal = 1.0;
        double mMin = 0.0, mMax = 1023.0; 


};

#endif // PID
