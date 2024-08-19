#ifndef NEXTIONHMI_HPP
#define NEXTIONHMI_HPP

#include <Arduino.h>
#include <EasyNextionLibrary.h>

class NextionHMI 
{ 
    public:
    NextionHMI();
    
    /**
     * @brief must be called in void loop() to response touch events
     * 
     */
    auto listen() -> void
    {
        pEasyNex->NextionListen();
    }
    /**
     * @brief initialize the Nextion HMI
     * @note must be called in void setup()
     * @param baud 
     */
    auto init(uint16_t baud) -> void
    {
        pEasyNex->begin(baud);
    }

    private: 
    EasyNex* pEasyNex = nullptr;
};

#endif // NEXTIONHMI_HPP