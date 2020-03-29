#ifndef HARDWARE_HELLO_AODV_H
#define HARDWARE_HELLO_AODV_H

#include "hardware_aodv.h"
#include "hardware_hello.h"

class HardwareHelloAODV : public HardwareAODV{
private:
    void _hardwareHelloAODV();
    void _hardwareHelloAODV(uint64_t helloDurationMS);
    /**
     * @brief this function is a while loop that called sendHellos, it is meant to be threaded 
     * 
     */
    void _sendHelloThreaded();

protected:
    HardwareHelloMonitor* helloMonitor;
    thread helloing;

public:
    // Constructors

    HardwareHelloAODV();

    /*!
     * @brief Construct a new HardwareHelloAODV object
     * 
     * @param ip the ip address of the current device
     */
    HardwareHelloAODV(uint32_t ip);
    HardwareHelloAODV(const char* ip);

    /**
     * @brief Construct a new HardwareHelloAODV object
     * 
     * @param ip the ip address of the current device
     * @param helloDurationMS the desired time to send out hello messages (program run time)
     */
    HardwareHelloAODV(uint32_t ip,  uint64_t helloDurationMS);
    HardwareHelloAODV(const char* ip, uint64_t helloDurationMS);

    // Destructors
    ~HardwareHelloAODV();

    virtual int handlePackets() override;
};

#endif