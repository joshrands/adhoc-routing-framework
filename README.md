# Ad hoc Network Routing Protocol Implementations

### NASA SmallSat Project - Colorado School of Mines

Classes designed for physical implementation and simulated implementation of an ad-hoc network on a single decentralized subnet.

Author: Josh Rands

## To Do

### AODV 

AODV Protocol: https://www.ietf.org/rfc/rfc3561.txt

1. Add functions for generating RREQ
2. Add functions for generating RREP
  - Create struct for packet format (section 5.2)
  - Add check in RREQ if time for RREP (section 6.6)
  - Unicast RREP message back to orig (section 6.7)
3. Add functions for generating RERR
  - Create struct for packet format (section 5.3)
  - Generate RERR in all 3 situations (section 6.11)
4. Add TTL for RREQ dissemination (section 6.4)
5. Add AODVRoutingTable updates 

### HARDWARE AODV
1. Make a Daemon to accept network packets
  - https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux
2. Use AODV object for routing table
3. Use AODV packets
4. Circular queue (max packets accepted)
5. Check ip address to see if it ourselves
6. Global debug definition
7. Find the communication rate (use different numbers of pis)
8. Packet sniffing
9. AODV switch script
10. Auto login
11. Set up nice readme and comment alot of things
12. 

### AODV-REM
1. Add network monitoring 

### ns3 Simulation
1. Create a module with this aodv
  - https://www.nsnam.org/docs/manual/html/new-modules.html

## Running the code

This code is currently stand alone classes, but I added test.cc to so we can test while we develop. 

1. Clone this repository 
2. Using a terminal, cd into the src directory and type 'make'
3. Code should compile and you can run ./test to run the tests

I use Visual Studio Code to develop. It has a nice built in terminal. 
