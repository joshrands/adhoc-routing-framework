/********************************
 * RoutingProtocol.h 
 * 
 * Simple routing protocol base class for implementation of more complex routing protocols.
 * Contains necessary building blocks for custom routing protocols. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

#include <map>

#define IP_ADDR			uint32_t

/* Routing Table class */
class RoutingTable
{
public:
	// default constructor 
	RoutingTable();

private:
	// internal routing table maps destination to next hop 
	std::map<IP_ADDR, IP_ADDR> table;

};


/* Routing Protocol base class */
class RoutingProtocol
{
public: 
    // default constructor
    RoutingProtocol();

private:

};

