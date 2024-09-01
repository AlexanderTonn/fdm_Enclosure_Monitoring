#include "logic.hpp"

Logic::Logic(IO *io) :  mIo(io), 
                        mInletPID(&mInletValues.input, &mInletValues.output, &mInletValues.setpoint, mInletValues.Kp, mInletValues.Ki, mInletValues.Kd, DIRECT),
                        mOutletPID(&mOutletValues.input, &mOutletValues.output, &mOutletValues.setpoint, mOutletValues.Kp, mOutletValues.Ki, mOutletValues.Kd, DIRECT) { }
/**
 * @brief Loop function for the logic
 * 
 */
auto Logic::loop() -> void
{
    if(mIo == nullptr)
        return;
    
    mIo->mRawData[PWM_FAN_OUT] = fanController(mIo->mRawData[DHT_INDOOR], mOutletValues, mOutletPID);
    mIo->mRawData[PWM_FAN_INTO] = fanController(mIo->mRawData[DHT_OUTDOOR], mInletValues, mInletPID);
 
}
/**
 * @brief Control the enclosure fans by using PID controller
 * 
 * @return result of PID controller as pwm value
 */
auto Logic::fanController(uint16_t input, pidValues _pidValues, PID _PID) -> double
{ 
    if(!_pidValues.initialized)
        _PID.SetMode(AUTOMATIC);
    
    _pidValues.input = static_cast<double>(input);
    _PID.Compute();
    return _pidValues.output;
}