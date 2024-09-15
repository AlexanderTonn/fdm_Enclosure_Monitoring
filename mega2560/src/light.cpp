#include "light.hpp"
/**
 * @brief Adjust the light intensity of the enclosure
 * @note only values between 0 and 100 (%) are allowed
 * @param value brightness in percent 
 * @return byte PWM Value
 */
auto lightController::adjust(uint8_t value) -> byte
{
    if(value > 100 || value < 0)
        return 0;
    
    return map(value, 0, 100, 0, 255);
}