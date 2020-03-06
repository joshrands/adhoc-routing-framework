#include "hardware_hello.h"

bool HardwareHelloMonitor::_sleep(uint64_t duration_ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    return true;
}

uint64_t HardwareHelloMonitor::_getCurrentTimeMS() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    return (uint64_t) ms.count();
}

// TESTING
bool HHMTest::sleep(uint64_t duration_ms){
    return _sleep(duration_ms);
}

uint64_t HHMTest::getCurrentTimeMS(){
    return _getCurrentTimeMS();
}