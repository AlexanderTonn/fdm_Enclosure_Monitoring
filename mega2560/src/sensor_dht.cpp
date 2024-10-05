#include "sensor_dht.hpp"


auto dhtSensor::init() -> void
{
    mDht.begin();
}


/**
 * @brief Get Temperature in Celsius
 *
 * @return float
 */
auto dhtSensor::getValues() -> int
{
    auto ret = 0; 

    auto start = micros();
    mHumidity = mDht.readHumidity();
    mTemperature = mDht.readTemperature();
    auto stop = micros();
    mLastRead = stop - start;

    if(isnan(mTemperature))
    {
         mTemperature = -99;
         ret = -1; 
    }
    if (isnan(mHumidity))
    {
        mHumidity = -99;
        ret = -1;
    }


    return ret;

}
