/*********************************
 * hello_monitor.h
 *
 * C++ implementation of periodic HELLO messages to determine current neighbors 
 * 
 * Author: Josh Rands
 * Date: 1/19/2019
 ********************************/

#include "network_monitor.h"
#include "RoutingProtocol.h"

class HelloNeighbors : public NetworkMonitor
{
public:
    HelloNeighbors();
    HelloNeighbors(IP_ADDR parentIp);
    ~HelloNeighbors();

    // Partner routing protocol where neighbors will be assigned 
    // Also used for sending periodic hello messages 
    RoutingProtocol* m_routing;
    // port number for where to send HELLO messages 
    int m_port; 

    static int HELLO_INTERVAL_MS;

    // 1. Update neighbors of routing protocol
    // 2. Clear neighbors for next time period 
    // 3. Wait 1/2 hello interval
    // 4. Broadcast hello message  
    // 5. Update neighbors from received hellos 
    // 6. Wait 1/2 hello interval
    // 7. Repeat
    void updateNeighbors();

    // broadcast a hello message so neighbors can add 
    void broadcastHelloMessage();
    // receive a hello message from a specific node 
    void receiveHelloMessage(IP_ADDR nodeIp);

    // virtual function for waiting a predetermined interval. This will be implemented different for hardware vs. simulation
    // returns TRUE once complete, FALSE otherwise
    virtual bool sleep(int DURATION_MS) = 0;

protected:
    vector<IP_ADDR> m_neighbors;
    IP_ADDR m_parentIp;

};
