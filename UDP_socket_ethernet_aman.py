import socket
import time

UDP_IP="192.168.1.177"
UDP_PORT=5002
client_sock=socket.socket(socket.AF_INET , socket.SOCK_DGRAM)
client_sock.bind(('',8890))
while True:
    client_sock.sendto(b'orange', (UDP_IP,UDP_PORT))
    print("Sent")
    rec_data, addr= client_sock.recvfrom(4096)
    print(rec_data)
    print(addr)


