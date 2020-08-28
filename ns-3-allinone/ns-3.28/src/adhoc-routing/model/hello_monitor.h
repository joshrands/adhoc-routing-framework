/*********************************
 * hello_monitor.h
 *
 * C++ implementation of periodic HELLO messages to determine current neighbors 
 * 
 * Author: Josh Rands
 * Date: 1/19/2019
 ********************************/
#ifndef HELLO_MONITOR_H
#define HELLO_MONITOR_H

#include <set>

#include "routing_protocol.h"
#include "hello_defines.h"

class HelloMonitor : public Port
{
public:
    HelloMonitor(int portId, AdhocRoutingProtocol* routing) : Port(portId, routing) { m_parentIp = routing->getIp(); m_active = true; }
    virtual ~HelloMonitor() {}

    /**
     * @brief This function is called by adhocRouting to give the port its data
     * 
     * @param data char array of data
     * @param length the length of the data
     */
    void handlePacket(char* data, int length, IP_ADDR source) override;

    // Initiate sending hello messages
    void sendHellos(uint64_t duration_ms);

    bool isActive() { helloMux.lock(); return m_active; helloMux.unlock(); }

    // receive a hello message from a specific node 
    void receiveHelloMessage(IP_ADDR nodeIp);

protected:
    map<IP_ADDR, uint64_t> m_neighborDetectionTimes;

    IP_ADDR m_parentIp;
    bool m_active;
    mutex helloMux;

    // 1. Update neighbors of routing protocol
    // 2. Clear neighbors for next time period 
    // 3. Wait 1/2 hello interval
    // 4. Broadcast hello message  
    // 5. Update neighbors from received hellos 
    // 6. Wait 1/2 hello interval
    // 7. Repeat
    void _updateNeighbors(uint64_t remaining_time_ms);

    // broadcast a hello message so neighbors can add 
    void _broadcastHelloMessage();

    /**
     * @brief waits for a predetermined interval
     * 
     * @param duration_ms the duration to wait for in milliseconds
     * @return true if completed successfully
     * @return false otherwise
     */
    virtual bool _sleep(uint64_t duration_ms) = 0;

    // abstract function for getting the current time 
    /**
     * @brief Returns the current time in milliseconds
     * 
     * @return uint64_t current time in milliseconds
     */
    virtual uint64_t _getCurrentTimeMS() = 0;
};

class HelloTest : public HelloMonitor
{
public: 
    HelloTest(int portId, AdhocRoutingProtocol* routing) : HelloMonitor(portId, routing) { }

protected:
    bool _sleep(uint64_t duration_ms) override;

    uint64_t _getCurrentTimeMS() override;

private:
    // current time in milliseconds
    uint64_t m_clockMS;

};

void dispatchHello(HelloTest* hello, uint64_t duration);

#endif
