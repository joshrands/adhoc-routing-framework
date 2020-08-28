/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Header files from module
#include "ns3/defines.h"
#include "ns3/packet_buffer.h"
#include "ns3/adhoc_structs.h"
#include "ns3/test.h"

#include <string.h>

// Do not put your test classes in namespace ns3.
using namespace ns3;

// Test the PacketBuffer class
class PacketBufferTestCase : public TestCase
{
public:
	PacketBufferTestCase ();
	virtual ~PacketBufferTestCase ();

private:
	virtual void DoRun (void);
};

// Add some helpful text to this case to describe what it is intended to test
PacketBufferTestCase::PacketBufferTestCase ()
	: TestCase ("AdhocRouting Packet Buffer")
{
}

// Destructor
PacketBufferTestCase::~PacketBufferTestCase ()
{
}

// Pure virtual method from class TestCase
//
void
PacketBufferTestCase::DoRun (void)
{
	// Create Packet Buffer
	PacketBuffer testPBuffer;
	IP_ADDR testIP = 1;
	NS_TEST_ASSERT_MSG_EQ (testPBuffer.packetsWaiting(testIP), false, "Empty buffer thinks it has a packet...");

	// Add packets to buffer
	char* packet = (char *)"Hello World";
	int length = (int)strlen(packet), portId = 0;

	BufferedPacket bufferedPacket;
	bufferedPacket.dest = testIP;
	bufferedPacket.buffer = (char *)(malloc(length));
	memcpy(bufferedPacket.buffer, packet, length);
	bufferedPacket.length = length;
	bufferedPacket.portId = portId;
	testPBuffer.storePacket(bufferedPacket);

	NS_TEST_ASSERT_MSG_EQ (testPBuffer.packetsWaiting(testIP), true, "Added packet to buffer, but buffer does not report a packet waiting.");

	// Remove packet from buffer
	bufferedPacket = testPBuffer.getPacket(testIP);
	free(bufferedPacket.buffer);

	NS_TEST_ASSERT_MSG_EQ (testPBuffer.packetsWaiting(testIP), false, "Empty buffer thinks it has a packet...");

	// Fill queue
	for(int i = 0; i <= 10; i++)
	{
		BufferedPacket bufferedPacket;
		bufferedPacket.dest = testIP;
		bufferedPacket.buffer = (char *)(malloc(length));
		memcpy(bufferedPacket.buffer, packet, length);
		bufferedPacket.length = length;
		bufferedPacket.portId = portId;

		testPBuffer.storePacket(bufferedPacket);

		NS_TEST_ASSERT_MSG_EQ (testPBuffer.packetsWaiting(testIP), true, "Non-empty buffer thinks it is empty...");
	}

    // Empty queue
    while(testPBuffer.packetsWaiting(testIP))
    {
        BufferedPacket packet = testPBuffer.getPacket(testIP);
    	free(packet.buffer);
    }

	NS_TEST_ASSERT_MSG_EQ (testPBuffer.packetsWaiting(testIP), false, "Empty buffer thinks it has a packet...");
}

// TestSuite class for AdHocRoutingHelper and its related classes
//
class AdhocRoutingHelperTestSuite : public TestSuite
{
public:
	AdhocRoutingHelperTestSuite ();
};

AdhocRoutingHelperTestSuite::AdhocRoutingHelperTestSuite ()
	: TestSuite ("adhoc-routing-helper", UNIT)
{
	// TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
	AddTestCase (new PacketBufferTestCase, TestCase::QUICK);
}

// Allocate an instance of this TestSuite
static AdhocRoutingHelperTestSuite adhocRoutingHelperTestSuite;

