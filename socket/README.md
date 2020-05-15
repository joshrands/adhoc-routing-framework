# Sockets

### NASA SmallSat Project - Colorado School of Mines

Name | Email | Cell
--- | --- | ---
Zachary Smeton | zsmeton@yahoo.com | (303)880-9437

## Description

UDP Sockets and helper classes. Sockets can have threaded listening on a port which will load all received data into messages which will be stored in a circular queue which overwrites the oldest data in the queue. 

Modified version of classes from MBED-OS: https://github.com/ARMmbed/mbed-os

## To-Do
0. Rework code to make Message a struct and remove Endpoint or use it more ubiquitously
1. Rework Socket class to be a better base for Port/Routing protocol overhaul described [here](../adhoc/README.md)
2. Make UDP Socket sendPacket() to thread each call

## How to test this code:
0. In a terminal, cd into the socket directory
1. Type 'make clean' to remove old binaries
2. Type 'make' to compile the  code
3. Type './test' to run the test code. 
4. Check the the result of [TEST] print outs

## How to use this code:
1. After typing 'make' this code is compiled to a static library in 'libsocket.a'
2. This library can be compiled with your source code using the following syntax: 
  'g++ test.o -o test libsocket.a'

## File Insights
[endpoint](endpoint.h) - Store an IP address and port.

[message](message.h) - Stores packets and their endpoint.

[socket](socket.h) - Base class for a socket supports socket initialization and setting socket options.

[udp socket](udp_socket.h) - Inherits from socket but implements a UDP Socket with the ability to send packets and safely thread the socket to listen to a specified port.

[socket defines](socket_defines.h) - Global defines used for debugging, logging, also sets the maxSize of the circular queue used in threading.

[test](test.cc) - Tests udp_socket functionality.


## Usage
### UDP Socket
This class can be used to send data over the network using UDP protocol. The receiving of packets is done by a thread which reads in data and then stores the packets in a circular buffer.
```c++
/// Setup
// Create socket
UDPSocket *socket = new UDPSocket();
// Intialize the socket
if(!socket->init()){
    fprintf(stderr, "[ERROR]:Could not initialize the socket\n");
}
// Make the port reusable if desired
if(!socket->setOption(SOL_SOCKET, SO_REUSEPORT, &REUSE_VAL, sizeof(REUSE_VAL))){
    fprintf(stderr, "[ERROR]:Could not set the socket to reuse ports\n");
}
// Bind the port to the desired port
if (!socket->bindToPort(ROUTING_PORT)) {
    fprintf(stderr, "[ERROR]:Could not bind the socket to port:%d\n", ROUTING_PORT);
}
// Make the port support broadcasting
if(!socket->setBroadcasting()){
    fprintf(stderr, "[ERROR]:Could not set the socket to broadcasting\n");
    exit(-1);
}
// Thread the receive call
returnThread = thread(&UDPSocket::receiveFromPortThread, socket);
returnThread.detach();

/// Usage
// Send a packet to ip address 192.168.1.2 on port 5036
Endpoint e;
e.setAddress("192.168.1.2", 5036);
string packet ("Hello 2");
socket->sendTo(e, packet.c_str(), packet.length());

// Receive all messages in the buffer
Message message;
while(socket->getMessage(message)){
    print("%s from %s\n", message.getData(), message.getAddressC());
}
```