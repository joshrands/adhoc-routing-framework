# Data Structures

### NASA SmallSat Project - Colorado School of Mines

Name | Email | Cell
--- | --- | ---
Zachary Smeton | zsmeton@yahoo.com | (303) 880-9437


## Description
Collection of thread safe data structures which are used throughout the project.

## To-Do
0. Add tests for safe queue and safe hash map


## How to test this code:
0. In a terminal, cd into the socket directory
1. Type 'make clean' to remove old binaries
2. Type 'make' to compile the  code
3. Type './test' to run the test code. 
4. Check the the result of [TEST] print outs

## How to use this code:
0. Include the directory in the build using ```-I${GLOBAL OR RELATIVE PATH TO data_structures}```

## File Insights
[circular queue](circular_queue.h) - Implementation of a circular queue where when the queue is full new data overwrites previous data

[safe circular queue](safe_circular_queue.h) - Wrapper of [circular queue](circular_queue.h) which is thread safe

[safe queue](safe_queue.h) - Wrapper of std::queue which is thread safe (can be swapped in instead of circular queue in threaded receiving)

[safe hash map](safe_hash_map.h) - A concurrent thread-safe hash map implemented in C++. Created by Kaushik Basu [here](https://github.com/kshk123/hashMap)

[safe hash node](safe_hash_node.h) - A helper class for the hash map implemented in C++. Created by Kaushik Basu [here](https://github.com/kshk123/hashMap)

[test](test.cc) - Tests the data structures


## Usage
### Safe Circular Queue
"Circular Queue is a linear data structure in which the operations are performed based on FIFO (First In First Out) principle and the last position is connected back to the first position to make a circle. It is also called ‘Ring Buffer’." This implementation will overwrite the contents when the queue is full using a FIFO replacement policy.

```c++
// Creation of int circular queue of size 5
SafeCircularQueue<int> q(5);
// To check if empty
q.empty();
// To push
q.push(1);
// To pop without getting element
q.pop();
// To pop and get element
int i;
q.pop(i);
// To get first element without popping
q.peek(i);
```

### Safe Queue
Is a thread safe implementation of the c++ queue using mutexes.

```c++
// Creation of int queue
SafeQueue<int> q;
// To check if empty
q.empty();
// To push
q.push(1);
// To pop without getting element
q.pop();
// To pop and get element
int i;
q.pop(i);
// To get first element without popping
q.peek(i);
```
