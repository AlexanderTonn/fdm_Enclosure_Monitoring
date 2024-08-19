#include "nextionHMI.hpp"
/**
 * @brief Construct a new Nextion H M I:: Nextion H M I object
 * 
 */
NextionHMI::NextionHMI()
{
    // Constructor
    // Initialize the Nextion HMI
    pEasyNex = new EasyNex (Serial);
}