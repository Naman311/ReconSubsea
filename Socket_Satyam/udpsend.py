import random
import socket
import multiprocessing 
UDP_IP="127.0.0.1"
sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind(('',8888))
while True:
    data=b'sdfsds'
    sock.sendto(data,(UDP_IP,8888))
                
