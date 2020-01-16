#include "port.h"

// Private Functions
void Port::_init(){
    this->portId = -1;
    this->adhocRouting = nullptr;
}

// Constructors
Port::Port(){
    _init();
}

Port::Port(int portId){
    _init();
    this->portId = portId;
}

Port::Port(int portId, AdHocRouting* adhocRouting){
    _init();
    this->portId = portId;
    this->adhocRouting = adhocRouting;
}

// Getters + Setters
void Port::setAdHocRouting(AdHocRouting* adhocRouting){
    this->adhocRouting = adhocRouting;
}

void Port::setPortId(int portId){
    this->portId = portId;
}

int Port::getPortId(){
    return portId;
}

