#include "light.hpp"
/**
 * @brief Adjust the light intensity of the enclosure
 * @note only values between 0 and 100 (%) are allowed
 * @param value brightness in percent 
 * @return byte PWM Value
 */
auto lightController::adjust(byte *value) -> byte
{
    if (value == nullptr)
        return 0;

    if(*value > 100 || *value < 0)
        return 0;

    byte output = map(*value, 0, 100, 0, 255);
    *value = map(output, 0, 255, 0, 100);

    return output;
}