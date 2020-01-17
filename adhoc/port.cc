#include "port.h"

// Private Functions
void Port::_init(){
    this->portId = -1;
    this->routingProtocol = nullptr;
}

// Constructors
Port::Port(){
    _init();
}

Port::Port(int portId){
    _init();
    this->portId = portId;
}

Port::Port(int portId, RoutingProtocol* routingProtocol){
    _init();
    this->portId = portId;
    this->routingProtocol = routingProtocol;
}

// Getters + Setters
void Port::setRoutingProtocol(RoutingProtocol* routingProtocol){
    this->routingProtocol = routingProtocol;
}

void Port::setPortId(int portId){
    this->portId = portId;
}

int Port::getPortId(){
    return portId;
}

