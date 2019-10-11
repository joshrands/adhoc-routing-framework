import socket
from time import sleep, time 

IN_BROADCAST_PORT = 58083
 
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', IN_BROADCAST_PORT))

r = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
r.bind(('', 0))

# Listen to port
received, addr = s.recvfrom(256)

if received:
    counter = 0
    while counter < 2:
        counter += 1
        print(received, addr)
        print("Sending reply...")
        data = b"Hello Broadcaster!\0"
        s.sendto(data, addr)
        sleep(1)

