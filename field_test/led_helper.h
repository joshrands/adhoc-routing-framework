#include <sstream>
#include <string>

/**
 * @brief Turns the led on for a specified duration and then off, the led pin must be setup before calling this function
 * 
 * @param pin the pin the led is on
 * @param duration_ms how many seconds the led should be on for
 */
void lightLed(int pin, uint duration_ms);