import time
import pygame
import socket
import sys
from threading import Thread
import copy


sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server_address=('192.168.10.2',5000)


#----------------------------------------initialising python---------------------------------

value=list()
pygame.init()
print('PyGame initiated')
j=pygame.joystick.Joystick(0)
j.init()
print('Initialised Joystick : %s'%j.get_name())

#--------------------------------------------------------------------------------------------

#-------------------------------------------Thread for joystick value-------------------------
class joyStick(Thread):
    val=list()
    def __init__(self):
        Thread.__init__(self)
        
    def run(self):
        while True:    
            out = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
            qL=out
            it = 0 #iterator
            pygame.event.pump()
            qL=list()
            #Read input from the two joysticks       
            for i in range(0, j.get_numaxes()):
                out[it] = round(j.get_axis(i),1)
                it+=1
            #Read input from buttons
            for i in range(0, j.get_numbuttons()):
                out[it] = j.get_button(i)
                it+=1
            if(abs(out[1])>abs(out[0])):
                out[0]=0.0
            else:
                out[1]=0.0
            if(abs(out[3])>abs(out[4])):
                out[4]=0.0
            else:
                out[3]=0.0
            if(out[1]==0.1 or out[1]==-0.1):
                out[1]=0.0
            if(out[0]==0.1 or out[0]==-0.1):
                out[0]=0.0
            if(out[3]==0.1 or out[3]==-0.1):
                out[3]=0.0
            if(out[4]==0.1 or out[4]==-0.1):
                out[4]=0.0
            out[1]=out[1]*10
            out[0]=out[0]*10
            out[4]=out[4]*10
            out[3]=out[3]*10
            out[2 ]=out[2]*10
            s=str(out).strip('[]')
            joyStick.val=copy.deepcopy(out)
            #print('ma ka bhosada')
            #print(val)

if __name__=='__main__':
    getLogi=joyStick()
    getLogi.setName('GetVal-1')
    getLogi.start()
    #getLogi.join()
    while True:
        #print(joyStick.val)
        str1=str(joyStick.val).encode('utf-8')
        #sock.sendto
        #print(str1)
        sock.sendto(str1,server_address)
        data=""
        try:
            data,server=sock.recvfrom(4096)
        except:
            print('communication error')
        print(data)
