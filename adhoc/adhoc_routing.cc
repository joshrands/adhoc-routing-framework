#include "adhoc_routing.h"

// Private
void AdHocRouting::_init(){
    routing = nullptr;
    monitor = nullptr;
}

// Constructors
AdHocRouting::AdHocRouting(){
    _init();
}

AdHocRouting::AdHocRouting(RoutingProtocol* routing){
    _init();
    this->routing = routing;
}

AdHocRouting::AdHocRouting(NetworkMonitor* monitor){
    _init();
    this->monitor = monitor;
}

AdHocRouting::AdHocRouting(RoutingProtocol* routing, NetworkMonitor* monitor){
    _init();
    this->routing = routing;
    this->monitor = monitor;
}

void AdHocRouting::addPort(Port* p){
    p->setAdHocRouting(this);
    this->ports.push_back(p);
}

void AdHocRouting::removePort(Port* p){
    for(int i = 0; i < ports.size(); i ++){
        if(ports.at(i) = p){
            ports.erase(ports.begin()+i);
        }
    }
}