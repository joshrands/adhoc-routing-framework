/*********************************
 * hello_monitor.h
 *
 * C++ implementation of periodic HELLO messages to determine current neighbors 
 * 
 * Author: Josh Rands
 * Date: 1/19/2019
 ********************************/

#include "routing_protocol.h"
#include "defines.h"
#include <set>

#define HELLO_DEBUG     DEBUG && 1

class HelloMonitor : public Port
{
public:
    static int NEIGHBOR_TTL_MS;

    HelloMonitor(int portId, RoutingProtocol* routing) : Port(portId, routing) { m_parentIp = routing->getIp(); m_active = true; }
    ~HelloMonitor();

    /**
     * @brief This function is called by adhocRouting to give the port its data
     * 
     * @param data char array of data
     * @param length the length of the data
     */
    void handlePacket(char* data, int length, IP_ADDR source) override;

    static int HELLO_INTERVAL_MS;

    // Initiate sending hello messages
    void sendHellos(int duration_ms);

    bool isActive() { helloMux.lock(); return m_active; helloMux.unlock(); }

    // receive a hello message from a specific node 
    void receiveHelloMessage(IP_ADDR nodeIp);

protected:
    map<IP_ADDR, uint32_t> m_neighborDetectionTimes;

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
    void _updateNeighbors(int remaining_time_ms);

    // broadcast a hello message so neighbors can add 
    void _broadcastHelloMessage();

    // virtual function for waiting a predetermined interval. This will be implemented different for hardware vs. simulation
    // returns TRUE once complete, FALSE otherwise
    virtual bool _sleep(int DURATION_MS) = 0;

    // abstract function for getting the current time 
    virtual uint32_t getCurrentTimeMS() = 0;
    // current time in milliseconds
    uint32_t m_clockMS;
};

class HelloTest : public HelloMonitor
{
public: 
    HelloTest(int portId, RoutingProtocol* routing) : HelloMonitor(portId, routing) { }

protected:
    bool _sleep(int DURATION_MS);

    uint32_t getCurrentTimeMS() override;

};

void dispatchHello(HelloTest* hello, int duration);
