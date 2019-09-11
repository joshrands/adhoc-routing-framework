/*********************************
 * aodv.h
 *
 * Custom c++ implementation of the aodv routing protocol. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

#include "RoutingProtocol.h"

struct RREQ
{
	uint8_t type;
	uint16_t flags;
	uint8_t hopCount;
	
	uint32_t rreqID;
	uint32_t destIP;
	uint32_t destSeqNum;
	uint32_t origIP;
	uint32_t origSeqNum;
};

struct RREP
{
	uint8_t type;
	uint16_t flags;
	uint8_t hopCount;

	uint32_t destIP;
	uint32_t destSeqNum;
	uint32_t origIP;
	uint32_t lifetime;
};

struct RERR
{
	uint8_t type;
	uint16_t flags;
	uint8_t destCount;

	uint32_t unreachableDestIP;
	uint32_t unreachableDestSeqNum;
};

class AODVInfo : public TableInfo
{
public:
	uint32_t destSequenceNumber;
};

class AODVRoutingTable : public RoutingTable
{
public:
	uint32_t getDestSequenceNumber(const IP_ADDR dest);
};

class AODV : public RoutingProtocol
{
public:
	// default constructor 
	AODV();

	// RREQ - Route Request 
	// initiating RREQ enters state of waiting for RREP
	RREQ createRREQ(const IP_ADDR dest);
	// forward RREQ enters state of maybe receiving RREP
	void forwardRREQ(const RREQ receivedRREQ);
	// convert rreq message to a char* buffer
	char* createRREQBuffer(const RREQ rreq);
	// read a received rreq buffer
	RREQ readRREQBuffer(char* buffer);

	// RREP - Route Reply
	// initiating RREP enters state of waiting for RREP
	RREP createRREP(const IP_ADDR dest);
	// forward RREP enters state of maybe receiving RREP
	void forwardRREP(const RREP receivedRREP);
	// convert rrep message to a char* buffer
	char* createRREPBuffer(const RREP rrep);
	// read a received rrep buffer
	RREP readRREPBuffer(char* buffer);

	// RERR - Route Error
	// initiating RERR enters state of waiting for RREP
	RERR createRERR(const IP_ADDR dest);
	// forward RERR enters state of maybe receiving RREP
	void forwardRERR(const RERR receivedRERR);
	// convert rerr message to a char* buffer
	char* createRERRBuffer(const RERR rerr);
	// read a received rrep buffer
	RERR readRERRBuffer(char* buffer);

private:
	// node sequence number. MUST increment on a route discovery
	uint32_t sequenceNum;
	// node rreq id. Incremented by one during route discovery
	uint32_t rreqID;
};

