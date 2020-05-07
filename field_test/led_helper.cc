#include "led_helper.h"

void lightLed(int pin, uint duration_ms, uint count){
  if(duration_ms > 0 && count > 0){
    std::stringstream commandStream;
    commandStream << "/usr/bin/python3.5 /home/pi/adhoc-routing-framework/field_test/light_led.py " << pin << " " << duration_ms << " " << count << " &";
    system(commandStream.str().c_str());
  }
}