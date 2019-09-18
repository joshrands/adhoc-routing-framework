#include "aodv.h"

#include <assert.h>

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
	test_aodv_rreq();
}

void test_aodv_rreq_simple()
{
	AODV aodv(getIpFromString("192.168.0.20"));

	// test simple rreq 
	IP_ADDR dest = getIpFromString("192.168.0.21");

	rreqPacket rreq;
	rreq = aodv.rreqHelper.createRREQ(dest);

	assert(rreq.type == 0x01);
	assert(rreq.hopCount == 0);
	assert(rreq.rreqID == 1);
	assert(rreq.destIP == dest);
	assert(rreq.origIP == aodv.getIp());
	assert(rreq.origSeqNum == 1);	

	dest = getIpFromString("192.168.0.22");
	rreq = aodv.rreqHelper.createRREQ(dest);

	assert(rreq.type == 0x01);
	assert(rreq.hopCount == 0);
	assert(rreq.rreqID == 2);
	assert(rreq.destIP == dest);
	assert(rreq.origIP == aodv.getIp());
	assert(rreq.origSeqNum == 2);	

}

void test_aodv_rreq_forwarding()
{
	// test forwarding capabilities for rreq messages 
	// after forwarding, middle nodes should also have table entry for path back to originator 

	// 0 - 1 - 2 - 3 - 4

	IP_ADDR orig = getIpFromString("192.168.0.0");
	IP_ADDR mid1 = getIpFromString("192.168.0.1");
	IP_ADDR mid2 = getIpFromString("192.168.0.2");
	IP_ADDR mid3 = getIpFromString("192.168.0.3");
	IP_ADDR dest = getIpFromString("192.168.0.4");

	AODV aodv(orig);

	rreqPacket rreq = aodv.rreqHelper.createRREQ(dest);

	// Note: In practice, each node will have its own AODV. Here we recycle the orig AODV
	// rreq sent from orig to mid1 
	rreq = aodv.rreqHelper.createForwardRREQ(rreq, orig);
	// mid1->mid2
	rreq = aodv.rreqHelper.createForwardRREQ(rreq, mid1);
	// mid2->mid3
	// using mid3 for routing table update test... need AODV 
	AODV aodv3(mid3);
	rreq = aodv3.rreqHelper.createForwardRREQ(rreq, mid2);
	// mid3->dest
	// final hop count increment... 
	rreq = aodv.rreqHelper.createForwardRREQ(rreq, mid3);

	// Create RREP! 
	assert(rreq.hopCount == 4);
	// check routing table update 
	// for mid3 to send packet to orig, go through mid2
	assert(aodv3.getTable()->getNextHop(orig) == mid2);
	assert(aodv3.getTable()->getNextHop(orig) != mid1);
}

void test_aodv_rreq_buffer()
{
	IP_ADDR orig = getIpFromString("192.168.0.11");
	IP_ADDR dest = getIpFromString("192.168.0.21");

	AODV aodv(orig);

	rreqPacket rreq = aodv.rreqHelper.createRREQ(dest);
	char* buffer = (char*)(malloc(sizeof(rreq))); 
	buffer = aodv.rreqHelper.createRREQBuffer(rreq);

	rreqPacket receivedRREQ;
	receivedRREQ = aodv.rreqHelper.readRREQBuffer(buffer);

	assert(receivedRREQ.type == rreq.type);
	assert(receivedRREQ.origIP == rreq.origIP);
	cout << receivedRREQ.destIP << endl;
}

void test_aodv_rreq()
{
	test_aodv_rreq_simple();
	test_aodv_rreq_forwarding();
	test_aodv_rreq_buffer();
}
