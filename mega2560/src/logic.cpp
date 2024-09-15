#include "logic.hpp"

Logic::Logic(IO *io, NextionHMI *hmi) :  mIo(io), mHmi(hmi),
                        mInletPID(&mInletValues.input, &mInletValues.output, &mInletValues.setpoint, mInletValues.Kp, mInletValues.Ki, mInletValues.Kd, REVERSE),
                        mOutletPID(&mOutletValues.input, &mOutletValues.output, &mOutletValues.setpoint, mOutletValues.Kp, mOutletValues.Ki, mOutletValues.Kd, REVERSE) 
                        {
                            mInletPID.SetMode(AUTOMATIC);
                            mOutletPID.SetMode(AUTOMATIC);
                        }
/**
 * @brief Loop function for the logic
 * 
 */
auto Logic::loop() -> void
{
    if(mIo == nullptr)
        return;
    
    mInletValues.input = mIo->mRawData[DHT_OUTDOOR];
    mOutletValues.input = mIo->mRawData[DHT_INDOOR];

    // Load HMI Settings to internal structures
    hmiToIntern();

    mIo->mRawData[PWM_FAN_OUT] = fanController(mOutletValues, mOutletPID, mHmi->mSettings.fanControl.SpeedOutput);
    mIo->mRawData[PWM_FAN_INTO] = fanController(mInletValues, mInletPID, mHmi->mSettings.fanControl.SpeedInput);

    mIo->mRawData[PWM_LIGHT] = mLight.adjust(mHmi->mSettings.lightControl.intensity);   

}
/**
 * @brief Control the enclosure fans by using PID controller
 * 
 * @param _pidValues _pidValues instance of the PID values struct
 * @param _PID PID object for the controller
 * @param _fan instance of the fan settings struct from the HMI - read only
 * @return uint16_t 
 */
auto Logic::fanController(pidValues &_pidValues, 
                            PID &_PID,
                            NextionHMI::hmiSettings::fanControl::speedLimits _fan) -> uint16_t
{ 
    // Manual Mode selected
    if(_fan.manualMode)
    {
        return map(_fan.manualSpeed, 0, 100, _fan.minSpeed, _fan.maxSpeed);
    } // update the internal pid values from the HMI

    _PID.SetOutputLimits(_fan.minSpeed, _fan.maxSpeed);
    _PID.SetSampleTime(_pidValues.sampletime);
    _PID.SetTunings(_pidValues.Kp, _pidValues.Ki, _pidValues.Kd);  
    _PID.Compute();
    

    return static_cast<uint16_t>(_pidValues.output);
}



auto Logic::hmiToIntern() -> void
{
    mInletValues.Kd = mHmi->mSettings.fanControl.pidInput.Kd;
    mInletValues.Ki = mHmi->mSettings.fanControl.pidInput.Ki;
    mInletValues.Kp = mHmi->mSettings.fanControl.pidInput.Kp;
    mInletValues.sampletime = mHmi->mSettings.fanControl.pidInput.sampletime;

    mOutletValues.Kd = mHmi->mSettings.fanControl.pidOutput.Kd;
    mOutletValues.Ki = mHmi->mSettings.fanControl.pidOutput.Ki;
    mOutletValues.Kp = mHmi->mSettings.fanControl.pidOutput.Kp;
    mOutletValues.sampletime = mHmi->mSettings.fanControl.pidOutput.sampletime;
}