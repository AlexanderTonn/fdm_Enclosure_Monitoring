#include <Arduino.h>
#include <io.hpp>

#include "nextionHMI.hpp"
#include "pinMapping.hpp"
#include "sensor_dht.hpp"

NextionHMI _hmi;
IO _io;

dhtSensor _dhtIndoor(DHT_INDOOR, DHTTYPE); // Object for sensor inside the enclosure
dhtSensor _dhtOutdoor(DHT_OUTDOOR, DHTTYPE); // Object for sensor outside the enclosure

constexpr static uint16_t baud PROGMEM = 9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  while (!Serial)
    ;
  
  Serial.println(String(millis()) + String(" Serial Started"));
  

  _hmi.init(baud);

  _io.reservePin(DHT_INDOOR, IO::mode::BUS);
  _io.reservePin(DHT_OUTDOOR, IO::mode::BUS);
  _dhtIndoor.start();
  _dhtOutdoor.start();
}

void loop() {
  _io.readAll();
  _io.mRawData[DHT_INDOOR] = static_cast<uint16_t>(_dhtIndoor.getTemperature());
  _io.mRawData[DHT_OUTDOOR] = static_cast<uint16_t>(_dhtOutdoor.getTemperature());

  _hmi.listen();

  _io.writeAll();
}