
#include "rem.h"

#include <assert.h>
#include "string.h"
#include <iostream>

using namespace std;

// Very cheap testing framework
void test_test();

int main (int argc, char *argv[]) 
{	
	cout << "Running tests..." << endl;	

	test_test();

	cout << "TESTS COMPLETE." << endl;

	return 0;
}

void test_test()
{
	// this test tests tests
	assert(true == true);
	cout << "Test test complete." << endl;
}
