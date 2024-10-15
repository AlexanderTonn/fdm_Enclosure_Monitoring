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
 * @note if true and setpoint is higher than actual value the output signal will be increased 
 * @note if false and setpoint is higher than actual value the output signal will be decreased
 * @param invert 
 */
auto PID::setInvert(bool invert) -> void
{
    mInvertSignal = invert ? -1.0 : 1.0;
}