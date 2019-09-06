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
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <math.h>

#define IP_ADDR			uint32_t

using namespace std;

IP_ADDR getIpFromString(string ipStr)
{
	IP_ADDR ip = 0;

  istringstream iss(ipStr);
  string num;

	int pow = 24;
  while (getline(iss, num, '.'))
  {
		// increment ip 
		ip += (stoi(num) << pow);
		pow -= 8;
  }
 
	cout << ipStr << " = " << ip << endl;
	 
	return ip;
}

/* Routing Table row class */
class TableInfo 
{
public:
	IP_ADDR dest; // target destination 
	IP_ADDR nextHop; // next hop 

	uint16_t ttl; // time to live for this table entry 
};


/* Routing Table class */
class RoutingTable
{
public:
	// default constructor 
	RoutingTable();

	const uint16_t DEFAULT_TTL = 1800; // 1800 seconds = 30 minutes 

	// returns the ip address of the next hop 
	// returns 0 if not in table 
	IP_ADDR getNextHop(const IP_ADDR dest);

	// adds an element to the table 
	void updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop);

private:
	// internal routing table maps destination to next hop 
	map<IP_ADDR, TableInfo> table;

};


/* Routing Protocol base class */
class RoutingProtocol
{
public: 
	// default constructor
	RoutingProtocol();

private:
	RoutingTable table;	

};

