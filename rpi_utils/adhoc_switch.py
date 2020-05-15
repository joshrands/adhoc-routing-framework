import RPi.GPIO as GPIO
import time
import subprocess
import socket

# Setup
GPIO.cleanup()
GPIO.setmode(GPIO.BCM)

# Variables
adhoc_default = 0 # wifi default if 0 else adhoc is default
 
password = 'smallsat'
adhoc_network_name = 'SmallSat'
wifi_network_name = 'CSMwireless'
interface = 'wlan0'
state_file = '/home/pi/adhoc-routing-framework/rpi_utils/last_setting.txt'
adhoc_state = 0
states_w_i = {"adhoc" : (adhoc_default % 2), "wifi" : (adhoc_default + 1) % 2}
states_i_w = {(adhoc_default % 2) : "adhoc", (adhoc_default + 1) % 2 : "wifi"}

def bash(*commands):
    """
    Runs entered commands in bash as sudo
    """
    for command in commands:
        subprocess.run('echo {password} | sudo -S {command}'.format(password=password,command=command), shell=True, check=True)


def set_adhoc():
    bash("cp /etc/network/interfaces_adhoc /etc/network/interfaces","reboot") 


def set_wifi():
    bash("cp /etc/network/interfaces_wifi /etc/network/interfaces","reboot")


def get_last_setting(current):
    global adhoc_state
    try:
        with open(state_file, 'r') as fin:
            last = fin.readline() 
            print("Last setting was:", last)
            if(last == 'adhoc' or last == 'wifi'):
                adhoc_state = states_w_i[last]
                time.sleep(1)
            else:
                print("[ERROR]: Could not load in last setting {} using current {}".format(last, current))
                adhoc_state = current
    except FileNotFoundError:
        print("[ERROR]: Could not load in last setting file not found using current {}".format(current))
        adhoc_state = current


def save_setting():
    with open(state_file, 'w') as fout:
        fout.write(states_i_w[adhoc_state])
    

if __name__=="__main__":
    adhoc_pin = 3
    GPIO.setup(adhoc_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    time.sleep(1) 
    get_last_setting(GPIO.input(adhoc_pin)) 
    save_setting()

    while True:
        current_state = GPIO.input(adhoc_pin)
        if current_state != adhoc_state:
            adhoc_state = current_state
            if adhoc_state == states_w_i['wifi']:
                print("Switching to WIFI mode...")
                save_setting()
                time.sleep(5)
                set_wifi()
            elif adhoc_state == states_w_i['adhoc']:
                print("Switching to AD HOC mode....")
                save_setting()
                time.sleep(5)
                set_adhoc()
            else:
                raise "ERROR: Invalid adhoc pin reading: {adhoc_state}".format(adhoc_state=adhoc_state)
        time.sleep(2)

