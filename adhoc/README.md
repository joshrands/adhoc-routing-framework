# ADHOC

### NASA SmallSat Project - Colorado School of Mines

Name | Email | Cell
--- | --- | ---
Zachary Smeton | zsmeton@yahoo.com | (303) 880-9437
Joshua Rands | joshrands1@gmail.com

## Description

Set of classes and functionality which should be used as skeletons for the routing protocols

## To-Do
1. Create Socket base class and modify Port to both handle and get messages using a Socket member

## How to test this code:
0. Well, we don't have any tests for this code....

## How to use this code:
1. After typing 'make' this code is compiled to a static library in 'libadhoc.a'
2. This library can be compiled with your source code using the following syntax: 
  'g++ test.o -o test libadhoc.a

## File Insights
[adhoc_defines](adhoc_defines.h) - Global defines used for debugging, logging, and hyperparameters for the code. 

[monitor_info](monitor_info.h) - Structs for local and pair monitoring data.

[network_monitor](network_monitor.h) - Base class for all network monitoring classes.

[routing_protocol](routing_protocol.h) - Base class for all routing protocol classes and routing tables.

[port](port.h) - Base class for ports in routing protocol (more info in usage).

[print_port](print_port.h) - Implementation of port which just prints out packet contents.

## Usage
The whole idea for our adhoc routing protocol framework is built on two things the routing protocol and ports. The routing protocol is responsible for routing packets between nodes and then delivering them to the correct port. The ports are the main interface to the routing protocol, they are responsible for handling packets.

### Routing Protocol
To create a custom routing protocol using this base class, five things must be implemented:
- ```int handlePackets()```: This function is responisble for handling routing and data packets. Routing packets should be used to update the routing table and anything else required to route packets and this should also give data to ports to handle using Port::handlePacket(). Usually routing protocol related headers should be removed from the packet before giving it to the port.
- ```bool sendPacket(int portId, char* data, int length, IP_ADDR dest, IP_ADDR origIP)```: This function should send the data to the destination IP from the origIP, which will usually be -1 so ```this->ipAddress```. It should send the data through the port specified in portId. 
- ```bool linkExists(IP_ADDR dest)```: This function should tell wether their is a link to the destination address
- ```void _buildPort(Port*)```: is called on ```addPort()``` and should handle the creation of any port related infrastructure. Usually this would be creating a UDP socket for hardware or NS3 Socket for simulation.
- ```void _destroyPort(Port*)```: is called on ```removePort()``` and should handle removing any infrastructure for the port. Usually the removal and destruction of the UDP/NS3 socket

### Port
The port class is resposible for handling any packets which are ment for this port. To use this base class one function must be implemented:
- ```void handlePacket(char* data, int length, IP_ADDR source)```: This function should handle the packet. EX. for a printing port this would print out the contents of the packet and the source, for monitoring port this would update statistics on pair or local data