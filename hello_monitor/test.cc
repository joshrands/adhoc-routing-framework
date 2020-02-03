#include "hello_monitor.h"

#include "../aodv/aodv.h"
#include "../aodv/aodv_test.h"
#include "../defines.h"

#include <assert.h>
#include <cmath>
#include <iostream>
#include <thread>

#include "string.h"

using namespace std;

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string END = "\033[0m\n";

void test(bool condition, string desc)
{
	if (condition)
		cout << GREEN << "[PASS]: " << desc << END;
	else 
		cout << RED << "[FAIL]: " << desc << END;
}

// Very cheap testing framework
void test_test();
void test_hello();

int main (int argc, char *argv[]) 
{	
	cout << "[TESTS]: Running tests..." << endl;	

	test_test();
	test_hello();

	cout << "[TESTS]: TESTS COMPLETE." << endl;

	return 0;
}

void test_test()
{
	// this test tests tests
	test(true == true, "true == true");
}

void test_hello()
{
	// Try to send a packet without HELLO (linkExists should return false)
	AODVTest aodv1(getIpFromString("192.168.0.1"));
	AODVTest aodv2(getIpFromString("192.168.0.2"));

	aodv1.addPhysicalNeighborOnly(&aodv2);
	aodv2.addPhysicalNeighborOnly(&aodv1);

	// Add print port for printing received packets 
	Port* printPort = new  PrintPort(8080);
	aodv1.addPort(printPort);
	aodv2.addPort(printPort);

	test(aodv1.isNeighbor(&aodv2) == false, "Nodes are not neighbors because no monitoring");
	test(aodv2.isNeighbor(&aodv1) == false, "Nodes are not neighbors because no monitoring");

	HelloTest hello1(HELLO_PORT, &aodv1);
	HelloTest hello2(HELLO_PORT, &aodv2);

	aodv1.addPort(&hello1);
	aodv2.addPort(&hello2);

	thread helloThread1(dispatchHello, &hello1, HelloNeighbors::HELLO_INTERVAL_MS*2);
	helloThread1.join();
	thread helloThread2(dispatchHello, &hello2, HelloNeighbors::HELLO_INTERVAL_MS*2);
	helloThread2.join();

	this_thread::sleep_for(chrono::milliseconds(HelloNeighbors::HELLO_INTERVAL_MS*2));

	globalMux.lock();
	test(aodv2.isNeighbor(&aodv1) == true, "Nodes are neighbors because monitoring!");
	test(aodv1.isNeighbor(&aodv2) == true, "Nodes are neighbors because monitoring!");
	globalMux.unlock();
}
