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
        DIRECT = 1,
        REVERSE = -1
    };
        auto setDirection(Direction) -> void;
        auto setLimits(double min, double max) -> void;
        auto setSetpoint(double setpoint) -> void;
        auto setTimeConversionFactor(double timeConversionFactor) -> void;
        auto setSampleTime(uint16_t sampleTime) -> void;
        

    private: 
        double mIntegral, mPrev_error;
        double mKp;
        double mKi;
        double mKd;
        double mDirection = 1.0;
        double mMin = 0.0, mMax = 1023.0; 
        uint16_t mSampleTime = 1000;
        double mTimeConversionFactor = 1000.0;
        uint32_t mMillisOld ;


};

#endif // PID
