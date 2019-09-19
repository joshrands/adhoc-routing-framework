#include "aodv.h"
#include "send_packet.h"

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
	// this test tests tests
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
	// create an aodv routing object for ip .20
	AODV aodv(getIpFromString("192.168.0.20"));

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
}

void test_aodv_rreq_forwarding()
{
	// test forwarding capabilities for rreq messages 
	// after forwarding, middle nodes should also have table entry for path back to originator 

	// 0 - 1 - 2 - 3 - 4

	AODV node0 = AODV(getIpFromString("192.168.0.0"));
	AODV node1 = AODV(getIpFromString("192.168.0.1"));
	AODV node2 = AODV(getIpFromString("192.168.0.2"));
	AODV node3 = AODV(getIpFromString("192.168.0.3"));
	AODV node4 = AODV(getIpFromString("192.168.0.4"));

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
//	cout << receivedRREQ.destIP << endl;
}

void test_aodv_rreq_loop_detection()
{
	// simple test to prevent a simple loop 
	// 0 <---> 1 <---> 2 ---> 3
	AODV node0 = AODV(getIpFromString("192.168.1.0"));
	AODV node1 = AODV(getIpFromString("192.168.1.1"));
	AODV node2 = AODV(getIpFromString("192.168.1.2"));
	AODV node3 = AODV(getIpFromString("192.168.1.3"));

	// generate a rreq from node 0 to node 3
	rreqPacket rreq = node0.rreqHelper.createRREQ(node3.getIp());

	// node 0 broadcasts rreq, which is received by node 1
	sendBuffer(RREQHelper::createRREQBuffer(rreq), sizeof(rreq), node0.getIp(), getIpFromString(BROADCAST));
	node1.decodeReceivedPacketBuffer(RREQHelper::createRREQBuffer(rreq), sizeof(rreq), node0.getIp());

	// node1 ip and packet count = 2 because node 0 sent one and then node 1 should have handled it and broadcasted
	assert(getLastSource() == node1.getIp());
	assert(getGlobalPacketCount() == 2);

	// node1 broadcasted a forward packet it received from node0, which should be received by node0 and node2
	rreq = node1.rreqHelper.createForwardRREQ(rreq, node0.getIp());
	node0.decodeReceivedPacketBuffer(RREQHelper::createRREQBuffer(rreq), sizeof(rreq), node1.getIp());
	node2.decodeReceivedPacketBuffer(RREQHelper::createRREQBuffer(rreq), sizeof(rreq), node1.getIp());

	// node 0 should have discarded the duplicate, but node 2 should have generated a forward rreq
	assert(getLastSource() == node2.getIp());
	assert(getGlobalPacketCount() == 3);

	// node 2 broadcasted a forward packet it received from node1, and it was received by node 1 and 3
	rreq = node2.rreqHelper.createForwardRREQ(rreq, node1.getIp());
	node1.decodeReceivedPacketBuffer(RREQHelper::createRREQBuffer(rreq), sizeof(rreq), node2.getIp());
	node3.decodeReceivedPacketBuffer(RREQHelper::createRREQBuffer(rreq), sizeof(rreq), node2.getIp());

	// node 1 and 3 should have both received the packet, but neither should have generated a forward rreq. 
	// node 1 should have discarded the duplicate, and node 3 should have generated and sent a rrep 
	// TODO: Update once rrep is implemented to reflect actual packet send
	assert(getLastSource() == node2.getIp());
	assert(getGlobalPacketCount() == 3);	
}

void test_aodv_rreq()
{
	test_aodv_rreq_simple();
	test_aodv_rreq_forwarding();
	test_aodv_rreq_buffer();
	test_aodv_rreq_loop_detection();
}
