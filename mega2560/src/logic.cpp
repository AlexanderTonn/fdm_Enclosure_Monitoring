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

    auto static gasConcentration = mMQ135.getConcentration(mIo);
    // Temperature & Humidity Control
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    if (mTempHumTimer.cyclic(3000))
    {
        mDhtIndoor->getValues();
        mDhtOutdoor->getValues();
    }

    if (mHmi->mSettings.fanControl.observeEnvironment)
        autoMinTemperature();

    auto fanSignal = fanController();

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
auto Logic::fanController() -> byte
{
    auto min = map(mHmi->mSettings.fanControl.Speed.minSpeed, 0, 100, 0, 255);
    auto max = map(mHmi->mSettings.fanControl.Speed.maxSpeed, 0, 100, 0, 255);

    // Manual Mode selected
    if (mHmi->mSettings.fanControl.Speed.manualMode)
    {
        return map(mHmi->mSettings.fanControl.Speed.manualSpeed, 0, 100, min, max);
    } // update the internal pid values from the HMI

    mPID.setLimits(min, max);

    //Converting the values from the HMI format to double
    static auto factor = 0.01;
    auto p = mHmi->mSettings.fanControl.pid.Kp*factor;
    auto i = mHmi->mSettings.fanControl.pid.Ki*factor;
    auto d = mHmi->mSettings.fanControl.pid.Kd*factor;

    mPID.setTunings(p, i, d, mHmi->mSettings.fanControl.pid.sampletime);
    return mPID.calc(mHmi->mSettings.fanControl.setpoint, mDhtIndoor->mTemperature);

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
        if(mDhtIndoor->mTemperature < mDhtOutdoor->mTemperature)
            mHmi->mSettings.fanControl.setpoint = mDhtOutdoor->mTemperature;
    }
    else
        mHmi->mSettings.fanControl.observeEnvironment = false;
}