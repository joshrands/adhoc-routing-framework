# Ad hoc Network Routing Protocol Implementations

### NASA SmallSat Project - Colorado School of Mines

Classes designed for physical implementation and simulated implementation of an ad-hoc network on a single decentralized subnet.

Author: Josh Rands

## To Do
1. Make update local models abstract? Or at least make time match current system some how... 
2. Update aodv get neighbors when local models are updated
3. Move aodv updateNeighbors to generic NetworkMonitor
4. Create ROS Node Wrapper
5. Implement hardware monitoring

### Bugs

0. Aodv's decodeReceivedPacket does not care about ports... if a data packet is sent with 0x01 as the first byte it will be classified as a RREQ message. 

### AODV 

AODV Protocol: https://www.ietf.org/rfc/rfc3561.txt
[code here](aodv)

### SOCKET
[code here](socket)

### ADHOC-HARDWARE
[code here](hardware)

### AODV-REM
1. Add network monitoring 

### ns3 Simulation
1. Create a module with this aodv
  - https://www.nsnam.org/docs/manual/html/new-modules.html

## Running the code

This code is currently stand alone classes, but I added test.cc to so we can test while we develop. 

1. Clone this repository 
2. Using a terminal, type 'make clean-all & make all'
3. Code should compile and you can run ./do-adhoc to start doing ad hoc! 

I use Visual Studio Code to develop. It has a nice built in terminal. <- Cool story bro <<-- Thanks mayn 


## Questions
[better route vs duplicate?](aodv/aodv_rreq.cc:57)
