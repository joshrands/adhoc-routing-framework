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
state_file = 'last_setting.txt'
adhoc_state = 0
states_w_i = {"adhoc" : 0, "wifi" : 1}
states_i_w = {0 : "adhoc", 1 : "wifi"}

def bash(*commands):
    """
    Runs entered commands in bash as sudo
    """
    for command in commands:
        subprocess.run('echo {password} | sudo -S {command}'.format(password=password,command=command), shell=True, check=True)


def set_adhoc():
    bash(("mv /etc/network/interfaces_adhoc /etc/network/interfaces",)) 


def set_wifi():
    bash(("mv /etc/network/interfaces_adhoc /etc/network/interfaces",))


def get_last_setting(current):
    with open(state_file, 'r') as fin:
        last = fin.readline()
        if(last == 'adhoc' or last == 'wifi'):
            adhoc_state = states_w_i[last]
        else:
            print("[ERROR]: Could not load in last setting {} using current {}".format(last, current))


def save_setting():
    with open(state_file, 'w') as fout:
        fout.write(states_i_w[adhoc_state])

if __name__=="__main__":
    adhoc_pin = 3
    GPIO.setup(adhoc_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    get_last_setting(states_i_w[GPIO.input(adhoc_pin)])

    while True:
        current_state = GPIO.input(adhoc_pin)
        if current_state != adhoc_state:
            adhoc_state = current_state
            if adhoc_state == 1:
                print("WIFI")
                save_setting()
                set_wifi()
            elif adhoc_state == 0:
                print("AD HOC")
                save_setting()
                set_adhoc()
            else:
                raise "ERROR: Invalid adhoc pin reading: {adhoc_state}".format(adhoc_state=adhoc_state)
        time.sleep(2)

