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

class AODV : RoutingProtocol
{
public:
	// default constructor 
	AODV();

	// TODO: Create aodv packet for each message? Only needs to be couple of bits.  

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
	// add this entry to routing able 
	void initiateRREP(const IP_ADDR dest);
	void forwardRREP(const IP_ADDR dest);

	// RERR - Route Error
	void initiateRERR(const IP_ADDR dest);
	void forwardRERR(const IP_ADDR dest);

private:
	uint32_t sequenceNum;

};

