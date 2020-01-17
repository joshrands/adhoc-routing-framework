#include "adhoc_routing.h"

// Private
void AdhocRouting::_init(){
    routing = nullptr;
    monitor = nullptr;
}

// Constructors
AdhocRouting::AdhocRouting(){
    _init();
}

AdhocRouting::AdhocRouting(RoutingProtocol* routing){
    _init();
    this->routing = routing;
}

AdhocRouting::AdhocRouting(NetworkMonitor* monitor){
    _init();
    this->monitor = monitor;
}

AdhocRouting::AdhocRouting(RoutingProtocol* routing, NetworkMonitor* monitor){
    _init();
    this->routing = routing;
    this->monitor = monitor;
}

void AdhocRouting::addPort(Port* p){
    p->setAdhocRouting(this);
    this->ports.push_back(p);
}

void AdhocRouting::removePort(Port* p){
    for(int i = 0; i < ports.size(); i ++){
        if(ports.at(i) == p){
            ports.erase(ports.begin()+i);
        }
    }
}