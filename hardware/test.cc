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
  if(!condition){
    cout << "FAIL" << endl;
  }
  cout << "PASS" << endl;
}

void test(bool condition, string message) {
  cout << "[TEST]: " << message << ": ";
  if(!condition){
    cout << "FAIL" << endl;
  }
  cout << "PASS" << endl;
}



int main() {
  // Test initialization and ports
  {
    printf("Initializing hardware AODV\n");
    HardwareAODV haodv(getIpFromString("127.0.0.1"));
    printf("________________________________\n\n");

    char* msg = (char*)(malloc(16));
    string message = "Hello World!";
    memcpy(msg, message.c_str(), 16);

    printf("Sending data directly through socket\n");
    haodv.socketSendPacket(msg, 16, getIpFromString("127.0.0.1"), 8080);
    printf("________________________________\n\n");

    printf("Sending data through AODV\n");
    haodv.sendPacket(msg, 16, getIpFromString("127.0.0.1"));
    printf("________________________________\n\n");

    printf("Handle any received packets (there should be none)\n");
    haodv.handleAODVPackets();
    printf("________________________________\n\n");

    //Message temp;
    //haodv.getDataPacket(temp);
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