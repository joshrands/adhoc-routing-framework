# Ad-hoc Routing Framework for Hardware and Simulation

### NASA SmallSat Project - Colorado School of Mines

Classes designed for physical implementation as well as simulated implementation of an ad-hoc network on a single decentralized subnet.

## Running the code

This codebase is organized into several 'modules'. Modules are code grouped in a subdirectory of the main directory. Each module has been developed with a built in test framework and is compiled into a shared object (.so) file for linking capabilities. 

### Testing Modules

Each module has a 'test.cc' file used for testing. Use the following tests to test a module: 

1. Navigate to the module directory in a terminal
2. Type 'make clean'
3. Type 'make'
4. Type './test'

The tests for this module should run. PASS indicates the test passed and FAIL indicates that the test failed. 

### Using NS3

NS3 is a network simulator. 

At the time of writing, to install the NS3 libraries, you should run:
Install it by following the instructions in the [NS3 tutorial](https://www.nsnam.org/docs/release/3.28/tutorial/html/getting-started.html). The simulator has currently been tested using ns-3.28. 

```bash
git clone https://gitlab.com/nsnam/ns-3-allinone.git
cd ns-3-allinone
python3 download.py -n ns-3.28
cd ns-3.28
./waf configure --build-profile=optimized --enable-examples --enable-tests
./waf
sudo ./waf install
```

#### Add CSM Module and Patches

In order to accomplish network monitoring, some patches and modules were built into the NS3 source code. Download the patches at the following repository and replace the associated files in your local ns-3.28 directory: https://github.com/joshrands/adhoc-routing-framework/tree/master/ns-allinone-3.28/ns-3.28 

## Modules Overview

### ADHOC

Infrastructure for providing ad-hoc communication. 
[code here](adhoc)

### HARDWARE

Hardware interfacing for components of ad-hoc routing.
[code here](hardware)

### SIMULATION

Simulation interfacing for components of ad-hoc routing.
[code here](simulation)

### AODV 

AODV Protocol implemented from AODV RFC3561 (https://www.ietf.org/rfc/rfc3561.txt)
[code here](aodv)

### HELLO_MONITOR

Hello messages to provide primitive network monitoring of one hop neighbors. 
[code here](hello_monitor)

### REM

Implementation of REgression-based network Monitoring (https://dl.acm.org/doi/abs/10.1145/3360774.3360795)
[code here](rem)

### SOCKET

UDP Sockets and helper classes. 
[code here](socket)

## TODOS
- Completely redo the file system and makefiles to use dependencies and actually use the static libraries, the Makefiles currently are awful
  - Use hidden directories to store build files and dependency files
  - Use structures in each file system to hold src (.cc) and include (.h)
  - Make sure all include paths are not relative ("../rssi/")
  - Use -L -l to link the static libraries
  - Figure out how to avoid recompiling every file every time we make
- Fully doxygen document the code base
- Unify the Style to use one style guideline
- Create stoppable threads
