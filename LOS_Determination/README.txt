Hello there!

This set of code is for determining whether two spacecraft have line of sight (LOS) to each other.
This is imperative for checking whether spacecraft can talk to one another while orbiting an asteroid.
This code should be used for both NS3 (or other computer simulations) and physical drone testbed.

An explanation of the algorithm is within the code itself.

This project was developed with Visual Studios on Windows 10 device. 
Visual Studios has a handful of files that it created when the code was being developed.
These have been omitted.

There are 4 files:
- source.cpp - This file has the "main" function and its only purpose is to demonstrate the other files
- LOS.h - This header file declares a class and a struct that will be used heavily. Functions are defined in defineLOS.cpp
- defineLOS.cpp - This file is the real meat of this small project. Functions declared in LOS.h are defined here
- sc_orbits.txt - This text file has all of the 3D position vectors over time for each spacecraft (including carrier)
		  The first row of the file has three numbers that are: # of spacecraft, size of timestep, and # of time samples
	  	  This file format was used earlier in the project by Sam.

Be aware that these files were written and tested with Visual Studio. 
Hopefully there won't be any issues running it elsewhere.

If you have questions, concerns, complaints, jokes, or issues about/with the code,
email me at my Mines school email: western@mymail.mines.edu