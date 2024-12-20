#include "nextionHMI.hpp"
/**
 * @brief Construct a new Nextion H M I:: Nextion H M I object
 *
 */
NextionHMI::NextionHMI()
{
    // Constructor
    // Initialize the Nextion HMI
    pEasyNex = new EasyNex(Serial);
}
/**
 * @brief must be called in void loop() to response touch events and getting the values from the HMI
 *
 */
auto NextionHMI::loop() -> void
{
    if (pEasyNex == nullptr)
        return;

    initSettings();
    pEasyNex->NextionListen();
    getSettings();
    setHeaderData();
}
/**
 * @brief Handling the data output to the header in the hmi
 *
 */
auto NextionHMI::setHeaderData() -> void
{
    pEasyNex->writeNum("globalVars.va0.val", mHeader.fanSpeed);
    pEasyNex->writeNum("globalVars.va1.val", mHeader.lightIntensity);
    pEasyNex->writeNum("globalVars.va2.val", mHeader.temperature);
    pEasyNex->writeNum("globalVars.va6.val", mHeader.humidity);

    // Set states for showing the states
    mHeader.fanActive = (mHeader.fanSpeed > 0) ? 1 : 0;
    mHeader.lightActive = (mHeader.lightIntensity > 0) ? 1 : 0;
    mHeader.humidityState = (mHeader.humidity > mSettings.humidityControl.humidityCritical) ? 1 : 0;

    // Set states for showing the temperature picture
    if (mHeader.temperature < mSettings.fanControl.setpoint)
        mHeader.temperatureState = 0;
    else if (mHeader.temperature > mSettings.fanControl.warn)
        mHeader.temperatureState = 1;
    else if (mHeader.temperature > mSettings.fanControl.critical)
        mHeader.temperatureState = 2;

    pEasyNex->writeNum("globalVars.va3.val", mHeader.fanActive);
    pEasyNex->writeNum("globalVars.va4.val", mHeader.lightActive);
    pEasyNex->writeNum("globalVars.va5.val", mHeader.temperatureState);
    pEasyNex->writeNum("globalVars.va7.val", mHeader.humidityState);
}
/**
 * @brief Set Settings Variables in first Run
 * 
 */
auto NextionHMI::initSettings() -> void
{
    if(mInitDone)
        return;

    mInitDone = true;

    // Initialize the settings
    pEasyNex->writeNum("lightControl1.n5.val", mSettings.lightControl.twiglightIntensity);
    pEasyNex->writeNum("lightControl1.n4.val", mSettings.lightControl.twiglightThreshold);
    pEasyNex->writeNum("lightControl1.c0.val", mSettings.lightControl.twilightControl);
    pEasyNex->writeNum("lightControl2.c0.val", mSettings.lightControl.printDetection);
    pEasyNex->writeNum("lightControl3.c0.val", mSettings.lightControl.manualControl);
    pEasyNex->writeNum("lightControl3.h0.val", mSettings.lightControl.intensity);
    
    pEasyNex->writeNum("fanControl1.c0.val", mSettings.fanControl.observeEnvironment);
    pEasyNex->writeNum("fanControl1.n4.val", mSettings.fanControl.setpoint);
    pEasyNex->writeNum("fanControl1.n5.val", mSettings.fanControl.warn);
    pEasyNex->writeNum("fanControl1.n6.val", mSettings.fanControl.critical);
    pEasyNex->writeNum("fanControl2.n3.val", mSettings.fanControl.Speed.minSpeed);
    pEasyNex->writeNum("fanControl2.n4.val", mSettings.fanControl.Speed.maxSpeed);
    pEasyNex->writeNum("fanControl3.x3.val", mSettings.fanControl.pid.Kp);
    pEasyNex->writeNum("fanControl3.x4.val", mSettings.fanControl.pid.Ki);
    pEasyNex->writeNum("fanControl3.x5.val", mSettings.fanControl.pid.Kd);
    pEasyNex->writeNum("fanControl3.n3.val", mSettings.fanControl.pid.sampletime);
    pEasyNex->writeNum("fanControl4.c0.val", mSettings.fanControl.Speed.manualMode);
    pEasyNex->writeNum("fanControl4.h0.val", mSettings.fanControl.Speed.manualSpeed);
    

}
/**
 * @brief 
 * 
 */
auto NextionHMI::getSettings() -> void
{
    // get the values from the HMI
    mSettings.lightControl.twiglightIntensity = pEasyNex->readNumber("lightControl1.n5.val");
    mSettings.lightControl.twiglightThreshold = pEasyNex->readNumber("lightControl1.n4.val");
    mSettings.lightControl.twilightControl = pEasyNex->readNumber("lightControl1.c0.val");
    mSettings.lightControl.printDetection = pEasyNex->readNumber("lightControl2.c0.val");
    mSettings.lightControl.manualControl = pEasyNex->readNumber("lightControl3.c0.val");
    mSettings.lightControl.intensity = pEasyNex->readNumber("lightControl3.h0.val");

    mSettings.fanControl.observeEnvironment = pEasyNex->readNumber("fanControl1.c0.val");
    mSettings.fanControl.setpoint = pEasyNex->readNumber("fanControl1.n4.val");
    mSettings.fanControl.warn = pEasyNex->readNumber("fanControl1.n5.val");
    mSettings.fanControl.critical = pEasyNex->readNumber("fanControl1.n6.val");
    mSettings.fanControl.Speed.minSpeed = pEasyNex->readNumber("fanControl2.n3.val");
    mSettings.fanControl.Speed.maxSpeed = pEasyNex->readNumber("fanControl2.n4.val");
    mSettings.fanControl.pid.Kp = pEasyNex->readNumber("fanControl3.x3.val");
    mSettings.fanControl.pid.Ki = pEasyNex->readNumber("fanControl3.x4.val");
    mSettings.fanControl.pid.Kd = pEasyNex->readNumber("fanControl3.x5.val");
    mSettings.fanControl.pid.sampletime = pEasyNex->readNumber("fanControl3.n3.val");
    mSettings.fanControl.Speed.manualMode = pEasyNex->readNumber("fanControl4.c0.val");
    mSettings.fanControl.Speed.manualSpeed = pEasyNex->readNumber("fanControl4.h0.val");

    mSettings.humidityControl.humidityWarn = pEasyNex->readNumber("humidityControl1.n3.val");
    mSettings.humidityControl.humidityCritical = pEasyNex->readNumber("humidityControl1.n4.val");
}
/**
 * @brief Update the hmi settings if the settings are changed
 * @note this function should prevent unnecessary updates of the settings
 */
auto NextionHMI::updateSettings() -> void
{
    if(!checksumChanged())
        return;
    
    pEasyNex->writeNum("fanControl1.n4.val", mSettings.fanControl.setpoint);
    pEasyNex->writeNum("fanControl1.c0.val", mSettings.fanControl.observeEnvironment);

}
/**
 * @brief Updates the checksumm of the settings
 * @note 
 * @param String 
 * @return void
 */
auto NextionHMI::checksumChanged() -> bool
{
    mChecksum += mSettings.fanControl.setpoint;
    mChecksum += mSettings.fanControl.observeEnvironment; 

    if(mChecksum == mOldChecksum)
        return false;

    mOldChecksum = mChecksum;
    return true; 
}