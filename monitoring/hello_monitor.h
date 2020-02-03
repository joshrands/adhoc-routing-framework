/*********************************
 * hello_monitor.h
 *
 * C++ implementation of periodic HELLO messages to determine current neighbors 
 * 
 * Author: Josh Rands
 * Date: 1/19/2019
 ********************************/

#include "RoutingProtocol.h"
#include <set>

class HelloNeighbors
{
public:
    HelloNeighbors(RoutingProtocol* routingProtocol);
    ~HelloNeighbors();

    // Partner routing protocol where neighbors will be assigned 
    // Also used for sending periodic hello messages 
    RoutingProtocol* m_routing;
    // port number for where to send HELLO messages 
    int m_port; 

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
    HelloTest(RoutingProtocol* routing) : HelloNeighbors(routing) { }

protected:
    bool _sleep(int DURATION_MS);

};
