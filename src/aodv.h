/*********************************
 * aodv.h
 *
 * Custom c++ implementation of the aodv routing protocol. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

#define AODV_DEBUG		0

/* aodv includes */
#include "aodv_routing_table.h"
#include "aodv_rreq.h"
#include "aodv_rrep.h"

struct RERR
{
	uint8_t type = 3;
	uint16_t flags;
	uint8_t destCount;

	uint32_t unreachableDestIP;
	uint32_t unreachableDestSeqNum;
};


class AODV : public RoutingProtocol
{
public:
	// default constructor 
	AODV();
	AODV(IP_ADDR ip);
	~AODV();

	// try to send data to a destination - the next hop is determined from the routing table  
	void sendPacketBuffer(char* buffer, int length, IP_ADDR finalDestination);

	static const int AODV_PORT = 654;
	// decode a received packet buffer from UPD port 654
	void decodeReceivedPacketBuffer(char* packet, int length, IP_ADDR source);

	// RREQ - Route Request 
	RREQHelper rreqHelper;
	// broadcast rreq to all neighbors
	void broadcastRREQBuffer(rreqPacket rreq);
	// make a decision on a received rreq packet using the rreq helper 
	void handleRREQ(char* buffer, int length, IP_ADDR source);

	// RREP - Route Reply
	RREPHelper rrepHelper;
	// handle a received rrep message 
	void handleRREP(char* buffer, int length, IP_ADDR source);

	// output the current contents of the routing table 
	void logRoutingTable();

	// RERR - Route Error
	// handle a received rerr message 
	void handleRERRBuffer(char* buffer, int length);
	// initiating RERR enters state of waiting for RREP
	RERR createRERR(const IP_ADDR dest);
	// forward RERR enters state of maybe receiving RREP
	void forwardRERR(const RERR receivedRERR);
	// convert rerr message to a char* buffer
	char* createRERRBuffer(const RERR rerr);
	// read a received rrep buffer
	RERR readRERRBuffer(char* buffer);

	// cast table to AODVRoutingTable
	AODVRoutingTable* getTable() { return (AODVRoutingTable*)(this->table);}
private:
	// node sequence number. MUST increment on a route discovery
	uint32_t sequenceNum;
	// node rreq id. Incremented by one during route discovery
	uint32_t rreqID;
};

