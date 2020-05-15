# Hardware AODV

### NASA SmallSat Project - Colorado School of Mines

Name | Email | Cell
--- | --- | ---
Zachary Smeton | zsmeton@yahoo.com | (303) 880-9437


## Description
Hardware implementation of the Ad-hoc On demand Distance Vector routing protocol. Mostly developed for RPis in AD-HOC mode


## To-Do
0. Look at threading the socket send as the time to handle packets might be > hello_monitor neighbor ttl 
1. Complete todos in [hardware_rssi](hardware_rssi.cc)


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
[hardware_defines](hardware_defines.h) - Defines used for debugging, logging, and hyperparameters for the code. 

[hardware_aodv](hardware_aodv.h) - Hardware implementation of [aodv](../aodv/aodv.h) using udp sockets defined [here](../socket/udp_socket.h)

[hardware_hello](hardware_hello.h) - Hardware implementation of [hello_monitor](../hello_monitor/hello_monitor.h) 

[hardware_hello_aodv](hardware_hello_aodv.h) - Implementation of [hardware_aodv](hardware_aodv.h) with [hello monitoring](hardware_hello.h) integrated

[hardware_rssi](hardware_rssi.h) - A class which monitors the RSSI of different nodes using packet sniffing

[test](test.cc) - Runs hardware AODV (does not really test functionality just good to test peice of mind that the code doesn't break immediately). It also tests some other functionality.

## Usage
### Hardware Aodv / Hardware Hello Aodv
```c++
// Create instance
RoutingProtocol* haodv = new HardwareHelloAODV(MY_IP_ADDR);
// Create any desired ports 
Port* printPort = new PrintPort(DATA_PORT);
...
// Add the port(s) to hardware aodv
haodv->addPort(printPort);
...
// Send packet(s) through haodv using port id
haodv->sendPacket(printPort, msg, message.length(), getIpFromString("192.168.1.1"))
...
// Make sure to call handle packets as often as possible
// Should try to call handle packets more than send packets
for(;;){
  haodv->handlePackets();
}
```

### Hardware RSSI
You will need to run the resulting code as sudo, and make sure that the interface being used as set in [defines](../defines.h) is in monitor mode, is seperate from the interface being used to run aodv, and that the interface is turned on.

Example commands to do this:
```bash
$ ifconfig  # WIll give names of interfaces, get name of panda dongle
$ INTERFACE_NAME=panda wireless dongle name
$ sudo ifconfig ${INTERFACE_NAME} down
$ sudo iwconfig ${INTERFACE_NAME} mode monitor
$ sudo ifconfig ${INTERFACE_NAME} up
```

There are two modes, selective and collect all:

If selective: only collects rssi data on the neighbor list
```c++
// Create hardware rssi instance
HardwareRSSI* hRSSI = new HardwareRSSI(false);
// Start capturing data
thread capturing = thread(&HardwareRSSI::captureData, hRSSI);
capturing.detach();
// Add neighbor(s) to monitor
hRSSI->addNeighbors("7c:9a:54:ea:8e:97", getIpFromString("192.168.1.2"));
...
// Get rssi data
hRSSI->getRSSI("7c:9a:54:ea:8e:97");
// Or
hRSSI->getRSSI(getIpFromString("192.168.1.2"));
```

If collect all: collects rssi data on all packets. Can only query on mac address if node not added to neighbor list manually. This is because the captured packets only contain mac address data not IP data...
```c++
// Create hardware rssi instance
HardwareRSSI* hRSSI = new HardwareRSSI(true);
// Start capturing data
thread capturing = thread(&HardwareRSSI::captureData, hRSSI);
capturing.detach();
// Add neighbor(s) to monitor
hRSSI->addNeighbors("7c:9a:54:ea:8e:97", getIpFromString("192.168.1.2"));
...
// Get rssi data
hRSSI->getRSSI("7c:9a:54:ea:8e:97");
// Or
hRSSI->getRSSI(getIpFromString("192.168.1.2"));
```
