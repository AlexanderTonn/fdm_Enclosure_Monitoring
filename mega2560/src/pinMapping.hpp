#ifndef PINMAPPING_HPP
#define PINMAPPING_HPP

#include <Arduino.h>
 // DHT 21 (AM2301)
constexpr static uint8_t DHT_INDOOR PROGMEM = 22;  // INPUT: DHT Sensor inside the enclosure
constexpr static uint8_t DHT_OUTDOOR PROGMEM = 23; // INPUT: DHT Sensor outside the enclosure

constexpr static uint8_t PWM_FAN_INTO PROGMEM = 6; // OUTPUT: PWM signal to control the fan speed which is blowding into the enclosure
constexpr static uint8_t PWM_FAN_OUT PROGMEM = 7; // OUTPUT: PWM signal to control the fan speed which is blowing out of the enclosure
constexpr static uint8_t PWM_LIGHT PROGMEM = 8;  // OUTPUT: PWM signal to control the light intensity

constexpr static uint8_t MQ135 PROGMEM = A0; // INPUT: MQ135 sensor


#endif // PINMAPPING_HPP