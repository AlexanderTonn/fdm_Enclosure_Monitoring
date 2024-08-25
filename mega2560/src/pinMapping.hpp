#ifndef PINMAPPING_HPP
#define PINMAPPING_HPP

#include <Arduino.h>

#define DHTTYPE DHT21   // DHT 21 (AM2301)
constexpr static uint8_t DHT_INDOOR PROGMEM = 2; 
constexpr static uint8_t DHT_OUTDOOR PROGMEM = 3;

#endif // PINMAPPING_HPP