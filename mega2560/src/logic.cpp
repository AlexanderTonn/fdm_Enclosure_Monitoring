#include "logic.hpp"

Logic::Logic(IO *io, NextionHMI *hmi) : mIo(io), mHmi(hmi)
{
}

auto Logic::init() -> void
{

    static auto initDone = false;

    if (initDone)
        return;

    initDone = true;

    mDhtIndoor = new dhtSensor(DHT_INDOOR);
    mDhtOutdoor = new dhtSensor(DHT_OUTDOOR);

    if (mDhtIndoor == nullptr || mDhtOutdoor == nullptr)
    {
        delete mDhtIndoor;
        delete mDhtOutdoor;
        return;
    }

    mPID.setDirection(PID::Direction::REVERSE);
    mPID.setSampletime(100);

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
    auto static gasConcentration = mMQ135.getConcentration(mIo);
    // Temperature & Humidity Control
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    if (mTempHumTimer.cyclic(3000))
    {
        mDhtIndoor->getValues();
        mDhtOutdoor->getValues();
        mPID_values.input = mDhtIndoor->mTemperature;
    }

    if (mHmi->mSettings.fanControl.observeEnvironment)
        autoMinTemperature();

    mPID_values.setpoint = mHmi->mSettings.fanControl.setpoint;
    auto fanSignal = fanController( &mPID_values, 
                                    &mPID, 
                                    &mHmi->mSettings.fanControl.Speed,
                                    mHmi->mSettings.fanControl.setpoint,
                                    mDhtIndoor->mTemperature);

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
    mIo->setValue(PWM_FAN_OUT, static_cast<uint16_t>(fanSignal)); 
    mIo->setValue(PWM_FAN_INTO, static_cast<uint16_t>(fanSignal));
    mIo->setValue(PWM_LIGHT, PWM_light);

    updateHmiData(fanSignal, PWM_light, mDhtIndoor->mTemperature, mDhtIndoor->mHumidity);
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
                          NextionHMI::hmiSettings::fanControl::speedLimits *_fan,
                          uint16_t setpoint, 
                          uint16_t actual) -> byte
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

    _PID->setTunings(p, i, d, _pidValues->sampletime);
    return _PID->calc(setpoint, actual);

}

auto Logic::hmiToIntern() -> void
{
    static auto factor = 0.01;
    mPID_values.Kd = mHmi->mSettings.fanControl.pid.Kd*factor;
    mPID_values.Ki = mHmi->mSettings.fanControl.pid.Ki*factor;
    mPID_values.Kp = mHmi->mSettings.fanControl.pid.Kp*factor;
    mPID_values.sampletime = mHmi->mSettings.fanControl.pid.sampletime;
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