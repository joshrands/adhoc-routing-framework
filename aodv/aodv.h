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

#include "../adhoc/send_packet.h"
#include "../adhoc/routing_protocol.h"

#include <vector>
#include <functional>
#include <queue>

struct packet
{
	char* buffer;
	uint32_t length;
	uint32_t id;
	uint16_t ttl;
};

class AODV : public RoutingProtocol
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
     * @param dest 
     * @param origIP 
     */
    void sendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP = -1);

	// output the current contents of the routing table 
	void logRoutingTable();

	// get the routing table 
	AODVRoutingTable* getTable() { return m_aodvTable; } 

	// Virtual functions
    /**
     * @brief Handles the receiving or processing of all packets
     * @brief when implementing this should query each of the sockets corresponding to each port
     * @brief and then "give" the data to each port
     * TODO: By creating a Socket base class we could implement this code and avoid the above req. 
     * 
     */
    virtual void handlePackets() = 0;

	// Network Monitoring
	// attempt to repair the link and then send the packet to its destination
	virtual void repairLink(IP_ADDR brokenLink, IP_ADDR finalDest, char* buffer, int length, IP_ADDR origIP, int port);
	// attempt to repair the link
	virtual bool attemptLocalRepair(IP_ADDR brokenLink, IP_ADDR finalDest);
	// get the next door neighbors in the network
	virtual void getOneHopNeighbors();
	// as there a link between this node and dest? 
	virtual bool linkExists(IP_ADDR dest);

protected:
	// node sequence number. MUST increment on a route discovery
	uint32_t sequenceNum;
	// node rreq id. Incremented by one during route discovery
	uint32_t rreqID;
	// aodv routing table
	AODVRoutingTable* m_aodvTable;
	// current packet id index
	uint32_t packetIdCount;	
	// map of rreq ids and their corresponding packet to be sent once the route is established
	// TODO: Fix up for port stuff
	map<IP_ADDR, queue<pair<char*, int>>> rreqPacketBuffer;

	// Functions
	// handle received data. If not in routing table, attempt local fix and then RERR 
	void _routePacket(char *buffer, int length, IP_ADDR source, int port);
	void _routePacket(char *buffer, int length, IP_ADDR source, Port* p);
	// handle data for AODV
	void _handleAODVPacket(char *buffer, int length, IP_ADDR source);

	// Send the data over a socket
	int _socketSendPacket(char *buffer, int length, IP_ADDR dest, Port* p);

	// Virtual Functions
	virtual int _socketSendPacket(char *buffer, int length, IP_ADDR dest, int port) = 0;

private:
	// RREQ - Route Request 
	RREQHelper rreqHelper;
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
};

/* AODVTest class
 * For testing AODV with unit tests. */
class AODVTest : public AODV
{
public: 	
	// debugging values 
	static int globalPacketCount;
	static IP_ADDR lastNode; 
	static IP_ADDR lastReceive;

	AODVTest(IP_ADDR ip) : AODV(ip) {}
	AODVTest(const char* ip) : AODV(ip) {}
	~AODVTest() { m_physicalNeighbors.clear(); }

	int socketSendPacket(char *buffer, int length, IP_ADDR dest, int port);

	// Network Monitoring 

	// add/remove node to neighbor list
	void addNeighbor(AODVTest* node);
	void removeNeighbor(AODVTest* node);

	// return true if node is neighbor
	bool isNeighbor(AODVTest node);

	// see if packet was put in queue
	bool packetInRreqBuffer(IP_ADDR dest);

private:
	vector<AODVTest*> m_physicalNeighbors;

};

class AODVMonitorTest : public AODVTest 
{
public:
	int socketSendPacket(char *buffer, int length, IP_ADDR dest, int port) override;

};

// retry the route request message if you don't receive one
void retryRouteRequestIfNoRREP(AODV* aodv, rreqPacket sendRREQ, int numberOfRetriesRemaining);

#endif