// ********************************************************
// * The boost log facility makes setting log levels easy.
// ********************************************************
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>

// ********************************************************
// * These don't really provide any improved functionality,
// * but IMHO they make the code more readable.
// ********************************************************
#define TRACE BOOST_LOG_TRIVIAL(trace) 
#define DEBUG BOOST_LOG_TRIVIAL(debug)  
#define INFO BOOST_LOG_TRIVIAL(info)  
#define WARNING BOOST_LOG_TRIVIAL(warning) 
#define ERROR BOOST_LOG_TRIVIAL(error) 
#define FATAL BOOST_LOG_TRIVIAL(fatal)  
#define ENDL  " (" << __FILE__ << ":" << __LINE__ << ")"

#include "aodv.h"

// **************************************************************************************
// * Very cheap testing framework
// **************************************************************************************

void test_test()
{
	assert(true == true);
}

void test_routing_table()
{
	RoutingTable table;

	// add a test entry
	IP_ADDR dest = getIpFromString("192.168.0.20");
	IP_ADDR nextHop = getIpFromString("192.168.0.22");
	
	assert(0 == table.getNextHop(dest));

	// haven't updated the routing table so shouldn't be the next hop yet...
	assert(nextHop != table.getNextHop(dest));

	table.updateTableEntry(dest, nextHop);
	assert(nextHop == table.getNextHop(dest));

	nextHop = getIpFromString("192.168.0.11");
	table.updateTableEntry(dest, nextHop);
	// test that the table entry changed 
	assert(nextHop == table.getNextHop(dest));
	assert(getIpFromString("192.168.0.22") != table.getNextHop(dest));
}

void test_aodv()
{
	AODV aodv;
}

// **************************************************************************************
// * main()
// **************************************************************************************
int main (int argc, char *argv[]) 
{	
	DEBUG << "Running tests..." << ENDL;	

	test_test();
	test_routing_table();
	test_aodv();

	DEBUG << "Tests complete." << ENDL;

	return 0;
}
