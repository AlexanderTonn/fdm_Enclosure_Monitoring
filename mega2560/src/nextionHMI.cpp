#include "nextionHMI.hpp"
/**
 * @brief Construct a new Nextion H M I:: Nextion H M I object
 * 
 */
NextionHMI::NextionHMI()
{
    // Constructor
    // Initialize the Nextion HMI
    pEasyNex = new EasyNex (Serial);
}
    /**
     * @brief must be called in void loop() to response touch events and getting the values from the HMI
     * 
     */
auto NextionHMI::loop() -> void
{
    if(pEasyNex == nullptr)
        return;

    pEasyNex->NextionListen();

    // get the values from the HMI
    mSettings.lightControl.twiglightIntensity = pEasyNex->readNumber("lightControl1.n5.val");
    mSettings.lightControl.twiglightThreshold = pEasyNex->readNumber("lightControl1.n4.val");
    mSettings.lightControl.twilightControl = pEasyNex->readNumber("lightControl1.c0.val");
    mSettings.lightControl.printDetection = pEasyNex->readNumber("lightControl2.c0.val");
    mSettings.lightControl.printDetection = pEasyNex->readNumber("lightControl3.c0.val");
    mSettings.lightControl.intensity = pEasyNex->readNumber("lightControl3.h0.val");

    mSettings.fanControl.autoSetpoint = pEasyNex->readNumber("fanControl1.c0.val");
    mSettings.fanControl.setpoint = pEasyNex->readNumber("fanControl1.n4.val");
    mSettings.fanControl.SpeedInput.minSpeed = pEasyNex->readNumber("fanControl2.n3.val");
    mSettings.fanControl.SpeedInput.maxSpeed = pEasyNex->readNumber("fanControl2.n4.val");
    mSettings.fanControl.SpeedInput.minSpeed = pEasyNex->readNumber("fanControl2.n5.val");
    mSettings.fanControl.SpeedInput.maxSpeed = pEasyNex->readNumber("fanControl2.n6.val");
    mSettings.fanControl.pidInput.Kp = pEasyNex->readNumber("fanControl3.n3.val");
    mSettings.fanControl.pidInput.Ki = pEasyNex->readNumber("fanControl3.n4.val");
    mSettings.fanControl.pidInput.Kd = pEasyNex->readNumber("fanControl3.n5.val");
    mSettings.fanControl.pidInput.sampletime = pEasyNex->readNumber("fanControl3.n6.val");
    mSettings.fanControl.pidOutput.Kp = pEasyNex->readNumber("fanControl3.n7.val");
    mSettings.fanControl.pidOutput.Ki = pEasyNex->readNumber("fanControl3.n8.val");
    mSettings.fanControl.pidOutput.Kd = pEasyNex->readNumber("fanControl3.n9.val");    
    mSettings.fanControl.pidOutput.sampletime = pEasyNex->readNumber("fanControl3.n10.val");
    mSettings.fanControl.SpeedInput.manualMode = pEasyNex->readNumber("fanControl4.c0.val");


}
/**
 * @brief Handling the data output to the header in the hmi
 * 
 */
auto NextionHMI::headerHandler() -> void
{
    // Header Handler
}
