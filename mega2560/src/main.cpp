#include <Arduino.h>
#include <io.hpp>

#include "nextionHMI.hpp"
#include "pinMapping.hpp"
#include "logic.hpp"


NextionHMI _hmi;
IO _io;
Logic _logic(&_io, &_hmi);


constexpr static uint32_t baud PROGMEM = 115200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  delay(5000);
  Serial.println("Starting up...");
  _hmi.init(baud);


  _io.reservePin(DHT_INDOOR, IO::mode::BUS);
  _io.reservePin(DHT_OUTDOOR, IO::mode::BUS);
  _io.reservePin(PWM_FAN_INTO, IO::mode::ANALOG_OUT);
  _io.reservePin(PWM_FAN_OUT, IO::mode::ANALOG_OUT);
  _io.reservePin(PWM_LIGHT, IO::mode::ANALOG_OUT);

  _logic.init();
}

void loop() {
  _io.readAll();

  _hmi.loop();
  _logic.loop();
  

  _io.writeAll();
}