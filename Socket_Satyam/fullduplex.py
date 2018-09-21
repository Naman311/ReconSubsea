import socket
from multiprocessing import Process,Queue
import random
import time
recvq=Queue()
sendq=Queue()
ct=time.time()
class Server(object):
    def __init__(self,addr):
        self.addr=addr
    def openserver(self):
        self.server=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.server.bind(('',self.addr[1]))
    def stopserver(self):
        self.server.close()
    def receiveserver(self):
        print(self.server.recv())
class fullduplex(object):
    def __init__(self,addr):
        self.addr=addr
        sendq.put("first send")
    def begin(self):
        server=Server(self.addr)
        sproc=sendprocess(server)
        rproc=recprocess(server)
        #qproc=queueprocess()
        sproc.start()
        rproc.start()
        sproc.join()
        rproc.join()            
        #qproc.start()
class recprocess(Process):
    def __init__(self,server):
        super(recprocess, self).__init__()
        self.server=server
    def run(self):
        while time.time()-ct<30: 
            print("receiving")
            recv,addr=self.sock.recvfrom(4096)
            print(recv,addr)
            #recvq.put(recv)
class sendprocess(Process):
    def __init__(self):
        super(sendprocess,self).__init__()
    def run(self):
        #if not sendq.empty():
        while time.time()-ct<30:
            data=b"something"
            
            print('sent')
"""class queueprocess(Process):
    def __init__(self):
        super(queueprocess,self).__init__()    
    def run(self):
        while True:
            if not recvq.empty():
                recvd=recvq.get()
                sendq.put(self.sendstring(recvd))
            else:
                sendq.put("no message was recv")
    def sendstring(self,recvd):
        return "reconsubsea"""

if __name__=='__main__':
    #f1=fullduplex(("127.0.0.1",8889))
    #f1.begin()    
