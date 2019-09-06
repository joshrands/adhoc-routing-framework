/*********************************
 * aodv.h
 *
 * Custom c++ implementation of the aodv routing protocol. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

#include "RoutingProtocol.h"

class AODV : RoutingProtocol
{
public:
	// default constructor 
	AODV();

	// TODO: Create aodv packet for each message? Only needs to be couple of bits.  

	// RREQ - Route Request 
	// initiating RREQ enters state of waiting for RREP
	void initiateRREQ(const IP_ADDR dest);
	// forward RREQ enters state of maybe receiving RREP
	void forwardRREQ(const IP_ADDR dest);

	// RREP - Route Reply
	// add this entry to routing able 
	void initiateRREP(const IP_ADDR dest);
	void forwardRREP(const IP_ADDR dest);

	// RERR - Route Error
	void initiateRERR(const IP_ADDR dest);
	void forwardRERR(const IP_ADDR dest);

private:

};

