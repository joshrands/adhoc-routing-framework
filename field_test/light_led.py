# main.py
import sys
import RPi.GPIO as GPIO
import time

if __name__ == "__main__":
    print("Lighting up led at pin {} for {} ms".format(sys.argv[1], sys.argv[2]))
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(int(sys.argv[1]),GPIO.OUT)
    GPIO.output(int(sys.argv[1]),GPIO.HIGH)
    time.sleep(float(sys.argv[2])/1000)
    GPIO.output(int(sys.argv[1]), GPIO.LOW)
    GPIO.cleanup()
