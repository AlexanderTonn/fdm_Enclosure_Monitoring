#include "logic.hpp"

Logic::Logic(IO *io, NextionHMI *hmi) : mIo(io), mHmi(hmi),
                                        mInletPID(&mInletValues.input, &mInletValues.output, &mInletValues.setpoint, mInletValues.Kp, mInletValues.Ki, mInletValues.Kd, REVERSE),
                                        mOutletPID(&mOutletValues.input, &mOutletValues.output, &mOutletValues.setpoint, mOutletValues.Kp, mOutletValues.Ki, mOutletValues.Kd, REVERSE)
{
}

auto Logic::init() -> void
{

    static auto initDone = false;

    if (initDone)
        return;

    initDone = true;

    mInletPID.SetMode(AUTOMATIC);
    mOutletPID.SetMode(AUTOMATIC);

    mDhtIndoor = new dhtSensor(DHT_INDOOR);
    mDhtOutdoor = new dhtSensor(DHT_OUTDOOR);

    if (mDhtIndoor == nullptr || mDhtOutdoor == nullptr)
    {
        delete mDhtIndoor;
        delete mDhtOutdoor;
        return;
    }

    mDhtIndoor->init();
    mDhtIndoor->init();
}
/**
 * @brief Loop function for the logic
 *
 */
auto Logic::loop() -> void
{
    if (mIo == nullptr)
        return;

    // Load HMI Settings to internal structures
    hmiToIntern();

    // Temperature & Humidity Control
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    if (mTempHumTimer.cyclic(3000))
    {
        mDhtIndoor->getValues();
        mDhtOutdoor->getValues();
        mInletValues.input = mDhtIndoor->mTemperature;
        mOutletValues.input = mDhtIndoor->mTemperature;
    }

    if (mHmi->mSettings.fanControl.observeEnvironment)
        autoMinTemperature();

    mInletValues.setpoint = mHmi->mSettings.fanControl.setpoint;
    mOutletValues.setpoint = mHmi->mSettings.fanControl.setpoint;
    static auto PWM_outlet = fanController(&mOutletValues, &mOutletPID, &mHmi->mSettings.fanControl.SpeedOutput);
    static auto PWM_inlet = fanController(&mInletValues, &mInletPID, &mHmi->mSettings.fanControl.SpeedInput);


    // Light Control
    // TODO!: Implement Twilight Control
    static byte PWM_light = 0;
    if (mHmi->mSettings.lightControl.manualControl)
        PWM_light = mLight.adjust(&mHmi->mSettings.lightControl.intensity);

    else if (mHmi->mSettings.lightControl.twilightControl)
    {
        // TODO!: Implement Twilight Control
    }
    else
        PWM_light = 0;

    // Assign Arduino Outputs
    Serial.println("PWM Outlet: " + String(PWM_outlet) + " PWM Inlet: " + String(PWM_inlet) + " PWM Light: " + String(PWM_light));
    mIo->setValue(PWM_FAN_OUT, PWM_outlet); 
    mIo->setValue(PWM_FAN_INTO, PWM_inlet);
    mIo->setValue(PWM_LIGHT, PWM_light);

    updateHmiData(PWM_outlet, PWM_light, mDhtIndoor->mTemperature, mDhtIndoor->mHumidity);
}
/**
 * @brief Control the enclosure fans by using PID controller
 *
 * @param _pidValues _pidValues instance of the PID values struct
 * @param _PID PID object for the controller
 * @param _fan instance of the fan settings struct from the HMI - read only
 * @return uint16_t
 */
auto Logic::fanController(pidValues *_pidValues,
                          PID *_PID,
                          NextionHMI::hmiSettings::fanControl::speedLimits *_fan) -> byte
{
    auto min = map(_fan->minSpeed, 0, 100, 0, 255);
    auto max = map(_fan->maxSpeed, 0, 100, 0, 255);

    // Manual Mode selected
    if (_fan->manualMode)
    {
        return map(_fan->manualSpeed, 0, 100, min, max);
    } // update the internal pid values from the HMI

    _PID->setLimits(min, max);
    _PID->setSampletime(_pidValues->sampletime);

    //Converting the values from the HMI format to double
    static auto factor = 0.01;
    auto p = _pidValues->Kp*factor;
    auto i = _pidValues->Ki*factor;
    auto d = _pidValues->Kd*factor;

    _PID->SetTunings(p, i, d);
    _PID->Compute();

    return static_cast<byte>(_pidValues->output);
}

auto Logic::hmiToIntern() -> void
{
    static auto factor = 0.01;
    mInletValues.Kd = mHmi->mSettings.fanControl.pidInput.Kd*factor;
    mInletValues.Ki = mHmi->mSettings.fanControl.pidInput.Ki*factor;
    mInletValues.Kp = mHmi->mSettings.fanControl.pidInput.Kp*factor;
    mInletValues.sampletime = mHmi->mSettings.fanControl.pidInput.sampletime;

    mOutletValues.Kd = mHmi->mSettings.fanControl.pidOutput.Kd*factor;
    mOutletValues.Ki = mHmi->mSettings.fanControl.pidOutput.Ki*factor;
    mOutletValues.Kp = mHmi->mSettings.fanControl.pidOutput.Kp*factor;
    mOutletValues.sampletime = mHmi->mSettings.fanControl.pidOutput.sampletime;
}
/**
 * @brief Update the Data in the Header of the HMI
 *
 */
auto Logic::updateHmiData(byte fanSpeed, byte lightIntensity, float temperature, float humidity) -> void
{
    mHmi->mHeader.fanSpeed = map(fanSpeed, 0, 255, 0, 100);
    mHmi->mHeader.lightIntensity = map(lightIntensity, 0, 255, 0, 100);
    mHmi->mHeader.temperature = temperature;
    mHmi->mHeader.humidity = mDhtIndoor->mHumidity;
}
/**
 * @brief Assign environment temperature as min value if the outdoor temperature is lower
 *
 */
auto Logic::autoMinTemperature() -> void
{
    if (!isnan(mDhtOutdoor->mTemperature))
    {
        if(mDhtOutdoor->mTemperature < mDhtIndoor->mTemperature)
            mHmi->mSettings.fanControl.setpoint = mDhtOutdoor->mTemperature;
    }
    else
        mHmi->mSettings.fanControl.observeEnvironment = false;
}