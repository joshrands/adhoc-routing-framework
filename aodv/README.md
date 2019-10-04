# Custom AODV

### Colorado School of Mines NASA SmallSat Project

Modeled after RFC 3561: https://www.ietf.org/rfc/rfc3561.txt

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

[RoutingProtocol](aodv/RoutingProtocol.cc) - 

[aodv](aodv/aodv.cc) -

aodv_packet - 
aodv_rreq -
aodv_rrep -
aodv_rerr - 
aodv_routing_table -
defines - 
aodv_sim -

send_packet - 

test - 
