#include <iostream>
#include "LOS.h"
#include <iomanip>
#include <map>
#include <vector>
#pragma warning(disable : 4996)

using namespace std;

int main() {
	int id = 0;
	string file = "sc_orbits_test3.txt";
	LOS test(id, file);
	// cout << test.fileName << endl;

	bool losYN = test.isThereLOS(1, 3);
	cout << "Is there LOS? Answer is: " << losYN << endl;

	cout << "Asteroid radius is: " << test.asteroid_radius << endl;

	test.scalePositions(2);
	cout << "Asteroid radius is: " << test.asteroid_radius << endl;

	double relativePositionInfo[4];
	test.distanceBetweenSC(1, 3, relativePositionInfo);
	double distance = relativePositionInfo[3];
	cout << "Distance between s/c is:" << distance << endl;
	cout << "x:" << relativePositionInfo[0] << endl;
	cout << "y:" << relativePositionInfo[1] << endl;
	cout << "z:" << relativePositionInfo[2] << endl;


	return 0;
}