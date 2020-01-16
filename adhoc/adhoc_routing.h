#ifndef ADHOC_ROUTING_H
#define ADHOC_ROUTING_H

#include <vector>
#include "../aodv/RoutingProtocol.h"
#include "../monitoring/network_monitor.h"
#include "../port/port.h"

// class to combine routing and monitoring 
class AdHocRouting{
private:
    void _init();

protected:
    virtual void _buildPort(Port*) = 0;
    vector<Port*> ports;

public:
    // Constructor
    AdHocRouting();
    AdHocRouting(RoutingProtocol*);
    AdHocRouting(NetworkMonitor*);
    AdHocRouting(RoutingProtocol*, NetworkMonitor*);

    // Functions
    /**
     * @brief adds a port to the routing protocol and sets the ports adhocRouting member
     * 
     * @param p the port to add
     */
    void addPort(Port* p);
    
    /**
     * @brief removes a port from the routing protocol
     * 
     * @param p the port to remove
     */
    void removePort(Port* p);

    // Virtual Functions
    /**
     * @brief Send a packet to a given ip address using a specified port
     * 
     * @param p the port to use
     * @param data the data to send
     * @param length the length of the data
     * @param dest 
     * @param origIP 
     */
    virtual void sendPacket(Port* p, char* data, int length, IP_ADDR dest, IP_ADDR origIP = -1) = 0;
    /**
     * @brief Handles the receiving or processing of all packets
     * @brief when implementing this should query each of the sockets corresponding to each port
     * @brief and then "give" the data to each port
     * TODO: By creating a Socket base class we could implement this code and avoid the above req. 
     * 
     */
    virtual void handlePackets() = 0;

    RoutingProtocol* routing; // The routing protocol, sends and receives data within the network
    NetworkMonitor* monitor; // The network monitor to use, decides who are connected
};

#endif;
