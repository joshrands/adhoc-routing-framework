# Hardware AODV

### Colorado School of Mines NASA SmallSat Project

Hardware implementation of the Ad-hoc On demand Distance Vector routing protocol. Mostly developed for RPis in AD-HOC mode

## To-Do
0. Add signal strength metric monitoring on packet receive 
<<<<<<< HEAD
2. Currently data packets that are for us are just printed out by AODV
    - We should create sperate functions for data from AODV, DATA, MONITOR 
    - We need to store data packets that aren't meant for us
3. Add monitoring socket for monitor data
=======
1. Change test to show PASS FAIL on left hand side
    i. Maybe look at coloring...
2. Comment code
3. Change transmission power using ioctl
>>>>>>> be5df44df0646c2a8a71d9c9f37adaf21ebf5bb1

## How to test this code:
0. In a terminal, cd into the hardware directory
1. Type 'make clean' to remove old binaries
2. Type 'make' to compile the  code
3. Type './test' to run the test code. 
4. Check the the result of [TEST] print outs

## How to use this code:
1. After typing 'make' this code is compiled to a static library in 'libhardware.a'
2. This library can be compiled with your source code using the following syntax: 
  'g++ test.o -o test libhardware.a 

## File Insights
[hardware_aodv](hardware_aodv.h) - Hardware implementation of [aodv](../aodv/aodv.h) using udp sockets defined [here](../socket/udp_socket.h)

[hardware_defines](hardware_defines.h) - Global defines used for debugging, logging, and hyperparameters for the code. 

[test](test.cc) - Runs hardware AODV (does not really test functionality just good to test peice of mind that the code doesn't break immediately)
