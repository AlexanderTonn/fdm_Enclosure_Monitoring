#include <Arduino.h>
#include <io.hpp>

#include "nextionHMI.hpp"
#include "pinMapping.hpp"

NextionHMI _hmi;
IO _io;

constexpr static uint16_t baud PROGMEM = 9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  _hmi.init(baud);

  _io.reservePin(DHT_INDOOR, IO::mode::BUS);
  _io.reservePin(DHT_OUTDOOR, IO::mode::BUS);

}

void loop() {
  _io.readAll();

  _hmi.listen();

  _io.writeAll();
}