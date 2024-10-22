#include "pidController.hpp"
/**
 * @brief 
 * 
 * @param Kp 
 * @param Ki 
 * @param Kd 
 * @param dt 
 */
auto PID::setTunings(double Kp, double Ki, double Kd, double sampletime) -> void
{
    mKp = Kp;
    mKi = Ki;
    mKd = Kd;
    mSampletime = sampletime;

    // reset the controller if the values has been changed
    static double kpChanged = 0, kiChanged = 0, kdChanged = 0;
    if (mKp != kpChanged || mKi != kiChanged || mKd != kdChanged)
    {
        Serial.println("PID values changed");
        kpChanged = mKp;
        kiChanged = mKi;
        kdChanged = mKd;
        reset();
    }

}
/**
 * @brief Reset the PID controller
 * 
 */
auto PID::reset() -> void
{
    mIntegral = 0;
    mPrev_error = 0;
}
/**
 * @brief  Set the limits for the output signal
 * 
 * @param min 
 * @param max 
 */
auto PID::setLimits(double min, double max) -> void
{
    mMin = min;
    mMax = max;
}
/**
 * @brief  Set the sample time for the PID controller
 * 
 * @param sampletime 
 */
auto PID::setSampletime(double sampletime) -> void
{
    mSampletime = sampletime;
}

/**
 * @brief  Calculate the PID output
 * 
 * @param setpoint 
 * @param actual 
 * @return double 
 */
auto PID::calc(double setpoint, double actual) -> double 
{
    auto error = setpoint - actual;

    // Proportional term
    auto p = mKp * error;

    // Integral term
    mIntegral += error * mSampletime;
    auto i = mKi * mIntegral;

    // Derivative term
    auto derivative = (error - mPrev_error) / mSampletime;
    auto d = mKd * derivative;

    // output
    auto out = p + i + d;

    // save for next iteration
    mPrev_error = error;

    // Limit the output signal range
    if(out > mMax)
    {
        out = mMax;
        // Prevent integral wind-up
        mIntegral -= error * mSampletime;
    }
    else if(out < mMin)
    {
        out = mMin;
        // Prevent integral wind-up
        mIntegral -= error * mSampletime;
    }

    return out * mInvertSignal;
}
/**
 * @brief Invert the output signal
 * @note DIRECT means the output signal will be increased if the setpoint is lower than the actual value
 * @note REVERSE means the output signal will be increased if the setpoint is higher than the actual value
 * @param PID::Direction
 */
auto PID::setDirection(PID::Direction direction = PID::Direction::DIRECT) -> void
{
    switch (direction)
    {
    case PID::Direction::DIRECT:
        mInvertSignal = 1.0;
        break;
    case PID::Direction::REVERSE:
        mInvertSignal = -1.0;
        break;
    default:
        break;
    }
}