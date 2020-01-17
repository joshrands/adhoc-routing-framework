#include <iostream>
#include <assert.h>
#include "string.h"

#include "adhoc/adhoc_routing.h"
#include "rem/rem.h"
#include "aodv/aodv.h"

#include <cmath>

// Very cheap testing framework
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

void test_test();
void test_adhoc();

int main (int argc, char *argv[]) 
{	
	cout << "[TESTS]: Running tests..." << endl;	

	test_test();
	test_adhoc();

	cout << "[TESTS]: TESTS COMPLETE." << endl;

	return 0;
}

void test_test()
{
	test(true == true, "true == true");
	// fail test... 
	test(true == false, "true == false??? This was supposed to fail :)");
}

void test_adhoc()
{
	cout << "[TESTS]: Testing complete ad hoc system..." << endl;

	IP_ADDR node1 = getIpFromString("192.168.0.1");
	// create network monitoring 
	REMTest rem1;
	// create a routing protocol 
	AODVTest aodv1(node1);

	// link monitoring and routing 
	rem1.routing = &aodv1;
	rem1.initialize(node1);

	// create adhoc routing object 
	AdhocRouting adhoc1;
	adhoc1.monitor = &rem1;
	adhoc1.routing = &aodv1;

	// do the same for a second simulated node 
	IP_ADDR node2 = getIpFromString("192.168.0.2");
	REMTest rem2; 
	AODVTest aodv2(node2); 

	rem2.routing = &aodv2;
	rem2.initialize(node2);

	AdhocRouting adhoc2;
	adhoc2.monitor = &rem2;
	adhoc2.routing = &aodv2;

	// connect the nodes! 
	aodv1.addNeighbor(&aodv2);
	aodv2.addNeighbor(&aodv1);

	// test neighbors should not be detected yet 
	test(rem1.isNodeOneHopNeighbor(node2) == false, "Node 2 should not be detected as a neighbor yet");
	test(rem2.isNodeOneHopNeighbor(node1) == false, "Node 1 should not be detected as a neighbor yet");

	// generate a battery model on node 1
	for (int i = 0; i < 3; i++)
	{
		rem1.updateLocalBatteryModel(rem1.getCurrentBatteryLevel());
		rem1.runClock();
		rem1.drainBattery();	

		rem2.updateLocalBatteryModel(rem2.getCurrentBatteryLevel());
		rem2.runClock();
		rem2.drainBattery();

		// send hello messages! 
		string msg = "Hello!";
		char* buffer = (char*)(malloc(msg.length()));
		memcpy(buffer, &(msg[0]), msg.length());
		char* buffer2 = (char*)(malloc(msg.length()));
		memcpy(buffer2, &(msg[0]), msg.length());

//		adhoc1.sendPacket(buffer, msg.length(), node2);
//		adhoc2.sendPacket(buffer, msg.length(), node1);

		// have to cheat with pair data for now... 
		// should use this function:
//		adhoc1.receivePacketWithPairData()
		// TODO: Fix this??? 
		pair_data pairFrom1;
		pairFrom1.pairIp = node1;
		pairFrom1.rss = -50;
		pair_data pairFrom2;
		pairFrom2.pairIp = node2;
		pairFrom2.rss = -50;

		// USE FAKE PORTS FOR NOW 
		adhoc1.receivePacketWithPairData(buffer, msg.length(), node2, 0, pairFrom2);
		adhoc2.receivePacketWithPairData(buffer2, msg.length(), node1, 0, pairFrom1);
	}

	// send all models. 
	pair_data pairFrom1;
	pairFrom1.pairIp = node1;
	pairFrom1.rss = -50;
	pair_data pairFrom2;
	pairFrom2.pairIp = node2;
	pairFrom2.rss = -50;

	// THIS SHOULD HAPPEN AUTOMATICALLY NORMALLY  
	REMModelPacket battery1 = rem1.localBatteryModel.createREMModelPacket();
	int length = sizeof(battery1);
	char* buffer = (char*)(malloc(length));
	memcpy(buffer, &(battery1), length);
	adhoc2.receivePacketWithPairData(buffer, length, node1, MONITOR_PORT, pairFrom1);

	REMModelPacket battery2 = rem2.localBatteryModel.createREMModelPacket();
	length = sizeof(battery2);
	char* buffer2 = (char*)(malloc(length));
	memcpy(buffer2, &(battery2), length);
	adhoc1.receivePacketWithPairData(buffer2, length, node2, MONITOR_PORT, pairFrom2);

	// test that the nodes actually built their own models...
	test(abs(rem1.getBatteryLevel() - rem1.getCurrentBatteryLevel()) < 0.01, "Node 1's own battery level: " + to_string(rem1.getBatteryLevel()) + " : " + to_string(rem1.getCurrentBatteryLevel()));
	test(abs(rem2.getBatteryLevel() - rem2.getCurrentBatteryLevel()) < 0.01, "Node 2's own battery level: " + to_string(rem2.getBatteryLevel()));

	// both nodes should have received each others models 
	// test neighbors should not be detected yet 
	test(rem1.isNodeOneHopNeighbor(node2) == true, "Node 2 should be detected as a neighbor");
	test(rem2.isNodeOneHopNeighbor(node1) == true, "Node 1 should be detected as a neighbor");

	test(abs(rem1.getBatteryLevel(node2) - rem2.getCurrentBatteryLevel()) < 0.01, "Node 1 assumed battery level of node 2: " + to_string(rem1.getBatteryLevel(node2)));
	test(abs(rem2.getBatteryLevel(node1) - rem1.getCurrentBatteryLevel()) < 0.01, "Node 2 assumed battery level of node 1: " + to_string(rem2.getBatteryLevel(node1)));

	cout << "[TESTS]: REM tests complete." << endl;
}
