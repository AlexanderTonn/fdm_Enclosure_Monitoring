#include "sensor_dht.hpp"

// Constructor
dhtSensor::dhtSensor(uint8_t pin, uint8_t mode) : mPin(pin) , mMode(mode), mDht(pin, mode) { }

/**
 * @brief Call this function in your coid setup() to start the sensor
 * 
 */
auto dhtSensor::start() -> void
{
    if(mIsStarted)
    {
        return;
    }

    mDht.begin();
    mIsStarted = true;
    Serial.println(String(millis()) + String(" DHT Sensor started on pin: ") + String(mPin));
}
/**
 * @brief Get Temperature in Celsius
 * 
 * @return float 
 */
auto dhtSensor::getTemperature() -> float
{
    if(!mIsStarted)
        return -1;

    auto value = mDht.readTemperature();

    // return -2 if the temperature is not a number
    if(isnan(value))
        return -2;

    return value;
}
/**
 * @brief Get the current Humidity
 * 
 * @return float 
 */
auto dhtSensor::getHumidity() -> float
{
    if(!mIsStarted)
        return -1;

    auto value = mDht.readHumidity();

    // return -2 if the humidity is not a number
    if(isnan(value))
        return -2;

    return value;
}
