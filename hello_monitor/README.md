# Hardware AODV

### NASA SmallSat Project - Colorado School of Mines

Name | Email | Cell
--- | --- | ---
Zachary Smeton | zsmeton@yahoo.com | (303) 880-9437


## Description
Hello message based monitoring. Which sends out a message every n milliseconds and then checks that neighboring nodes are connected based on the time since the last received message.


## To-Do


## How to test this code:
0. In a terminal, cd into the hardware directory
1. Type 'make clean' to remove old binaries
2. Type 'make' to compile the  code
3. Type './test' to run the test code. 
4. Check the the result of [TEST] print outs

## How to use this code:
1. After typing 'make' this code is compiled to a static library in 'libhello.a'
2. This library can be compiled with your source code using the following syntax: 
  'g++ test.o -o test libhello.a

## File Insights
[hello_defines](hello_defines.h) - Defines used for debugging, logging, and hyperparameters for the code. 

[hello_monitor](hello_monitor.h) - Class which implements a [monitor](../adhoc/network_monitor.h) which uses hello messages to monitor link states

[test](test.cc) - Tests hello monitor functionality
