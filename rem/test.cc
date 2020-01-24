#include <assert.h>
#include <cmath>
#include <iostream>
#include <thread>

#include "string.h"
#include "rem.h"
#include "../aodv/aodv.h"
#include "../aodv/aodv_test.h"

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
void test_battery_model();
void test_rss_model();
void test_packet_encode_decode();
void test_local_update_thread();

int main (int argc, char *argv[]) 
{	
	cout << "[TESTS]: Running tests..." << endl;	

	test_test();
	test_battery_model();
	test_rss_model();
	test_packet_encode_decode();
	test_local_update_thread();

	cout << "[TESTS]: TESTS COMPLETE." << endl;

	return 0;
}

void test_test()
{
	// this test tests tests
	test(true == true, "true == true");
}

void test_battery_model()
{
	BatteryModel model;

	// add data points 
	// battery = 50 at time 0
	model.addDataPoint(50, 0);
	model.addDataPoint(45, 1);
	model.addDataPoint(40, 2);
	model.addDataPoint(35, 3);

	if (BATTERY_DEBUG)
	{
		cout << "[DEBUG]: Data count = " << model.getDataCount() << endl;
		cout << "[DEBUG]: Predicted battery = " << model.getDataPoint(4) << endl;
	}

	test(model.getDataCount() == 4, "getDataCount() == 4");
	test(model.getDataPoint(4) == 30, "getDataPoint(4) == 30");
	test(model.getDataPoint(8) == 10, "getDataPoint(8) == 10");

	cout << "[TESTS]: Battery model test complete." << endl;
}

void test_rss_model()
{
	RssModel model;

	// add data points 
	model.addDataPoint(-68, 1.75);
	model.addDataPoint(-69, 2.75);
	model.addDataPoint(-70, 3.5);
	model.addDataPoint(-71, 5.25);

	if (RSS_DEBUG)
	{
		cout << "[DEBUG]: Data count = " << model.getDataCount() << endl;
		cout << "[DEBUG]: Predicted rss = " << model.getDataPoint(6) << endl;
		cout << "[DEBUG]: Difference = " << std::abs(float(model.getDataPoint(6) - (-72))) << endl;
		cout << "[DEBUG]: Difference to 75: " << abs(float(model.getDataPoint(10) - (-75))) << endl;
	}

	test(model.getDataCount() == 4, "getDataCount() == 4");
	// test with accuracry requirement of 0.5 dB
	test(abs(float(model.getDataPoint(6) - (-72))) <= 0.5, to_string(abs(float(model.getDataPoint(6) - (-72)))) + " <= 0.5");
	test(abs(float(model.getDataPoint(10) - (-75))) <= 0.5, to_string(abs(float(model.getDataPoint(10) - (-75)))) + " <= 0.5");

	cout << "[TESTS]: RSS model test complete." << endl;
}

void test_packet_encode_decode()
{
	IP_ADDR node1 = getIpFromString("192.168.0.1");
	IP_ADDR node2 = getIpFromString("192.168.0.2");

	REMTest remTest;
	remTest.initialize(node1);

	// TODO: Add warnings if no routing protocol has been assigned... 
	AODVTest aodvTest(node1);
	remTest.routing = &aodvTest;

	// add data points 
	// battery = 50 at time 0
	remTest.updateLocalBatteryModel(remTest.getCurrentBatteryLevel());
	remTest.runClock();
	remTest.updateLocalBatteryModel(remTest.getCurrentBatteryLevel());
	remTest.runClock();
	remTest.updateLocalBatteryModel(remTest.getCurrentBatteryLevel());
	remTest.runClock();
	remTest.updateLocalBatteryModel(remTest.getCurrentBatteryLevel());
	remTest.runClock();

	REMModelPacket modelPacket = remTest.localBatteryModel.createREMModelPacket();
	int length = sizeof(modelPacket);
	char* buffer = (char*)(malloc(length));
	memcpy(buffer, &(modelPacket), length);

	REMTest rem;
	rem.initialize(node2);

	AODVTest aodv(node2);
	rem.routing = &aodv;

	rem.handleMonitoringPacketBuffer(buffer, length, node1, MONITOR_PORT);

	test(rem.getBatteryLevel(node1) == remTest.getCurrentBatteryLevel(), "Received network packet test: " + to_string(rem.getBatteryLevel(node1)));

	cout << "[TESTS]: Packet encode decode tests complete." << endl;
}

void test_local_update_thread()
{
	REMTest rem(getIpFromString("192.168.0.1"));
	AODVTest aodv("192.168.0.1");
	rem.routing = &aodv;

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	cout << "[TESTS]: Local update thread tests complete." << endl;
}
