#pragma once 

/*********************************
 * aodv_routing_table.h
 *
 * Custom c++ implementation of the aodv routing table  
 * 
 * Author: Josh Rands
 * Date: 9/18/2019
 ********************************/

// include for routing table 
// TODO: Pull routing table out of RoutingProtocol.h
#include "RoutingProtocol.h"
#include "aodv_packets.h"


class AODVInfo : public TableInfo
{
public:
	uint32_t destSequenceNumber;
	uint8_t hopCount;
	uint32_t lastRREQId;
};

class AODVRoutingTable
{
public:
	AODVRoutingTable();
	~AODVRoutingTable();

	const uint16_t DEFAULT_TTL = 1800; // 1800 seconds = 30 minutes 

	void updateAODVRoutingTableFromRREQ(rreqPacket* receivedRREQ, IP_ADDR sourceIP);
	void updateAODVRoutingTableFromRREP(rrepPacket* receivedRREP, IP_ADDR sourceIP);

	IP_ADDR getNextHop(const IP_ADDR dest);
	uint32_t getDestSequenceNumber(const IP_ADDR dest);
	uint8_t getDestHopCount(const IP_ADDR dest);
	uint32_t getLastRREQId(const IP_ADDR dest);
	void setDestSequenceNumber(const IP_ADDR dest, uint32_t destSeqNum);
	void setHopCount(const IP_ADDR dest, uint8_t hopCount);
	void setLastRREQId(const IP_ADDR dest, uint32_t lastRREQId);

	void updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop);

	// get the cost to send packets to this node 
	int getCostOfDest(const IP_ADDR dest);
	int getCostOfRREQ(const rreqPacket rreq);
	int getCostOfRREP(const rrepPacket rrep);

	map<IP_ADDR, AODVInfo> getInternalAODVTable() { return m_aodvTable; }

private:
	map<IP_ADDR, AODVInfo> m_aodvTable;
};