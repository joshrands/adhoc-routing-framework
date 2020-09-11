#ifndef AODV_H
#define AODV_H

/*********************************
 * aodv.h
 *
 * Custom c++ implementation of the aodv routing protocol. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

/* aodv includes */
#include "aodv_routing_table.h"
#include "aodv_rreq.h"
#include "aodv_rrep.h"
#include "aodv_rerr.h"
#include "aodv_params.h"
#include "packet_buffer.h"
#include "adhoc_structs.h"

#include "routing_protocol.h"

#include <vector>
#include <functional>
#include <queue>

class AODV : public AdhocRoutingProtocol
{
public:
	// default constructor 
	AODV();
	AODV(const char* ip);
	AODV(IP_ADDR ip);
	~AODV();

	/**
     * @brief Send a packet to a given ip address using a specified port
     * 
     * @param portId the port number to use
     * @param packet the packet to send
     * @param length the length of the packet
     * @param dest the destination ip address
     * @param origIP IP address where the packet was from
     */
	//TODO: Actually make this return false on a failure, currently is a silent failure
    virtual bool sendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP = -1) override;
	
	// Network Monitoring
	// attempt to repair the link and then send the packet to its destination
	virtual void repairLink(int port, IP_ADDR brokenLink, IP_ADDR finalDest, char* buffer, int length, IP_ADDR origIP);
	// attempt to repair the link
	virtual bool attemptLocalRepair(IP_ADDR brokenLink, IP_ADDR finalDest);

	// output the current contents of the routing table 
	void logRoutingTable();
	// ask if there is a direct link to another node's IP
	bool directLink(IP_ADDR toIP);

	// get the routing table 
	AODVRoutingTable* getTable() { return (AODVRoutingTable*) m_pRoutingTable; } 
	// returns the available bandwidth
    int getAvailableBandwidthBits() { return m_availableBandwidthBits; }
	// sets the available bandwidth for the model
    void setAvailableBandwidthBits(int availableBandwidth) { this->m_availableBandwidthBits = availableBandwidth; }

protected:
	// node sequence number. MUST increment on a route discovery
	uint32_t sequenceNum;
	// node rreq id. Incremented by one during route discovery
	uint32_t rreqID;
	// aodv routing table
//	AODVRoutingTable* m_aodvTable;
	// current packet id index
	uint32_t packetIdCount;	
	// map of rreq ids and their corresponding packet to be sent once the route is established
	// TODO: Update unit-testing
	map<IP_ADDR, queue<BufferedPacket>> rreqPacketBuffer;
	// RREQ - Route Request 
	RREQHelper rreqHelper;
	// Packet buffer for packets waiting to be sent
	PacketBuffer m_oPacketBuffer;
	// broadcast rreq to all neighbors
	void _broadcastRREQBuffer(rreqPacket rreq);
	// make a decision on a received rreq packet using the rreq helper 
	void _handleRREQ(char* buffer, int length, IP_ADDR source);

	// RREP - Route Reply
	RREPHelper rrepHelper;
	// handle a received rrep message 
	void _handleRREP(char* buffer, int length, IP_ADDR source);

	// RERR - Route Error
	RERRHelper rerrHelper;
	// handle a received rerr message 
	void _handleRERR(char* buffer, int length, IP_ADDR source);

	// Functions
	/**
	 * @brief Takes in a packet and routes it in the network or to the desired port
	 * 
	 * @param portId the port id to send from/to
	 * @param buffer the data to be sent, needs to contain the AODV header
	 * @param length the length of that data
	 * @param source the ip address the packet was received from
	 */
	virtual void _handlePacket(int portId, char *buffer, int length, IP_ADDR source);
	/**
	 * @brief Takes in a packet and routes it in the network or to the desired port
	 * 
	 * @param port the port class to send from/to
	 * @param buffer the data to be sent, needs to contain the AODV header
	 * @param length the length of that data
	 * @param source the ip address the packet was received from
	 */
	void _handlePacket(Port* p, char *buffer, int length, IP_ADDR source);

	// handle data for AODV
	void _handleAODVPacket(char *buffer, int length, IP_ADDR source);

	// Send the data over a socket
	bool _socketSendPacket(Port* port, char *buffer, int length, IP_ADDR dest);

	// Virtual Functions
	virtual bool _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) = 0;
};

// retry the route request message if you don't receive one
void retryRouteRequestIfNoRREP(AODV* aodv, rreqPacket sendRREQ, int numberOfRetriesRemaining);



#endif
