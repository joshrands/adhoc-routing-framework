# main.py
import sys
import RPi.GPIO as GPIO
import time

if __name__ == "__main__":
    print(f"Lighting up led at pin {sys.argv[1]} for {sys.argv[2]} ms")
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(sys.argv[1],GPIO.OUT)
    GPIO.output(sys.argv[1],GPIO.HIGH)
    time.sleep(sys.argv[2]/1000)
    GPIO.output(sys.argv[1], GPIO.LOW)
    GPIO.cleanup()
