#include "hardware_led_aodv.h"

HardwareLedAODV::HardwareLedAODV() : HardwareHelloAODV() {}

HardwareLedAODV::HardwareLedAODV(uint32_t ip) : HardwareHelloAODV(ip) {}

HardwareLedAODV::HardwareLedAODV(const char* ip) : HardwareHelloAODV(ip) {}

HardwareLedAODV::HardwareLedAODV(uint32_t ip, uint64_t helloDurationMS)
    : HardwareHelloAODV(ip) {}

HardwareLedAODV::HardwareLedAODV(const char* ip, uint64_t helloDurationMS)
    : HardwareHelloAODV(ip) {}

HardwareLedAODV::~HardwareLedAODV() {}

int HardwareLedAODV::handlePackets() {
  int count = 0;
  Message message;
  // Handle aodv packets
  while (aodvSocket->getMessage(message)) {
    _handleAODVPacket(message.getData(), message.getLength(),
                      message.getAddressI());
    helloMonitor->receiveHelloMessage(message.getAddressI());
    count++;
    
    // Light up the led if not a message from ourselves
    if(message.getEndpoint().getAddressI() != this->ipAddress){
      if(AODV_LED_ON)
        lightLed(AODV_LED_PIN, LIGHT_MS);
      if(LED_AODV_DEBUG)
        printf("[LED AODV]:[DEBUG]: Received AODV message from %s\n", message.getEndpoint().getAddressC());
    }else{
      if(LED_AODV_DEBUG)
        printf("[LED AODV]:[DEBUG]: Received AODV message from self\n");
    }
  }

  // Handle packets on the ports
  for (auto socketPair : portSockets) {
    while (socketPair.second->getMessage(message)) {
      this->_handlePacket(socketPair.first, message.getData(),
                          message.getLength(), message.getAddressI());
      helloMonitor->receiveHelloMessage(message.getAddressI());
      count++;

      // Light hello led if hello
      if(socketPair.first == HELLO_PORT){
        if(message.getEndpoint().getAddressI() != this->ipAddress){
          if(HELLO_LED_ON)
            lightLed(HELLO_LED_PIN, LIGHT_MS);
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received HELLO message from %d (self: %d)\n", message.getEndpoint().getAddressI(), this->getIp());
        }else{
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received HELLO message from self\n");
        }
      }else{
        if(message.getEndpoint().getAddressI() != this->ipAddress){
          if(OTHER_LED_ON)
            lightLed(OTHER_LED_PIN, LIGHT_MS);
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received DATA message from %s\n", message.getEndpoint().getAddressC());
        }else{
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received message from self\n");
        }
      }
    }
  }
  return count;
}