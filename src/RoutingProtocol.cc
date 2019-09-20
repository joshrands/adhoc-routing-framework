#include "RoutingProtocol.h"

#define ROUTE_DEBUG		0

#include <fstream>

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
 
  //cout << ipStr << " = " << ip << endl;
   
  return ip;
}

RoutingTable::RoutingTable()
{
	if (ROUTE_DEBUG)	
		cout << "Routing table created" << endl;
}

RoutingTable::~RoutingTable()
{
	this->table.empty();
}

IP_ADDR RoutingTable::getNextHop(const IP_ADDR dest)
{
	IP_ADDR nextHop;

	if (this->table.count(dest))
	{
//		cout << "Entry exists." << endl;
		nextHop = table[dest].nextHop;
	}
	else
	{
//		cout << "Table entry does not exist." << endl;
		nextHop = 0;
	}

	return nextHop;
}

void RoutingTable::updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop)
{
	// check if this entry exists 
	if (this->table.count(dest))
	{
		// entry exists, update existing 
		this->table[dest].nextHop = nextHop;
		this->table[dest].ttl = DEFAULT_TTL;
	}
	else
	{
		// no entry, create new 
		TableInfo info; 
		info.dest = dest;
		info.nextHop = nextHop;
		info.ttl = DEFAULT_TTL;
	
		this->table[dest] = info;
	}
}

RoutingProtocol::RoutingProtocol()
{

}


