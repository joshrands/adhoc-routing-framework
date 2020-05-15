# Raspberry Pi Utilities

### NASA SmallSat Project - Colorado School of Mines

Name | Email | Cell
--- | --- | ---
Zachary Smeton | zsmeton@yahoo.com | (303)880-9437

## Description

Set of scripts to aid in the setup and usage of Raspberry Pis.

## To-Do
0. Fix gmail account to allow sending mail through python script
1. Create a setup script which handles setting up the configuration files to make the rpis work well
2. Also possibly a script which can be used to set data in the define files based on the rpi's ip address

## Usage
0. Follow the steps outlined in the RPI Setup document in the SmallSat Documentation on the google drive (Ask Professor Qi Han if you don't have access)

## File Insights
[adhoc_switch](adhoc_switch.py) - Automatically swaps the raspberry pi from wifi to adhoc mode when pin 3 is shorted. Can switch the default by changing ```adhoc_default=0``` to ```adhoc_default=1```

[startup_mailer](startup_mailer.py) - Sends out an email to smallsat@gmail.com with the IP address of the RPI


