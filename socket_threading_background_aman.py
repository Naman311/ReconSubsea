import socket
import threading
import time


UDP_IP="192.168.1.177"
UDP_PORT=5002
client_sock=socket.socket(socket.AF_INET , socket.SOCK_DGRAM)

def data_transfer(interval):
    while True:
        client_sock.sendto(b'orange', (UDP_IP,UDP_PORT))
        rec_data, addr = client_sock.recvfrom(4096)
        print(rec_data)
        print(addr)
        time.sleep(interval)

def __init__():
    thread = threading.Thread(target=data_transfer(1), args=())
    thread.daemon = True
    thread.start()

st=__init__()
time.sleep(3)
print("from now the code will run in background")
    
    
