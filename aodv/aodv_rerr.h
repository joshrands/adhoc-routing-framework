/*********************************
 * aodv_rerr.h
 *
 * Custom c++ implementation of the aodv rerr message 
 *
 * 
 A node initiates processing for a RERR message in three situations:

   (i)       if it detects a link break for the next hop of an active
             route in its routing table while transmitting data (and
             route repair, if attempted, was unsuccessful), or

   (ii)      if it gets a data packet destined to a node for which it
             does not have an active route and is not repairing (if
             using local repair), or

   (iii)     if it receives a RERR from a neighbor for one or more
             active routes.
 *  
 * Author: Josh Rands
 * Date: 9/23/2019
 ********************************/

#define RERR_DEBUG      1

#include "defines.h"
#include "aodv_packets.h"
#include "aodv_routing_table.h"

class RERRHelper
{
public: 
    // default constructor
    RERRHelper();
	RERRHelper(IP_ADDR ip, AODVRoutingTable* table, uint32_t* seqNum);

	void setIp(const IP_ADDR ip) { this->m_ip = ip; }
	void setRoutingTable(AODVRoutingTable* table) { this->m_pTable = table; }
	void setSequenceNum(uint32_t* seqNum) { this->m_pSequenceNum = seqNum; }

    // was this node part of the routing table? 
    bool shouldGenerateRERRFromBrokenLink(IP_ADDR brokenLink);
    // attempt a local repair - possibly from network monitoring information? 
    bool attemptLocalRepair(IP_ADDR brokenLink);

    // create a rerr packet 
	rerrPacket createRERR(const IP_ADDR dest);
	// forward RERR enters state of maybe receiving RREP
	void forwardRERR(const rerrPacket receivedRERR);
	// convert rerr message to a char* buffer
	static char* createRERRBuffer(const rerrPacket rerr);
	// read a received rrep buffer
	static rerrPacket readRERRBuffer(char* buffer);

private:
    IP_ADDR m_ip;
    AODVRoutingTable* m_pTable;
	uint32_t* m_pSequenceNum;
};
