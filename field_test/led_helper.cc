#include "led_helper.h"

void lightLed(int pin, uint duration_ms){
  std::stringstream commandStream;
  commandStream << "python3 light_led.py " << pin << " " << duration_ms;
  system(commandStream.str().c_str());
}