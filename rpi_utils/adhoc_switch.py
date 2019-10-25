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
    """
    Runs entered commands in bash as sudo
    """
    for command in commands:
        subprocess.run('echo {password} | sudo -S {command}'.format(password=password,command=command), shell=True, check=True)


def set_adhoc():
    host_name = socket.gethostname()
    bash("ifconfig {interface} down".format(interface=interface), "iwconfig {interface} channel 1 essid {adhoc_network_name} mode ad-hoc".format(interface=interface,adhoc_network_name=adhoc_network_name), "ifconfig {interface} up".format(interface=interface), "ifconfig {interface} 192.168.1.{sub} netmask 255.255.255.0".format(interface=interface, sub=host_name[-1])) 


def set_wifi():
    bash("ifconfig {interface} down".format(interface=interface), "iwconfig {interface} essid {wifi_network_name}".format(interface=interface, wifi_network_name=wifi_network_name), "ifconfig {interface} up".format(interface=interface))


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
                raise "ERROR: Invalid adhoc pin reading: {adhoc_state}".format(adhoc_state=adhoc_state)
        time.sleep(2)

