#ifndef HARDWARE_LED_AODV_H
#define HARDWARE_LED_AODV_H

#include <sstream>
#include <string>

#include "hardware_hello_aodv.h"
#include "field_test_defines.h"

class HardwareLedAODV : public HardwareHelloAODV{
private:
    /**
     * @brief Constructor helper which sets up the led pins
     * 
     */
    void _hardwareLedAODV();

    /**
     * @brief Turns the led on for a specified duration and then off, the led pin must be setup before calling this function
     * 
     * @param pin the pin the led is on
     * @param duration_ms how many seconds the led should be on for
     */
    void _lightLed(int pin, uint duration_ms);

public:
    // Constructors

    HardwareLedAODV();

    /*!
     * @brief Construct a new HardwareLedAODV object
     * 
     * @param ip the ip address of the current device
     */
    HardwareLedAODV(uint32_t ip);
    HardwareLedAODV(const char* ip);

    /**
     * @brief Construct a new HardwareLedAODV object
     * 
     * @param ip the ip address of the current device
     * @param helloDurationMS the desired time to send out hello messages (program run time)
     */
    HardwareLedAODV(uint32_t ip,  uint64_t helloDurationMS);
    HardwareLedAODV(const char* ip, uint64_t helloDurationMS);

    // Destructors
    ~HardwareLedAODV();

    virtual int handlePackets() override;
};

#endif