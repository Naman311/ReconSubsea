import socket
import pickle
import _thread as t
import time
host = ('192.168.1.167',5002)

global conn,addr
print("xxxxx")
sock2=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock2.bind(host)
sock2.listen(5)
conn,addr=sock2.accept()
#print("a")
def receive():
    #print("b")
    sock1=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    sock1.connect(('192.168.1.168',5001))
    while True:
        msg=sock1.recv(1024)
        msg1 = pickle.loads(msg)
        print("aman : ",msg1)
        time.sleep(.01)
def send():    
    while True:
        list1 = ['a','v','c','r',5.6,7,2]
        s = str(list1).strip('[]')
        data = pickle.dumps(s)
        conn.send(data)
        time.sleep(.01)
t.start_new_thread(receive,())
t.start_new_thread(send,())
