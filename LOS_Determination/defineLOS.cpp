#include <iostream>
#include "LOS.h"
#include <iomanip>
#pragma warning(disable : 4996)

using namespace std;

// constructor
LOS::LOS(int id, string orbitFileName)
{
	homeID = id;
	fileName = orbitFileName;
	pullOrbitData();
}

void LOS::pullOrbitData() {
	// This function pulls a text file with each s/c's orbit position over time and some other useful info

	const char* filePtr = fileName.c_str();
	FILE* orbitFile;
	orbitFile = fopen(filePtr, "r");				// open text file
	if (orbitFile == NULL) {
		cout << "Error in opening file" << endl;	// print error message if file wasn't opened
	}

	// fill in some information from the text file
	char scName[80];			// name of entity. Will either be "Spacecraft" or "Carrier". Needs to be char array for fscanf
	int scID;					// Number ID for the s/c. Carrier ID is 0. IDs range from 1 to numSC
	pos3D temp;					// declare a temporary holder for orbit x-y-z position data

	// Read important first line of file
	fscanf(orbitFile, "%i" "%i" "%i", &numSC, &timestep_size, &timestep_num);	//read top line of txt file that has key simulation info
	cout << endl;				// give some space on display
	cout << "Number of S/C is: " << numSC << endl;
	cout << "Timestep size is: " << timestep_size << endl;
	cout << "Number of timesteps is: " << timestep_num << endl;

	numSC = numSC;

	// initialize orbitData and size it
	for (size_t i = 0; i < numSC + 1; i++)
	{
		orbitData[i].resize(timestep_num);	// make each key, i.e. each s/c
		//cout << "Initializing S/C: " << i << endl;
	}
	cout << endl << endl;		// give some space in the display

	// Read carrier data
	fscanf(orbitFile, "%s", &scName);	//read in s/c title string and s/c number ID
	for (int j = 0; j < timestep_num; j++) {
		fscanf(orbitFile, "%lf" "%lf" "%lf", &temp.xpos, &temp.ypos, &temp.zpos);
		orbitData[0][j] = temp;
		// cout << setprecision(12) << scName << " is at position: " << orbitData[0][j].xpos << ", " << orbitData[0][j].ypos << ", " << orbitData[0][j].zpos << endl;
	}

	// Read small s/c data
	for (int i = 1; i < numSC+1; i++)
	{
		fscanf(orbitFile, "%s" "%i", &scName, &scID);	//read in s/c title string and s/c number ID
		for (int j = 0; j < timestep_num; j++) {
			fscanf(orbitFile, "%lf" "%lf" "%lf", &temp.xpos, &temp.ypos, &temp.zpos);
			orbitData[i][j] = temp;
		}
		//cout << setprecision(12) << scName << ": " << scID << " is at POSITION: " << orbitData[i].back().xpos << ", " << orbitData[i].back().ypos << ", " << orbitData[i].back().zpos << endl;

	}

	fclose(orbitFile);
	
}


bool LOS::isThereLOS(int other_sc_ID, double current_time)
{
	// This function needs "sc_orbits.txt" to run

	// Steps of how to determine line of sight.
	// 1. Find the iteration number corresponding to the input time
	// 2. Pull the cartesian coordinates from .txt orbit file for each s/c. Interpolate between timesteps
	// 3. With the position vector for both s/c 1 and s/c 2, run algorithm for finding closest point on a line. As of 10/31/2019, using spherical approximation for asteroid
	// 4. Return: true (s/c can see each other) or false (no line of sight) using the results of the algorithm
	// Friendly Reminder: s/c and sc are abbreviations for spacecraft and can refer to any satellite, including the carrier
	// Also note that "SC 1" and "SC 2" are used to annotate the code below. This is simply to keep track of what is what and to match the literature
	//		Do not confuse these with the s/c number IDs.
	// Algorithm in Step 3 is from JPL's code and is based off of this website: http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html

	// Step 1 -------------------------------------------------------------------------------------------------------------
		// The time passed to this function will not be exactly equal to one of the timesteps. It will always fall somewhere in between two timesteps
		// Because of this, linear interpolation is used to estimate the position between the timesteps
		// To clarify, let t=time input to function. Let t_1 be the time less than t and t_2 be the time greater than t such that t_1 <= t < t_2
		// Knowing the size of the timestep (i.e. how "big" the step is), the timestep (or iteration or entry) corresponding to t_1 can be found:
		int timeStep_Below = current_time / timestep_size;		// timestep corresponding to t_1
		int timeStep_Above = timeStep_Below + 1;				// timestep corresponding to t_2
		double t_1 = timeStep_Below * timestep_size*1.0;			// t_1
		double t_2 = timeStep_Above * timestep_size*1.0;			// t_2

	// Step 2 -------------------------------------------------------------------------------------------------------------
		// Position vectors have the following format:
				// r[0] = x coordinate
				// r[1] = y coordinate
				// r[2] = z coordinate
		// position vectors for s/c 1 and 2
		double r1[3];
		double r2[3];

		// these are temporary arrays for performing interpolation
		double positionBelow[3];	// will store position for timestep before input time.	This corresponds to t_1
		double positionAbove[3];	// will store position for timestep after input time.	This corresponds to t_2

		// Find Position vector for SC 1
		positionBelow[0] = orbitData[homeID][timeStep_Below].xpos;
		positionBelow[1] = orbitData[homeID][timeStep_Below].ypos;
		positionBelow[2] = orbitData[homeID][timeStep_Below].zpos;
		positionAbove[0] = orbitData[homeID][timeStep_Above].xpos;
		positionAbove[1] = orbitData[homeID][timeStep_Above].ypos;
		positionAbove[2] = orbitData[homeID][timeStep_Above].zpos;


		// linearly interpolate between t_1 and t_2 for x, y, and z coordinates
		// X coordinate
		r1[0] = positionBelow[0] + (current_time - t_1) * (positionAbove[0] - positionBelow[0]) / (t_2 - t_1);
		// Y coordinate
		r1[1] = positionBelow[1] + (current_time - t_1) * (positionAbove[1] - positionBelow[1]) / (t_2 - t_1);
		// Z coordinate
		r1[2] = positionBelow[2] + (current_time - t_1) * (positionAbove[2] - positionBelow[2]) / (t_2 - t_1);
		cout << "Position r1 is: " << r1[0] << "   " << r1[1] << "   " << r1[2] << endl << endl;

		// Repeat process for finding the posiion vector of S/C 2
		positionBelow[0] = orbitData[other_sc_ID][timeStep_Below].xpos;
		positionBelow[1] = orbitData[other_sc_ID][timeStep_Below].ypos;
		positionBelow[2] = orbitData[other_sc_ID][timeStep_Below].zpos;
		positionAbove[0] = orbitData[other_sc_ID][timeStep_Above].xpos;
		positionAbove[1] = orbitData[other_sc_ID][timeStep_Above].ypos;
		positionAbove[2] = orbitData[other_sc_ID][timeStep_Above].zpos;

		// linearly interpolate between t_1 and t_2 for x, y, and z coordinates
		// X coordinate
		r2[0] = positionBelow[0] + (current_time - t_1) * (positionAbove[0] - positionBelow[0]) / (t_2 - t_1);
		// Y coordinate
		r2[1] = positionBelow[1] + (current_time - t_1) * (positionAbove[1] - positionBelow[1]) / (t_2 - t_1);
		// Z coordinate
		r2[2] = positionBelow[2] + (current_time - t_1) * (positionAbove[2] - positionBelow[2]) / (t_2 - t_1);
		cout << "Position r2 is: " << r2[0] << "   " << r2[1] << "   " << r2[2] << endl << endl;

	// Step 3 -------------------------------------------------------------------------------------------------------------
		// Create relative position vector, r21
		double r21[3];					// declare relative position vector from s/c 1 to s/c 2
		r21[0] = r2[0] - r1[0];			// x component of r21
		r21[1] = r2[1] - r1[1];			// y component of r21
		r21[2] = r2[2] - r1[2];			// z component of r21

		// Find t. 
		double denominator = r21[0] * r21[0] + r21[1] * r21[1] + r21[2] * r21[2];	// square of the magnitude of r21, the relative position vector
		double numerator = r1[0] * r21[0] + r1[1] * r21[1] + r1[2] * r21[2];		// dot product between r1 and r21
		double t = numerator / denominator;		// scalar variable that represents distance along line L to the point closest to the origin
		t *= -1;								// do not confuse this t with time variables!! t is used here to look like the literature
		cout << "t = " << t << std::endl;

		// Find position vector, rp, which is from asteroids center (also the origin) to the closest point on line L
		double rp[3];					// position vector from origin (asteroid center) to closest point on line L, which passes through s/c 1 and s/c 2
		rp[0] = r21[0] * t + r1[0];		// x component of rp
		rp[1] = r21[1] * t + r1[1];		// y component of rp
		rp[2] = r21[2] * t + r1[2];		// z component of rp


	// Step 4 -------------------------------------------------------------------------------------------------------------
		double rp_norm = sqrtf(rp[0] * rp[0] + rp[1] * rp[1] + rp[2] * rp[2]);		// norm (or magnitude) of the vector rp. This is called D in the brief description below
		std::cout << "Magnitude of Rp = " << rp_norm << std::endl;

		if (t >= 0 && t <= 1) {
			if (rp_norm >= asteroid_radius) {
				std::cout << "CASE 1" << std::endl;
				return true;	// occurs when the line connecting the s/c doesn't pass through the sphere
			}					

			else {
				std::cout << "CASE 2" << std::endl;
				return false;	// occurs when the line connecting the s/c DOES pass through the sphere
			}								
		}
		else
		{
			if (sqrt(r1[0] * r1[0] + r1[1] * r1[1] + r1[2] * r1[2]) > asteroid_radius&& sqrt(r2[0] * r2[0] + r2[1] * r2[1] + r2[2] * r2[2]) > asteroid_radius)
			{
				std::cout << "CASE 3" << std::endl;
				return true;	// this occurs when the closst point to the asteroid center doesn't lie on the line connecting the s/c, which (practically) always results in true (i.e. there is LOS)
			}
			else {
				std::cout << "CASE 4" << std::endl;
				return false;	// this occurs when a s/c is inside of the asteroid (which shouldn't happen anyways, but is here for completeness)
			}	
		}
	// Brief algorithm description: 
		// There is a line L that passes through s/c 1 and s/c 2. Also let the center of the asteroid be at the origin.
		// The portion of line L  between s/c 1 and s/c 2 is essentially the "line of sight" line. We are interested in whether a 
		// point in the asteroid lies on this portion of the line L. If even one point does, then there is no LOS. 
		// To determine this, a short algorithm is used. 
		// The first step is to find the point, P, on line L that is closest to the origin.
		// Next, calculate the distance, D, from the asteroid's center (i.e. origin) to the point P.
		// Next, check if point P lies between s/c 1 and s/c 2 (this corresponds to having a t between 0 and 1)
		// If it does, check whether D > asteroid radius. 
		//		If it is, then point P lies outside of the asteroid. The s/c have line of sight to each other
		//		If it isn't, then point P lies inside the asteroid and line of sight is blocked (i.e. no LOS)
		// If point P doesn't lie between the s/c, 
		//      Then there is LOS between the s/c for all practical purposes.
		//		The only way there wouldn't be LOS in this case would be if one or both of the s/c were actually inside the asteroid
		//		which means a s/c crashed and there is a whole lot of other issues.
		// Again, some of the material for this can be found on http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html

}


void LOS::scalePositions(float scaleFactor)
{// Scale the asteroid radius as well
	asteroid_radius *= scaleFactor;	
	for (int i = 0; i < numSC+1; i++)				// scale positions for each s/c
	{	
		for (int j = 0; j < timestep_num; j++) {	// scale positions at each timestep
			orbitData[i][j].xpos *= scaleFactor;	// scale the x position
			orbitData[i][j].ypos *= scaleFactor;	// scale the y position
			orbitData[i][j].zpos *= scaleFactor;	// scale the z position
		}
	}
}


void LOS::distanceBetweenSC(int other_sc_ID, double current_time, double relativePositionInfo[])
{
	// Step 1 -------------------------------------------------------------------------------------------------------------
		// The time passed to this function will not be exactly equal to one of the timesteps. It will always fall somewhere in between two timesteps
		// Because of this, linear interpolation is used to estimate the position between the timesteps
		// To clarify, let t=time input to function. Let t_1 be the time less than t and t_2 be the time greater than t such that t_1 <= t < t_2
		// Knowing the size of the timestep (i.e. how "big" the step is), the timestep (or iteration or entry) corresponding to t_1 can be found:
	int timeStep_Below = current_time / timestep_size;		// timestep corresponding to t_1
	int timeStep_Above = timeStep_Below + 1;				// timestep corresponding to t_2
	double t_1 = timeStep_Below * timestep_size * 1.0;			// t_1
	double t_2 = timeStep_Above * timestep_size * 1.0;			// t_2

// Step 2 -------------------------------------------------------------------------------------------------------------
	// Position vectors have the following format:
			// r[0] = x coordinate
			// r[1] = y coordinate
			// r[2] = z coordinate
	// position vectors for s/c 1 and 2
	double r1[3];
	double r2[3];

	// these are temporary arrays for performing interpolation
	double positionBelow[3];	// will store position for timestep before input time.	This corresponds to t_1
	double positionAbove[3];	// will store position for timestep after input time.	This corresponds to t_2

	// Find Position vector for SC 1
	positionBelow[0] = orbitData[homeID][timeStep_Below].xpos;
	positionBelow[1] = orbitData[homeID][timeStep_Below].ypos;
	positionBelow[2] = orbitData[homeID][timeStep_Below].zpos;
	positionAbove[0] = orbitData[homeID][timeStep_Above].xpos;
	positionAbove[1] = orbitData[homeID][timeStep_Above].ypos;
	positionAbove[2] = orbitData[homeID][timeStep_Above].zpos;


	// linearly interpolate between t_1 and t_2 for x, y, and z coordinates
	// X coordinate
	r1[0] = positionBelow[0] + (current_time - t_1) * (positionAbove[0] - positionBelow[0]) / (t_2 - t_1);
	// Y coordinate
	r1[1] = positionBelow[1] + (current_time - t_1) * (positionAbove[1] - positionBelow[1]) / (t_2 - t_1);
	// Z coordinate
	r1[2] = positionBelow[2] + (current_time - t_1) * (positionAbove[2] - positionBelow[2]) / (t_2 - t_1);
	cout << "Position r1 is: " << r1[0] << "   " << r1[1] << "   " << r1[2] << endl << endl;

	// Repeat process for finding the posiion vector of S/C 2
	positionBelow[0] = orbitData[other_sc_ID][timeStep_Below].xpos;
	positionBelow[1] = orbitData[other_sc_ID][timeStep_Below].ypos;
	positionBelow[2] = orbitData[other_sc_ID][timeStep_Below].zpos;
	positionAbove[0] = orbitData[other_sc_ID][timeStep_Above].xpos;
	positionAbove[1] = orbitData[other_sc_ID][timeStep_Above].ypos;
	positionAbove[2] = orbitData[other_sc_ID][timeStep_Above].zpos;

	// linearly interpolate between t_1 and t_2 for x, y, and z coordinates
	// X coordinate
	r2[0] = positionBelow[0] + (current_time - t_1) * (positionAbove[0] - positionBelow[0]) / (t_2 - t_1);
	// Y coordinate
	r2[1] = positionBelow[1] + (current_time - t_1) * (positionAbove[1] - positionBelow[1]) / (t_2 - t_1);
	// Z coordinate
	r2[2] = positionBelow[2] + (current_time - t_1) * (positionAbove[2] - positionBelow[2]) / (t_2 - t_1);
	cout << "Position r2 is: " << r2[0] << "   " << r2[1] << "   " << r2[2] << endl << endl;

	// Step 3 -------------------------------------------------------------------------------------------------------------
		// Create relative position vector, r21
	double r21[3];					// declare relative position vector from s/c 1 to s/c 2
	r21[0] = r2[0] - r1[0];			// x component of r21
	r21[1] = r2[1] - r1[1];			// y component of r21
	r21[2] = r2[2] - r1[2];			// z component of r21

	// Step 4
	// Find the magnitude of the relative position vector, r21
	double r21_mag = sqrt(r21[0] * r21[0]   +   r21[1] * r21[1]   +   r21[2] * r21[1]); // square root of the sum of squared componets
	cout << "From the internal works, the distance is: " << r21_mag << endl;
	// Step 5
	// Populating array (length 4) with the mangitude of the position vector and the components of the vector
	// The array[4] relativePositionInfo is broken down like this:
	//		-Element 0: the x-component of r21
	//		-Element 1: the y-component of r21
	//		-Element 2: the z-component of r21
	//	    -Element 3: the magnitude of r21 (i.e. the Euclidean distance between the two s/c)
	relativePositionInfo[0] = r21[0];
	relativePositionInfo[1] = r21[1];
	relativePositionInfo[2] = r21[2];
	relativePositionInfo[3] = r21_mag;
}
