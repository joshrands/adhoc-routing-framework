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
	
}

// **************************************************************************************
// * main()
// **************************************************************************************
int main (int argc, char *argv[]) 
{	
	DEBUG << "Running tests..." << ENDL;	

	test_test();
	test_routing_table();

	DEBUG << "Tests complete." << ENDL;

	return 0;
}
