#ifndef HARDWARE_LED_AODV_H
#define HARDWARE_LED_AODV_H

#include <sstream>
#include <string>

#include "hardware_hello_aodv.h"
#include "field_test_defines.h"
#include "led_helper.h"

class HardwareLedAODV : public HardwareHelloAODV{
private:
    /**
     * @brief Constructor helper which sets up the led pins
     * 
     */
    void _hardwareLedAODV();

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