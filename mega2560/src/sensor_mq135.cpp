#include "sensor_mq135.hpp"
/**
 * @brief Starting the MQ_135 sensor
 * @note This function also considers the heatup time
 * @param pin 
 */
auto MQ_135::getConcentration(IO *pIo) -> uint16_t
{
    if(pIo == nullptr)
        return 0;


    if(!mWarmUpTimer.delayOn(60000, true))
        return;

    mValue = map(pIo->mRawData[MQ135], 0, 1023, mMin, mMax);

}
/**
 * @brief Adjust the limits for the ppm value
 * @note by default the limits are set to 0 and 1000 ppm
 * @param min 
 * @param max 
 */
auto MQ_135::setLimits(uint16_t min, uint16_t max) -> void
{
    mMin = min;
    mMax = max;
}