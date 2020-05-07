#include <sstream>
#include <string>

/**
 * @brief Turns the led on for a specified duration and then off
 * 
 * @param pin the pin the led is on
 * @param duration_ms how many seconds the led should be on for
 * @param count how many times to light up the led (default=1)
 */
void lightLed(int pin, uint duration_ms, uint count=1);