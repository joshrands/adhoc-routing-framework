#include "aodv.h"
#include "send_packet.h"

#include <assert.h>
#include "string.h"

// Very cheap testing framework

void test_test();
void test_routing_table();
void test_aodv();
void test_aodv_rreq();

int main (int argc, char *argv[]) 
{	
	cout << "Running tests..." << endl;	

	test_test();
	test_routing_table();
	test_aodv();

	cout << "TESTS COMPLETE." << endl;

	return 0;
}

void test_test()
{
	// this test tests tests
	assert(true == true);
	cout << "Test test complete." << endl;
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

	cout << "Test routing table complete." << endl;
}

void test_aodv()
{
	test_aodv_rreq();
}

void test_aodv_rreq_simple()
{
	// create an aodv routing object for ip .20
	AODVTest aodv(getIpFromString("192.168.0.20"));

	// test simple rreq 
	IP_ADDR dest = getIpFromString("192.168.0.21");

	// create a rreq packet to destination
	rreqPacket rreq = aodv.rreqHelper.createRREQ(dest);

	assert(rreq.type == 0x01);
	assert(rreq.hopCount == 0);
	assert(rreq.rreqID == 1);
	assert(rreq.destIP == dest);
	assert(rreq.origIP == aodv.getIp());
	assert(rreq.origSeqNum == 1);	

	// try a rreq for a different destination 
	dest = getIpFromString("192.168.0.22");
	rreq = aodv.rreqHelper.createRREQ(dest);

	assert(rreq.type == 0x01);
	assert(rreq.hopCount == 0);
	assert(rreq.rreqID == 2);
	assert(rreq.destIP == dest);
	assert(rreq.origIP == aodv.getIp());
	assert(rreq.origSeqNum == 2);	

	cout << "Test aodv rreq simple complete." << endl;
}

void test_aodv_rreq_forwarding()
{
	// test forwarding capabilities for rreq messages 
	// after forwarding, middle nodes should also have table entry for path back to originator 

	// 0 - 1 - 2 - 3 - 4

	AODVTest node0 = AODVTest(getIpFromString("192.168.0.0"));
	AODVTest node1 = AODVTest(getIpFromString("192.168.0.1"));
	AODVTest node2 = AODVTest(getIpFromString("192.168.0.2"));
	AODVTest node3 = AODVTest(getIpFromString("192.168.0.3"));
	AODVTest node4 = AODVTest(getIpFromString("192.168.0.4"));

	// create a rreq from node 0 to node 4
	rreqPacket rreq = node0.rreqHelper.createRREQ(node4.getIp());

	// rreq received by node1 from node0 
	rreq = node1.rreqHelper.createForwardRREQ(rreq, node0.getIp());
	// rreq received by node2 from node1
	rreq = node2.rreqHelper.createForwardRREQ(rreq, node1.getIp());
	// req received by node3 from node2
	rreq = node3.rreqHelper.createForwardRREQ(rreq, node2.getIp());
	// final hop count increment... 
	rreq = node4.rreqHelper.createForwardRREQ(rreq, node3.getIp());

	// Create RREP! 

	assert(rreq.hopCount == 4);

	// check routing table update 
	// for node3 to send packet to node0, go through node2
	assert(node3.getTable()->getNextHop(node0.getIp()) == node2.getIp());
	assert(node3.getTable()->getNextHop(node0.getIp()) != node1.getIp());

	cout << "Test aodv rreq forwarding complete." << endl;
}

void test_aodv_rreq_buffer()
{
	IP_ADDR orig = getIpFromString("192.168.0.11");
	IP_ADDR dest = getIpFromString("192.168.0.21");

	AODVTest aodv(orig);

	rreqPacket rreq = aodv.rreqHelper.createRREQ(dest);
	char* buffer = (char*)(malloc(sizeof(rreq))); 
	buffer = aodv.rreqHelper.createRREQBuffer(rreq);

	rreqPacket receivedRREQ;
	receivedRREQ = aodv.rreqHelper.readRREQBuffer(buffer);

	assert(receivedRREQ.type == rreq.type);
	assert(receivedRREQ.origIP == rreq.origIP);

	cout << "Test aodv rreq buffer complete." << endl;
}

void test_aodv_rreq_to_rrep()
{
	// simple test to prevent a simple loop 
	// 0 <---> 1 <---> 2 ---> 3
	AODVTest node0(getIpFromString("192.168.1.0"));
	AODVTest node1(getIpFromString("192.168.1.1"));
	AODVTest node2(getIpFromString("192.168.1.2"));
	AODVTest node3(getIpFromString("192.168.1.3"));

	// set up neighbors
	node0.addNeighbor(&node1);

	node1.addNeighbor(&node2);
	node1.addNeighbor(&node0);

	node2.addNeighbor(&node3);
	node2.addNeighbor(&node1);

	node3.addNeighbor(&node2);

	// send a packet from node 0 to node 3
	string msg = "Hello node 3! From node 0";
	int length = msg.length();
	char* buffer = (char*)(malloc(length));
	for (int i = 0; i < length; i++)
		buffer[i] = msg.at(i);

	node0.sendPacket(buffer, length, node3.getIp());	

	node0.sendPacket(buffer, length, node3.getIp());	
	delete buffer;

	cout << getStringFromIp(AODVTest::lastNode) << endl;
	
	node0.logRoutingTable();
	node1.logRoutingTable();
	node2.logRoutingTable();
	node3.logRoutingTable();

	cout << "Test aodv rreq to rrep complete." << endl;
}

void test_aodv_rreq()
{
	test_aodv_rreq_simple();
	test_aodv_rreq_forwarding();
	test_aodv_rreq_buffer();
	test_aodv_rreq_to_rrep();
}
