#ifndef SENSOR_DHT_HPP
#define SENSOR_DHT_HPP

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>  

class dhtSensor
{
    public:
        dhtSensor() : mDht(DHTPIN, DHTTYPE) {};
        auto init() -> void;
        auto readHumidity() -> void;
        auto readTemperature() -> void;

    private:
        DHT mDht;

};

#endif // SENSOR_DHT_HPP