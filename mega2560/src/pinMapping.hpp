#ifndef PINMAPPING_HPP
#define PINMAPPING_HPP

#include <Arduino.h>

#define DHTTYPE DHT21   // DHT 21 (AM2301)
constexpr static uint8_t DHT_INDOOR PROGMEM = 2;  // INPUT: DHT Sensor inside the enclosure
constexpr static uint8_t DHT_OUTDOOR PROGMEM = 3; // INPUT: DHT Sensor outside the enclosure

constexpr static uint8_t PWM_FAN_INTO PROGMEM = 8; // OUTPUT: PWM signal to control the fan speed which is blowding into the enclosure
constexpr static uint8_t PWM_FAN_OUT PROGMEM = 9; // OUTPUT: PWM signal to control the fan speed which is blowing out of the enclosure
constexpr static uint8_t PWM_LIGHT PROGMEM = 10;  // OUTPUT: PWM signal to control the light intensity


#endif // PINMAPPING_HPP