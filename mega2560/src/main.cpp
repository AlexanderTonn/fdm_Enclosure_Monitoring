#include <Arduino.h>
#include <io.hpp>

#include "nextionHMI.hpp"
#include "pinMapping.hpp"
#include "sensor_dht.hpp"
#include "logic.hpp"

NextionHMI _hmi;
IO _io;
Logic _logic(&_io, &_hmi);

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
  _io.reservePin(PWM_FAN_INTO, IO::mode::ANALOG_OUT);
  _io.reservePin(PWM_FAN_OUT, IO::mode::ANALOG_OUT);
  _io.reservePin(PWM_LIGHT, IO::mode::ANALOG_OUT);

  _dhtIndoor.start();
  _dhtOutdoor.start();
}

void loop() {
  _io.readAll();
  _io.mRawData[DHT_INDOOR] = static_cast<uint16_t>(_dhtIndoor.getTemperature());
  _io.mRawData[DHT_OUTDOOR] = static_cast<uint16_t>(_dhtOutdoor.getTemperature());
  
  
  _logic.loop();
  _hmi.loop();

  _io.writeAll();
}