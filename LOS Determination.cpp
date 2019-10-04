// LOS Determination.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <string.h>
#include <fstream>

using namespace std;

bool determineLOS()
{
	// real inputs: int sc1_ID, int sc2_ID, double time, bool unitFlag
	int sc1_ID = 0; 
	int sc2_ID = 1; 
	float time = 45123.1; 
	bool unitFlag = 0;


	// This function needs "sc_orbits.txt" to run

	// Steps of how to determine line of sight.
	// 1. open .txt file with sc orbit info
	// 2. read in the timestep size, number of s/c, and number of timesteps
	// 3. find the iteration number corresponding to the input time
	// 4. pull the cartesian coordinates from .txt orbit file for each s/c. Interpolate between timesteps
	// 5. if unitFlag = true, then scale down orbit distances to drone distances
	// 6. with the position vector for both s/c 1 and s/c 2, run algorithm. As of 10/1/2019, using spherical approximation for asteroid
	// 7. return: true (s/c can see each other) or false (no line of sight)
	// Friendly Reminder: s/c and sc are abbreviations for spacecraft and can refer to any satellite, including the carrier
	// Algorithm in Step 6 is from JPL's code and is based off of this website: http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
	
	// Define some constants
	double asteroid_radius = 16.84;			// asteroid radius [km] using spherical approximation (mean radius obtained from wikipedia)

	// Step 1:
	ifstream input_file;
	input_file.open("sc_orbits.txt");		// open file to read orbital positions of all s/c and other info

	// Step 2:
	int tStepSize;		// size of timestep [seconds]
	int num_sc, numTimeSteps;
	input_file >> num_sc;
	input_file >> tStepSize;
	input_file >> numTimeSteps;
	cout << "Number of S/C: " << num_sc << ", " << "Timestep size: " << tStepSize << ", " << "Number of timesteps: " << numTimeSteps << endl;
	

	// Step 3:
	// The time passed to this function will not be exactly equal to one of the timesteps. It will always fall somewhere in between two timesteps
	// Because of this, linear interpolation is used to estimate the position between the timesteps
	// To clarify, let t=time input to function. Let t_1 be the time less than t and t_2 be the time greater than t such that t_1 <= t < t_2
	// Knowing the size of the timestep (i.e. how "big" the step is), the timestep (or iteration or entry) corresponding to t_1 can be found:
	int timeStep_Below = time / tStepSize;		// timestep corresponding to t_1
	int timeStep_Above = timeStep_Below + 1;	// timestep corresponding to t_2
	int t_1 = timeStep_Below * tStepSize;		// t_1
	int t_2 = timeStep_Above * tStepSize;		// t_2


	// Step 4:
		// Position vectors have the following format:
			// r[0] = x coordinate
			// r[1] = y coordinate
			// r[2] = z coordinate
		// position vectors for s/c 1 and 2. units of km
		double r1[3]; // = { 0.5, 0.5, 0 };
		double r2[3]; // = { 0.5, 0.4, 0 };

		// these are temporary vectors for storing info from the txt file and performing interpolation
		double temp1[3];	// will store position for timestep before input time.	This corresponds to t_1
		double temp2[3];	// will store position for timestep after input time.	This corresponds to t_2
		
		// Find line of txt file where the string name of the s/c is. For example, for the carrier, lineHeader = 2, and for the 1st s/c.
		// this is used as a reference to find the position at the correct timestep for each particular s/c

		// Do this process for s/c 1
			int lineHeader = sc1_ID * (numTimeSteps + 1) + 2;
			int line = lineHeader + 1 + timeStep_Below;
			cout << "The timestep below selected time is: " << timeStep_Below << endl;
			cout << "Line Header is: " << lineHeader << " and the specific line is: " << line << endl;

			for (int i = 0; i < line - 1; i++) {
				input_file.ignore(numeric_limits<streamsize>::max(), '\n');	//skip unecessary lines
			}
			input_file >> temp1[0] >> temp1[1] >> temp1[2];	// store position for t_1 for s/c 1
			input_file >> temp2[0] >> temp2[1] >> temp2[2];	// store position for t_2 for s/c 2
			cout << "Numbers from txt file are: " << temp1[0] << "   " << temp1[1] << "   " << temp1[2] << endl;
			cout << "Numbers from txt file are: " << temp2[0] << "   " << temp2[1] << "   " << temp2[2] << endl << endl;

			// linearly interpolate between t_1 and t_2 for x, y, and z coordinates
			// X coordinate
				r1[0] = temp1[0] + (time - t_1) * (temp2[0] - temp1[0]) / (t_2 - t_1);
			// Y coordinate
				r1[1] = temp1[1] + (time - t_1) * (temp2[1] - temp1[1]) / (t_2 - t_1);
			// Z coordinate
				r1[2] = temp1[2] + (time - t_1) * (temp2[2] - temp1[2]) / (t_2 - t_1); 
			cout << "Position r1 is: " << r1[0] << "   " << r1[1] << "   " << r1[2] << endl << endl;

		// Do this process again for s/c 2
			// close and reopen file because pulling information for r1 created some issues reading in data for r2
			input_file.close();						// close sc_orbits.txt
			input_file.open("sc_orbits.txt");		// open file to read orbital positions of all s/c and other info

			lineHeader = sc2_ID * (numTimeSteps + 1) + 2;
			line = lineHeader + 1 + timeStep_Below;	
			cout << "The timestep below selected time is: " << timeStep_Below << endl;
			cout << "Line Header is: " << lineHeader << " and the specific line is: " << line << endl;

			for (int i = 0; i < line - 1; i++) {
				input_file.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			input_file >> temp1[0] >> temp1[1] >> temp1[2];	// store position for t_1 for s/c 1
			input_file >> temp2[0] >> temp2[1] >> temp2[2];	// store position for t_2 for s/c 2
			cout << "Numbers from txt file are: " << temp1[0] << "   " << temp1[1] << "   " << temp1[2] << endl;
			cout << "Numbers from txt file are: " << temp2[0] << "   " << temp2[1] << "   " << temp2[2] << endl << endl;

			// linearly interpolate between t_1 and t_2 for x, y, and z coordinates
			// X coordinate
				r2[0] = temp1[0] + (time - t_1) * (temp2[0] - temp1[0]) / (t_2 - t_1);
			// Y coordinate
				r2[1] = temp1[1] + (time - t_1) * (temp2[1] - temp1[1]) / (t_2 - t_1);
			// Z coordinate
				r2[2] = temp1[2] + (time - t_1) * (temp2[2] - temp1[2]) / (t_2 - t_1);
			cout << "Position r2 is: " << r2[0] << "   " << r2[1] << "   " << r2[2] << endl << endl;
		
		// Close file
		input_file.close();			// close sc_orbits.txt

	// Step 5:
		double scalingFactor = 1.0 * 1/1000;	// Scales distances. Needs to be changed later to fit testbed. Currently set to km to meters for demonstration
		if (unitFlag) {
			asteroid_radius *= scalingFactor;
			for (int i = 0; i < 3; i++) {
				r1[i] *= scalingFactor;
				r2[i] *= scalingFactor;
			}
		}
		cout << "asdkjfasdfh " << asteroid_radius << endl;
	// Step 6:
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
		std::cout << "t = " << t << std::endl;

		// Find position vector, rp, which is from asteroids center (also the origin) to the closest point on line L
		double rp[3];					// position vector from origin (asteroid center) to closest point on line L, which passes through s/c 1 and s/c 2
		rp[0] = r21[0] * t + r1[0];		// x component of rp
		rp[1] = r21[1] * t + r1[1];		// y component of rp
		rp[2] = r21[2] * t + r1[2];		// z component of rp


	// Step 7
		double rp_norm = sqrtf(rp[0] * rp[0] + rp[1] * rp[1] + rp[2] * rp[2]);		// norm (or magnitude) of the vector rp. This is called D in the brief description below
		std::cout << "Magnitude of Rp = " << rp_norm << std::endl;

		if (t >= 0 && t <= 1) {
			if (rp_norm >= asteroid_radius)  { 
				std::cout << "CASE 1" << std::endl;
				return true; }	// occurs when the line connecting the s/c doesn't pass through the sphere

			else { 
				std::cout << "CASE 2" << std::endl;
				return false; }								// occurs when the line connecting the s/c DOES pass through the sphere
		}
		else
		{
			if (sqrt(r1[0] * r1[0] + r1[1] * r1[1] + r1[2] * r1[2]) > asteroid_radius && sqrt(r2[0] * r2[0] + r2[1] * r2[1] + r2[2] * r2[2]) > asteroid_radius)
			{
				std::cout << "CASE 3" << std::endl;
				return true;		// this occurs when the closst point to the asteroid center doesn't lie on the line connecting the s/c, which (practically) always results in true (i.e. there is LOS)
			}
			else { 
				std::cout << "CASE 4" << std::endl;
				return false; }	// this occurs when a s/c is inside of the asteroid (which shouldn't happen anyways, but is here for completeness)
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



int main()
{
    std::cout << "Hello World!\n";
	bool los = determineLOS();
	std::cout << los << std::endl;
}
