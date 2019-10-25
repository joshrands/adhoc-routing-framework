#include "hardware_aodv.h"
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include <time.h>
using namespace std;

void test(bool condition, char *message) {
  printf("%s: ", message);
  assert(condition);
  cout << "PASS" << endl;
}

void test(bool condition, string message) {
  cout << message << ": ";
  assert(condition);
  cout << "PASS" << endl;
}



int main() {
  // Test initialization and ports
  {
    HardwareAODV haodv(getIpFromString("127.0.0.1"));
    char* msg = (char*)(malloc(16));
    string message = "Hello World!";
    memcpy(msg, &message, 16);

    haodv.sendPacket(msg, 16, getIpFromString("127.0.0.1"));
    haodv.sendPacket(msg, 16, getIpFromString("127.0.0.1"));
//    haodv.~HardwareAODV();
  }

  printf("________________________________\n\n");
  // Test messages can be sent
  {
    
  }

  printf("________________________________\n\n");
  // Test communication between sockets
  {
    
  }
  return 0;
}