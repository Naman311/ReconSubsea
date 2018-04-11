import socket
import sys
import time

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_address = ('169.254.234.100', 5000)
message = 'Init'

try:

    # Send data
    sent= sock.sendto(bytes(message, "utf-8"), (server_address))

    # Receive response
    while(1):
        data, server = sock.recvfrom(4096)
        print(data.decode())
        time.sleep(1)

finally:
    print('closing socket')
    sock.close()
