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
	AODVTest aodv("192.168.0.1");
	HelloTest hello(HELLO_PORT, &aodv);
}
