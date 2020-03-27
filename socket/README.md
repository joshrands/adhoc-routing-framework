# Sockets

### Colorado School of Mines NASA SmallSat Project

UDP Sockets and helper classes. Sockets can have threaded listening on a port which will load all received data into messages
which will be stored in a circular queue which overwrites the oldest data in the queue

## To-Do
0. Rework code to make Message a struct and remove Endpoint
1. 

## How to test this code:
0. In a terminal, cd into the socket directory
1. Type 'make clean' to remove old binaries
2. Type 'make' to compile the  code
3. Type './test' to run the test code. 
4. Check the the result of [TEST] print outs

## How to use this code:
1. After typing 'make' this code is compiled to a static library in 'libsocket.a'
2. This library can be compiled with your source code using the following syntax: 
  'g++ test.o -o test libsocket.a 

## File Insights
[endpoint](endpoint.h) - Store an IP address and port

[message](message.h) - Stores packets and their endpoint

[socket](socket.h) - Base class for a socket supports socket initialization and setting socket options

[udp socket](udp_socket.h) - Inherits from socket but implements a UDP Socket with the ability to send packets and safely thread the socket to listen to a specified port

[socket defines](socket_defines.h) - Global defines used for debugging, logging, also sets the maxSize of the circular queue used in threading

[test](test.cc) - Tests udp_socket functionality
