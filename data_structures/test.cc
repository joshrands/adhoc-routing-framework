#include <assert.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <utility>

#include "safe_circular_queue.h"
using namespace std;

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string END = "\033[0m\n";

void test(bool condition, string desc)
{
	if (condition)
		cout << GREEN << "[PASS]: " << desc << END;
	else 
		cout << RED << "[FAIL]: " << desc << END;
}

void test(bool condition, const char *desc) {
    if (condition) {
        cout << GREEN << "[PASS]: " << desc << END;
    } else
        cout << RED << "[FAIL]: " << desc << END;
}

bool getYesNo(string message) {
    cout << message << "(y/n): ";
    string response;
    cin >> response;
    for (auto &c : response) {
        c = tolower(c);
    }
    return (response == "y" || response == "yes");
}

void testCircularQueue() {
    SafeCircularQueue<int> q(5);
    // Cannot pop empty queue

    // push 3 things on, pull them off check that they are good
    for (int i = 0; i < 5; i++) {
        q.push(i);
    }
    for (int i = 0; i < 5; i++) {
        int elem;
        q.pop(elem);
        test(elem == i, "Basic pushing and popping");
    }
    test(q.empty(), "Queue is empty after all elements popped");
    test(!q.pop(), "Cannot pop empty queue");
    // Do that again check for wrapping
    for (int i = 0; i < 3; i++) {
        q.push(i);
    }
    for (int i = 0; i < 3; i++) {
        int elem;
        q.pop(elem);
        test(elem == i, "Wrapping during pushing and popping");
    }
    test(q.empty(), "Queue is empty after all elements popped");
    test(!q.pop(), "Cannot pop empty queue");

    // Filler up, fill some more see what happens (it should overwrite the
    // oldest data but still access oldest to youngest)
    for (int i = 0; i < 7; i++) {
        q.push(i);
    }
    for (int i = 2; i < 7; i++) {
        int elem;
        q.pop(elem);
        test(elem == i, "Handles overfilling correctly");
    }
    test(q.empty(), "Queue is empty after all elements popped");
    test(!q.pop(), "Cannot pop empty queue");
}

int main() {
    // Test circular queue
    { testCircularQueue(); }
    printf("________________________________\n\n");

    return 0;
}