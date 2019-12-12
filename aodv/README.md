# Generic AODV

### Colorado School of Mines NASA SmallSat Project

Abstract implementation of the Ad-hoc On demand Distance Vector routing protocol. It can be used in simulation as well as on a physical testbed by implementing the SocketSendData function to send data across the desired medium.

Modeled after RFC 3561: https://www.ietf.org/rfc/rfc3561.txt

## To-Do
1. Add ttl for sent packets and remove packets after time  
2. Create function for sending all buffered packets for a destination IP 
3. Create a function for creating new bufferedPacket in map 

## Latest Features
- Changed sequence number for duplicate RREQ to check set of past received sequences


## How to test this code:
1. In a terminal, cd into the aodv directory
2. Type 'make clean' to remove old binaries
3. Type 'make' to compile the  code
4. Type './test' to run the test code. 
5. If the file makes it to the end all tests passed. 

## How to use this code:
1. After typing 'make' this code is compiled to a static library in 'libaodv.a'
2. This library can be compiled with your source code using the following syntax: 
  'g++ test.o -o test libaodv.a 

## File Insights

[RoutingProtocol](RoutingProtocol.h) - Base class for a routing protocol with basic functionality that all routing protocols should have. 

[aodv](aodv.h) - Abstract AODV class. Brings everything together to make AODV happen. The AODV class itself is abstract because there needs to be a method for transmitting data. To create an implementation of AODV, inherit from this class and implement the virtual function socketSendData. AODVTest is a basic example of how to do this. 

[aodv_packets](aodv_packets.h) - Basic structs to represent the different types of packets sent during aodv. 

[aodv_rreq](aodv_rreq.h) - Route Request message. The RREQHelper class is used by aodv to manage RREQ messages. This class can create RREQs, forward them, decode them, etc. 

[aodv_rrep](aodv_rrep.h) - Route Reply message. The RREPHelper class is used by aodv to manage RREP messages. This class can create RREPs, forward them, decode them, etc. 

[aodv_rerr](aodv_rerr.h) - Route Error Message. The RERRHelper class is used by aodv to manage RERR messages. This helper will also take advantage of network monitoring to determine link breakage and attempt local repairs. 

[aodv_routing_table](aodv_routing_table.h) - The routing table for AODV. Stores information such as next hop, time to live, sequence numbers, and more. 

[aodv_defines](aodv_defines.h) - Global defines used for debugging, logging, etc. 

[aodv_sim](aodv_sim.h) - An implementation of AODV that can be used in Network Simulator 3 when binded to a socket send data function in the simulator. 

[send_packet](send_packet.h) - Old stuff used for fake sending data. 

[test](test.cc) - For testing purposes only. Uses aodv functionality for simple simulated routing. 
