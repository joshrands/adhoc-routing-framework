import RPi.GPIO as GPIO
import time
import subprocess
import socket

# Setup
GPIO.cleanup()
GPIO.setmode(GPIO.BCM)

# Variables
password = 'smallsat'
adhoc_network_name = 'SmallSat'
wifi_network_name = 'CSMwireless'
interface = 'wlan0'


def bash(*commands):
    for command in commands:
        subprocess.run(f'echo {password} | sudo -S {command}', shell=True, check=True)


def set_adhoc():
    host_name = socket.gethostname()
    bash(f"ifconfig {interface} down", f"iwconfig {interface} channel 1 essid {adhoc_network_name} mode ad-hoc", f"ifconfig {interface} up", f"ifconfig {interface} 192.168.1.{host_name[-1]} netmask 255.255.255.0") 


def set_wifi():
    bash(f"ifconfig {interface} down", f"iwconfig {interface} essid {wifi_network_name}", f"ifconfig {interface} up")


if __name__=="__main__":
    adhoc_pin = 3
    GPIO.setup(adhoc_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

    adhoc_state = 1

    while True:
        current_state = GPIO.input(adhoc_pin)
        if current_state != adhoc_state:
            adhoc_state = current_state
            if adhoc_state == 1:
                print("WIFI")
                set_wifi()
            elif adhoc_state == 0:
                print("AD HOC")
                set_adhoc()
            else:
                raise f"ERROR: Invalid adhoc pin reading: {adhoc_state}"
        time.sleep(2)

