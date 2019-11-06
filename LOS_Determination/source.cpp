#include <iostream>
#include "LOS.h"
#include <iomanip>
#include <map>
#include <vector>
#pragma warning(disable : 4996)

using namespace std;

int main() {
	int id = 0;
	string file = "sc_orbits.txt";
	LOS test(id, file);
	// cout << test.fileName << endl;

	bool losYN = test.isThereLOS(1, 3);
	cout << "Is there LOS? Answer is: " << losYN << endl;

	cout << "Asteroid radius is: " << test.asteroid_radius << endl;

	test.scalePositions(2);
	cout << "Asteroid radius is: " << test.asteroid_radius << endl;

	return 0;
}