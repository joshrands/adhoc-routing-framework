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
    char message [16] = "Hello World!";
    haodv.sendPacket(message, 16, getIpFromString("127.0.0.1"));
    
  }

  printf("________________________________\n\n");
  // Test messages can be sent
  {
    /*
    HardwareAODV haodv1(getIpFromString("127.0.0.1"));
    HardwareAODV haodv2(getIpFromString("127.0.0.1"));
    char message [16] = "Hello World!";
    haodv1.sendPacket(message, 16, getIpFromString("127.0.0.1"));
    int counter = 0;
    do{
      sleep(1);
      counter ++;
    }while(haodv2.handleReceivedPackets() == -1 and counter <= 20);
    */
  }

  printf("________________________________\n\n");
  // Test communication between sockets
  {
    
  }
  return 0;
}