#pragma once
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <cmath>

using namespace std;

struct pos3D
{
	double xpos;
	double ypos;
	double zpos;
};

class LOS
{
public:
	// Declare variables
	map<int, vector<pos3D>> orbitData;			// store orbit xyz positions for each s/c
	int homeID;									// this is the ID of the s/c that the program is running on
	int other_sc_ID;							// this is the ID of the s/c that a s/c wants to know if it has LOS with
	string fileName;							// should just be "sc_orbits.txt", but this is here for flexibility
	int numSC;									// total # of s/c (not including carrier). This data comes from orbit info file
	int timestep_size;							// size of timestep. e.g. deltaT = 1 sec. This data comes from orbit info file
	int timestep_num;							// number of data points that each represent a point in time. This data comes from orbit info file.
	double asteroid_radius = 1;					// asteroid radius [km] using spherical approximation (mean radius obtained from wikipedia)

	// Declare Constructor
	LOS(int ID, string orbitFileName);			// declare constructor. define it in defineLOS.cpp

	// Declare functions
	void pullOrbitData();									// open the file with orbit info
	bool isThereLOS(int other_sc_ID, double current_time);	// returns true if there is no Line of Sight (LOS) and false if not
	void scalePositions(float scaleFactor);					// scales all of the positions by scaleFactor input argument. Use for testbed adaptation
	void distanceBetweenSC(int other_sc_ID, double current_time, double relativePositionInfo[]);  // returns the distance [km] between s/c 1 (homeID) and s/c 2 (other_sc_ID) at a particular time. Note this is not a position vector
};