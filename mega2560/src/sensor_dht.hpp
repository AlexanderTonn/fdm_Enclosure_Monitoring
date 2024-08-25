#ifndef SENSOR_DHT_HPP
#define SENSOR_DHT_HPP

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>  

class dhtSensor
{
    public:
        dhtSensor(uint8_t pin, uint8_t mode);
    
    private:
        uint8_t mPin;
        uint8_t mMode;
        DHT mDht;

    public:
        auto start() -> void;
        bool mIsStarted = false; // flag to check if the sensor has been started

    public:    
        auto getHumidity() -> float;
        auto getTemperature() -> float;



};

#endif // SENSOR_DHT_HPP