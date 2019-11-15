
#include "rem.h"
#include "../aodv/aodv.h"

#include <assert.h>
#include "string.h"
#include <cmath>
#include <iostream>

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

int main (int argc, char *argv[]) 
{	
	cout << "[TESTS]: Running tests..." << endl;	

	test_test();
	test_battery_model();
	test_rss_model();

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
