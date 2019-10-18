#!/bin/bash
# Install required dhcp server
sudo apt-get install isc-dhcp-server

sudo cp /etc/network/interfaces /etc/network/interfaces_backup

host=$(hostname)

echo 

sudo echo "# Include files from /etc/network/interfaces.d:\nsource-directory /etc/network/interfaces.d\n\nauto lo\niface lo inet loopback\niface eth0 inet dhcp\n\nauto wlan0\niface wlan0 inet static\n  address 192.168.1.${host: (-1)}\n  netmask 255.255.255.0\n  wireless-channel 1\n  wireless-essid SmallSat\n  wireless-mode ad-hoc\n"> /etc/network/interfaces

sudo ifdown wlan0
sudo ifup wlan0

sudo echo "ddns-update-style interim;\ndefault-lease-time 600;\nmax-lease-time 7200;\nauthoritative;\nsubnet 192.168.1.0 netmask 255.255.255.0 {\n  range 192.168.1.10 192.168.0.240;\n}\n"> /etc/dhcpd.conf

