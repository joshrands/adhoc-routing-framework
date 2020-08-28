#ifndef PORT_H
#define PORT_H

#include "routing_protocol.h"

class AdhocRoutingProtocol;

class Port{
private:
    void _init();
    int portId; // The port number messages will be received and sent from

protected:
    AdhocRoutingProtocol* routingProtocol; // AdHocRouting member

public:
    // Constructor
    /**
     * @brief Construct a new Port object
     * portId -> -1
     * adhocRouting -> null
     */
    Port();
    /**
     * @brief Construct a new Port object
     * 
     * @param portId 
     * adhocRouting -> null
     */
    Port(int portId);
    /**
     * @brief Construct a new Port object
     * 
     * @param portId the port number 
     * @param adhocRouting the routing protocol
     */
    Port(int portId, AdhocRoutingProtocol* adhocRouting);
    
    // Destructor
    virtual ~Port(){}

    // Functions
    /**
     * @brief This function is called by adhocRouting to give the port its data
     * 
     * @param data char array of data
     * @param length the length of the data
     */
    virtual void handlePacket(char* data, int length, IP_ADDR source) = 0;

    // Getters + Setters
    /**
     * @brief Set the Routing Protocol object
     * 
     * @param AdhocRoutingProtocol
     */
    void setRoutingProtocol(AdhocRoutingProtocol* routingProtocol);
    /**
     * @brief Set the Port Id object
     * 
     * @param portId the port number
     */
    void setPortId(int portId);
    /**
     * @brief Get the Port Id object
     * 
     * @return int the port number
     */
    int getPortId();
};

#endif // PORT_H
