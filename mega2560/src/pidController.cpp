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

    // reset the controller if the values has been changed
    static double kpChanged = 0, kiChanged = 0, kdChanged = 0;
    if (mKp != kpChanged || mKi != kiChanged || mKd != kdChanged)
    {
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
 * @brief  Calculate the PID output
 * 
 * @param setpoint 
 * @param actual 
 * @return double 
 */
auto PID::calc(double setpoint, double actual) -> double 
{
    auto millisNow = millis();

    static auto result = 0.0; // store the result for the next iteration

    if( millisNow - mMillisOld < mSampleTime )
        return result;

    auto deltaTime = (millisNow - mMillisOld) / mTimeConversionFactor;
    mMillisOld = millisNow;

    auto error = setpoint - actual;

    // Proportional term
    auto p = mKp * error;

    // Integral term
    mIntegral += error * deltaTime;
    auto i = mKi * mIntegral;

    // Derivative term
    auto derivative = (error - mPrev_error) / deltaTime;
    auto d = mKd * derivative;

    // output
    result = p + i + d;

    // save for next iteration
    mPrev_error = error;

    // Limit the output signal range
    if(result > mMax)
    {
        result = mMax;
        // Prevent integral wind-up
        mIntegral -= error * deltaTime;
    }
    else if(result < mMin)
    {
        result = mMin;
        // Prevent integral wind-up
        mIntegral -= error * deltaTime;
    }

    return result * mDirection;
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
        mDirection = 1.0;
        break;
    case PID::Direction::REVERSE:
        mDirection = -1.0;
        break;
    default:
        break;
    }
}
/**
 * @brief set time conversion factor
 * @note a higher value will result a slower response because of shorter time difference
 * 
 * @param timeConversionFactor 
 */
auto PID::setTimeConversionFactor(double timeConversionFactor) -> void
{
    mTimeConversionFactor = timeConversionFactor;
}
/**
 * @brief Set the sample time for the PID controller
 * 
 * @param sampleTime 
 */
auto PID::setSampleTime(uint16_t sampleTime) -> void
{
    mSampleTime = sampleTime;
}