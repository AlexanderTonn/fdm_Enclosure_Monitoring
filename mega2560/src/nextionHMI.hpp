#ifndef NEXTIONHMI_HPP
#define NEXTIONHMI_HPP

#include <Arduino.h>
#include <EasyNextionLibrary.h>

class NextionHMI 
{ 
    public:
    NextionHMI();
    
    auto loop() -> void;

    /**
     * @brief initialize the Nextion HMI
     * @note must be called in void setup()
     * @param baud 
     */
    auto init(uint16_t baud) -> void
    {
        pEasyNex->begin(baud);
    }

    public:
    struct hmiSettings
    {
        struct fanControl
        {
            bool autoSetpoint = true; // flag to check if the setpoint is set automatically
            byte setpoint = 25; // default setpoint for fancontrol
            byte warn = 30; // warning temperature
            byte critical = 35; // critical temperature
            struct speedLimits
            {
                byte minSpeed = 0; // min speed of the fan in percent
                byte maxSpeed = 100; // max speed of the fan in percent
                bool manualMode = false; // manual mode 
                byte manualSpeed = 40; // default speed in manual mode
            } SpeedInput, SpeedOutput;
            struct pidValues
            {
                float Kp = 3.0;
                float Ki = 0.15;
                float Kd = 0.15;
                uint16_t sampletime = 100; // sample time in ms

            } pidInput, pidOutput;

        } fanControl;
        struct lightControl
        {
            bool twilightControl = false; // Automatic twilight control if the sun goes down, the light goes on
            bool manualControl = false; // Manual control of the light
            bool printDetection = true; // light goes on if a print was started
            uint8_t intensity = 50; // default light intensity
            uint8_t twiglightIntensity = 30; // default light intensity for twilight
            uint8_t twiglightThreshold = 20; // threshold for activating the light if the sun goes down
        } lightControl;
        struct humidityControl
        {
            uint8_t humidityWarn = 60; // warning humidity
            uint8_t humidityCritical = 70; // critical humidity
        } humidityControl;
    }  mSettings;

    private: 
    EasyNex* pEasyNex = nullptr;


    private: 
    auto initSettings() -> void;
    bool mInitDone = false; 

    auto getSettings() -> void;
    auto setHeaderData() -> void;

    public:
    struct hmiHeader
    {
        bool fanActive = false; // 0 == off (red), 1 == on (green)
        bool lightActive = false; // 0 == off (red), 1 == on (green)
        bool humidityState = false; // 0 == normal (green), 1 == to High (red)
        byte temperatureState = 0; // 0 == normal (green), 1 == warning (yellow), 2 == critical (red)
        byte fanSpeed = 0; // 0 - 100 % fan speed
        byte lightIntensity = 0; // 0 - 100 % light intensity
        byte temperature = 0; // 0 - 100 °C 
        byte humidity = 0; // 0 - 100 %
    } mHeader;

};

#endif // NEXTIONHMI_HPP