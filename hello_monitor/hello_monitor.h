/*********************************
 * hello_monitor.h
 *
 * C++ implementation of periodic HELLO messages to determine current neighbors 
 * 
 * Author: Josh Rands
 * Date: 1/19/2019
 ********************************/

#include "../adhoc/routing_protocol.h"
#include "../defines.h"
#include <set>

#define HELLO_DEBUG     DEBUG && 1

class HelloNeighbors : public Port
{
public:
    HelloNeighbors(int portId, RoutingProtocol* routing) : Port(portId, routing) { }
    ~HelloNeighbors();

    /**
     * @brief This function is called by adhocRouting to give the port its data
     * 
     * @param data char array of data
     * @param length the length of the data
     */
    void handlePacket(char* data, int length, IP_ADDR source) override;

    static int HELLO_INTERVAL_MS;

    // Initiate sending hello messages
    void initializeHellos();

protected:
    set<IP_ADDR> m_neighbors;
    IP_ADDR m_parentIp;

    // 1. Update neighbors of routing protocol
    // 2. Clear neighbors for next time period 
    // 3. Wait 1/2 hello interval
    // 4. Broadcast hello message  
    // 5. Update neighbors from received hellos 
    // 6. Wait 1/2 hello interval
    // 7. Repeat
    void _updateNeighbors();

    // broadcast a hello message so neighbors can add 
    void _broadcastHelloMessage();
    // receive a hello message from a specific node 
    void _receiveHelloMessage(IP_ADDR nodeIp);

    // virtual function for waiting a predetermined interval. This will be implemented different for hardware vs. simulation
    // returns TRUE once complete, FALSE otherwise
    virtual bool _sleep(int DURATION_MS) = 0;

};

class HelloTest : public HelloNeighbors
{
public: 
    HelloTest(int portId, RoutingProtocol* routing) : HelloNeighbors(portId, routing) { }

protected:
    bool _sleep(int DURATION_MS);

};
