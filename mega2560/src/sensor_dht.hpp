#ifndef SENSOR_DHT_HPP
#define SENSOR_DHT_HPP

#include <DHT.h>
#include <Adafruit_Sensor.h>

class dhtSensor
{
    public:
        dhtSensor(uint8_t pin, uint8_t mType = DHT21) : mPin(pin), mType(mType), mDht(pin, mType) {};
        double mTemperature;
        double mHumidity;

    private:
        uint8_t mPin;
        uint8_t mType;
        constexpr static uint8_t DHTTYPE PROGMEM = DHT21; 
        DHT mDht;
        uint32_t mLastRead;

    private:
        

    public:    
        auto init() -> void;
        auto getValues() -> int;
        auto getReadTime() -> uint32_t { return mLastRead; };
    




};

#endif // SENSOR_DHT_HPP