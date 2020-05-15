# Network Monitoring 

### NASA SmallSat Project - Colorado School of Mines

Name | Email | Cell
--- | --- | ---
Joshua Rands | joshrands1@gmail.com


## Description

Abstract implementation of network monitoring for virtual and physical environments. 

## To-Do
0. Start a thread for locally available network monitoring 
1. Add call to store RSS in routing receive packet 

## How to test this code:
1. In a terminal, cd into the aodv directory
2. Type 'make clean' to remove old binaries
3. Type 'make' to compile the  code
4. Type './test' to run the test code. 
5. [PASS] indicates a passed test and [FAIL] indicates a failed test

## How to use this code:
1. After typing 'make' this code is compiled to a static library in 'libmonitoring.a'
2. This library can be compiled with your source code using the following syntax: 
  'g++ test.o -o test libmonitoring.a 

## File Insights

[REM](rem.h) - The software implementation of the REgression-based Network Monitoring algorithm 
