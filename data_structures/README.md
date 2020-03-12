# Data Structures

### Colorado School of Mines NASA SmallSat Project
Collection of thread safe data structures which are used throughout the project.


## How to test this code:
0. In a terminal, cd into the socket directory
1. Type 'make clean' to remove old binaries
2. Type 'make' to compile the  code
3. Type './test' to run the test code. 
4. Check the the result of [TEST] print outs

## How to use this code:
1. Include whichever headers you desire and then use them

## File Insights
[circular queue](circular_queue.h) - Implementation of a circular queue where when the queue is full new data overwrites previous data

[safe circular queue](safe_circular_queue.h) - Wrapper of [circular queue](circular_queue.h) which is thread safe

[safe queue](safe_queue.h) - Wrapper of std::queue which is thread safe (can be swapped in instead of circular queue in threaded receiving)

[safe hash map](safe_hash_map.h) - A concurrent thread-safe hash map implemented in C++. Created by Kaushik Basu [here](https://github.com/kshk123/hashMap)

[safe hash node](safe_hash_node.h) - A helper class for the hash map implemented in C++. Created by Kaushik Basu [here](https://github.com/kshk123/hashMap)

[test](test.cc) - Tests udp_socket and circular queue functionality
