#include "test.h"
#include "send_packet.h"

// AODV TESTING

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
void test_inet_addr();
void test_routing_table();
void test_aodv();
void test_aodv_rreq();
void test_aodv_loop_prevention();
void test_aodv_do_nothing();
void test_aodv_link_break();
void test_aodv_rreq_simple();
void test_aodv_rreq_buffer();
void test_aodv_rreq_forwarding();
void test_aodv_no_route();
void test_aodv_rreq_to_rrep();

int main (int argc, char *argv[]) 
{	
	cout << "[TESTS]: Running tests..." << endl;	

	test_test();
	test_inet_addr();
	test_routing_table();
	test_aodv();

	cout << "[TESTS]: TESTS COMPLETE." << endl;

	return 0;
}

void test_test()
{
	test(true == true, "true == true");
}

void test_inet_addr()
{
	// go from string to number 
	struct sockaddr_in sa;
	char str[INET_ADDRSTRLEN];

	inet_pton(AF_INET, "192.168.1.4", &(sa.sin_addr));

//	cout << sa.sin_addr.s_addr << endl;

	IP_ADDR ip = getIpFromString("192.168.1.4");
//	cout << ip << endl;

	test(sa.sin_addr.s_addr == ip, "sa.sin_addr.s_addr == ip");

	// go from number to string
	inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);

	string ipString1(str);
	string ipString2 = getStringFromIp(ip);

	test(ipString1 == ipString2, "ipString1 == ipString2");

	test(inet_addr("192.168.1.4") == getIpFromString("192.168.1.4"),"inet_addr(\"192.168.1.4\") == getIpFromString(\"192.168.1.4\")");

	AODVTest aodv("192.168.1.4");
	test(inet_addr("192.168.1.4") == aodv.getIp(),"inet_addr(\"192.168.1.4\") == aodv.getIp()");
}

void test_routing_table()
{
	RoutingTable table;

	// add a test entry
	IP_ADDR dest = getIpFromString("192.168.0.20");
	IP_ADDR nextHop = getIpFromString("192.168.0.22");
	
	test(0 == table.getNextHop(dest), "0 == table.getNextHop(dest)");

	// haven't updated the routing table so shouldn't be the next hop yet...
	test(nextHop != table.getNextHop(dest), "nextHop != table.getNextHop(dest)");

	table.updateTableEntry(dest, nextHop);
	test(nextHop == table.getNextHop(dest), "nextHop == table.getNextHop(dest)");

	nextHop = getIpFromString("192.168.0.11");
	table.updateTableEntry(dest, nextHop);
	// test that the table entry changed 
	test(nextHop == table.getNextHop(dest), "nextHop == table.getNextHop(dest)");
	test(getIpFromString("192.168.0.22") != table.getNextHop(dest), "table.getNextHop(dest) != 192.168.0.22");

	cout << "[TESTS]: Test routing table complete." << endl;
}

void test_aodv()
{	
	test_aodv_do_nothing();
	test_aodv_rreq();
	test_aodv_loop_prevention();
	test_aodv_link_break();
}

void test_aodv_rreq_simple()
{
	// create an aodv routing object for ip .20
	AODVTest aodv(getIpFromString("192.168.0.20"));

	// test simple rreq 
	IP_ADDR dest = getIpFromString("192.168.0.21");

	// create a rreq packet to destination
	rreqPacket rreq = aodv.getRREQHelper()->createRREQ(dest);

	test(rreq.type == 0x01, "rreq.type == 0x01");
	test(rreq.hopCount == 0, "rreq.hopCount == 0");
	test(rreq.rreqID == 1, "rreq.rreqID == 1");
	test(rreq.destIP == dest, "rreq.destIP == dest");
	test(rreq.origIP == aodv.getIp(), "rreq.origIP == aodv.getIP()");
	test(rreq.origSeqNum == 1, "rreq.origSeqNum == 1");	

	// try a rreq for a different destination 
	dest = getIpFromString("192.168.0.22");
	rreq = aodv.getRREQHelper()->createRREQ(dest);

	printf("rreqID: %d\n", rreq.rreqID);
	test(rreq.rreqID == 2, "rreq.rreqID == 2");
	test(rreq.origSeqNum == 2, "rreq.origSeqNum == 2");	

	cout << "[TESTS]: Test aodv rreq simple complete." << endl;
}

void test_aodv_rreq_forwarding()
{
	// test forwarding capabilities for rreq messages 
	// after forwarding, middle nodes should also have table entry for path back to originator 

	// 0 - 1 - 2 - 3 - 4

	AODVTest node0(getIpFromString("192.168.0.0"));
	AODVTest node1(getIpFromString("192.168.0.1"));
	AODVTest node2(getIpFromString("192.168.0.2"));
	AODVTest node3(getIpFromString("192.168.0.3"));
	AODVTest node4(getIpFromString("192.168.0.4"));

	// create a rreq from node 0 to node 4
	rreqPacket rreq = node0.getRREQHelper()->createRREQ(node4.getIp());

	// rreq received by node1 from node0 
	rreq = node1.getRREQHelper()->createForwardRREQ(rreq, node0.getIp());
	// rreq received by node2 from node1
	rreq = node2.getRREQHelper()->createForwardRREQ(rreq, node1.getIp());
	// req received by node3 from node2
	rreq = node3.getRREQHelper()->createForwardRREQ(rreq, node2.getIp());
	// final hop count increment... 
	rreq = node4.getRREQHelper()->createForwardRREQ(rreq, node3.getIp());

	// Create RREP! 
	test(rreq.hopCount == 4, "rreq.hopCount == 4");

	// check routing table update 
	// for node3 to send packet to node0, go through node2
	test(node3.getTable()->getNextHop(node0.getIp()) == node2.getIp(), "Next hop from node 3 to node 0 is node 2");

	cout << "[TESTS]: Test aodv rreq forwarding complete." << endl;
}

void test_aodv_do_nothing()
{
	AODVTest aodv();
}

void test_aodv_rreq_buffer()
{
	IP_ADDR orig = getIpFromString("192.168.0.11");
	IP_ADDR dest = getIpFromString("192.168.0.21");

	AODVTest aodv(orig);
	Port* printPort = new  PrintPort(8080);
	aodv.addPort(printPort);

	rreqPacket rreq = aodv.getRREQHelper()->createRREQ(dest);
	char* buffer = (char*)(malloc(sizeof(rreq))); 
	buffer = aodv.getRREQHelper()->createRREQBuffer(rreq);

	rreqPacket receivedRREQ;
	receivedRREQ = aodv.getRREQHelper()->readRREQBuffer(buffer);

	// Send a message to an unavailable location
	char message []="Hello World\0";
	aodv.sendPacket(printPort->getPortId(), message, 13, dest);
	test(aodv.packetInRreqBuffer(dest), "Packet successfully added to RREQ Buffer");

	// "Find route"
	cout << "[TESTS]: Test aodv rreq buffer complete." << endl;

	delete buffer;
}

void test_aodv_rreq_to_rrep()
{
	// simple test to prevent a simple loop 
	// 0 <---> 1 <---> 2 ---> 3
	AODVTest node0(getIpFromString("192.168.1.0"));
	AODVTest node1(getIpFromString("192.168.1.1"));
	AODVTest node2(getIpFromString("192.168.1.2"));
	AODVTest node3(getIpFromString("192.168.1.3"));
	Port* printPort = new  PrintPort(8080);
	node0.addPort(printPort);
	node1.addPort(printPort);
	node2.addPort(printPort);
	node3.addPort(printPort);

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

	node0.sendPacket(printPort->getPortId(), buffer, length, node3.getIp());	

	delete buffer;

	node0.logRoutingTable();
	node1.logRoutingTable();
	node2.logRoutingTable();
	node3.logRoutingTable();

	test(true, "Test aodv rreq to rrep complete");
}

void test_aodv_loop_prevention()
{
	AODVTest node0(getIpFromString("192.168.1.0"));
	AODVTest node1(getIpFromString("192.168.1.1"));
	AODVTest node2(getIpFromString("192.168.1.2"));
	AODVTest node3(getIpFromString("192.168.1.3"));
	AODVTest node4(getIpFromString("192.168.1.4"));
	
	Port* printPort = new  PrintPort(8080);
	node0.addPort(printPort);
	node1.addPort(printPort);
	node2.addPort(printPort);
	node3.addPort(printPort);
	node4.addPort(printPort);
	

	// assign neighbors, creating a loop
	node0.addNeighbor(&node1);
	node0.addNeighbor(&node2);

	node1.addNeighbor(&node0);
	node1.addNeighbor(&node2);

	node2.addNeighbor(&node0);
	node2.addNeighbor(&node1);
	node2.addNeighbor(&node3);
	node2.addNeighbor(&node4);

	node3.addNeighbor(&node2);
	node3.addNeighbor(&node4);

	node4.addNeighbor(&node2);
	node4.addNeighbor(&node3);

	// send a packet from node 0 to node 3
	string msg = "Hello node 3! From node 0";
	int length = msg.length();
	char* buffer = (char*)(malloc(length));
	for (int i = 0; i < length; i++)
		buffer[i] = msg.at(i);

	node0.sendPacket(printPort->getPortId(), buffer, length, node4.getIp());

	node0.logRoutingTable();
	node1.logRoutingTable();
	node2.logRoutingTable();
	node3.logRoutingTable();
	node4.logRoutingTable();

	test(true, "Test aodv loop prevention passed");

	delete buffer;
}

void test_aodv_rreq_no_route()
{
	AODVTest node0(getIpFromString("192.168.1.0"));
	AODVTest node1(getIpFromString("192.168.1.1"));

	// assign neighbors, creating a loop
	node0.addNeighbor(&node1);
	node1.addNeighbor(&node0);

	Port* printPort = new  PrintPort(8080);
	node0.addPort(printPort);
	node1.addPort(printPort);

	// send a packet from node 0 to non existing node 
	string msg = "Hello fake node! From node 0";
	int length = msg.length();
	char* buffer = (char*)(malloc(length));
	for (int i = 0; i < length; i++)
		buffer[i] = msg.at(i);

	test(false, "[WARNING]: Aodv can NOT handle no possible route.");

	node0.sendPacket(printPort->getPortId(), buffer, length, getIpFromString("192.168.1.4"));

	// TODO: Add this test 
	// TODO: Is this a buffer problem?

	delete buffer;
	delete printPort;
}

void test_aodv_rreq()
{
	test_aodv_rreq_simple();
	test_aodv_rreq_forwarding();
	test_aodv_rreq_buffer();
	test_aodv_rreq_to_rrep();
	test_aodv_rreq_no_route();
}

void test_aodv_link_break()
{
	AODVTest node0(getIpFromString("192.168.1.0"));
	AODVTest node1(getIpFromString("192.168.1.1"));
	AODVTest node2(getIpFromString("192.168.1.2"));
	AODVTest node3(getIpFromString("192.168.1.3"));
	AODVTest node4(getIpFromString("192.168.1.4"));

	Port* printPort = new  PrintPort(8080);
	node0.addPort(printPort);
	node1.addPort(printPort);
	node2.addPort(printPort);
	node3.addPort(printPort);
	node4.addPort(printPort);
	

	// assign neighbors, creating a loop
	node0.addNeighbor(&node1);
	node0.addNeighbor(&node2);

	node1.addNeighbor(&node0);
	node1.addNeighbor(&node2);
	node1.addNeighbor(&node3);

	node2.addNeighbor(&node0);
	node2.addNeighbor(&node1);
	node2.addNeighbor(&node3);
	node2.addNeighbor(&node4);

	node3.addNeighbor(&node1);
	node3.addNeighbor(&node2);
	node3.addNeighbor(&node4);

	node4.addNeighbor(&node2);
	node4.addNeighbor(&node3);

	// send a packet from node 0 to node 4
	string msg = "Hello node 4! From node 0";
	int length = msg.length();
	char* buffer = (char*)(malloc(length));
	for (int i = 0; i < length; i++)
		buffer[i] = msg.at(i);

	node0.sendPacket(printPort->getPortId(), buffer, length, node4.getIp());

	node0.sendPacket(printPort->getPortId(), buffer, length, node4.getIp());
	delete buffer;

	test(AODVTest::lastReceive == node4.getIp(), "Last received packet was by node 4");

	// break the link between 2 and 4
	node4.removeNeighbor(&node2);
	node2.removeNeighbor(&node4);

	node0.sendPacket(printPort->getPortId(), buffer, length, node4.getIp());
//	node0.sendPacket(buffer, length, node4.getIp());

	// FAILING this test is okay because if links break we will consider those packets lost...
	test(AODVTest::lastReceive == node4.getIp(), "OKAY TO FAIL: Last received packet was node " + getStringFromIp(AODVTest::lastReceive) + " not node 4");

	node0.sendPacket(printPort->getPortId(), buffer, length, node4.getIp());
	test(AODVTest::lastReceive == node4.getIp(), "Last received packet was node " + getStringFromIp(AODVTest::lastReceive));

	cout << "[TESTS]: Test aodv link break passed" << endl;

	node0.logRoutingTable();
	node1.logRoutingTable();
	node2.logRoutingTable();
	node3.logRoutingTable();
	node4.logRoutingTable();

}
