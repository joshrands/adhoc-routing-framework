
#include "rem.h"

#include <assert.h>
#include "string.h"
#include <cmath>
#include <iostream>

using namespace std;

// Very cheap testing framework
void test_test();
void test_battery_model();
void test_rss_model();

int main (int argc, char *argv[]) 
{	
	cout << "Running tests..." << endl;	

	test_test();
	test_battery_model();
	test_rss_model();

	cout << "TESTS COMPLETE." << endl;

	return 0;
}

void test_test()
{
	// this test tests tests
	assert(true == true);
	cout << "Test test complete." << endl;
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
		cout << "Data count = " << model.getDataCount() << endl;
		cout << "Predicted battery = " << model.getDataPoint(4) << endl;
	}

	assert(model.getDataCount() == 4);
	assert(model.getDataPoint(4) == 30);
	assert(model.getDataPoint(8) == 10);

	cout << "REM Battery Model test COMPLETE." << endl;
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
		cout << "Data count = " << model.getDataCount() << endl;
		cout << "Predicted rss = " << model.getDataPoint(6) << endl;
		cout << "Difference = " << std::abs(float(model.getDataPoint(6) - (-72))) << endl;
		cout << "Difference to 75: " << abs(float(model.getDataPoint(10) - (-75))) << endl;
	}

	assert(model.getDataCount() == 4);
	// test with accuracry requirement of 1 dB
	assert(abs(float(model.getDataPoint(6) - (-72))) <= 1);
	assert(abs(float(model.getDataPoint(10) - (-75))) <= 1);

	cout << "REM RSS Model test COMPLETE." << endl;
}
