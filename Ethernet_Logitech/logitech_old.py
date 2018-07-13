import time
import pygame
import socket
import sys

# Create a UDP socket

server_address = ('169.254.234.100', 5000)
# Initializing controller
pygame.init()
j = pygame.joystick.Joystick(0)
j.init()
print ('Initialized Joystick : %s' % j.get_name())

"""
1.Left analog x axis
2.Left analog y axis
3.LT +ve RT -ve
4.Right y axis
5.Right x axis
6.A
7.B
8.X
9.Y
10.LB
11.RB
12.BACK
13.START
14.LEFT CLICK
15.RIGHT CLICK
"""

def get():
    out = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    it = 0 #iterator
    pygame.event.pump()
    
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
    print(s)
    return s
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while True:
    
    # Send data
    #get()
    #print("x")
    sent= sock.sendto(bytes(get(), "utf-8"), (server_address))
    
    #while True:
    try:
        # Receive response
    #print("try")
        data, server = sock.recvfrom(4096)
    #print('y')
        #print(data.decode())
    except:
    
        print("Data is passed")
        pass
    #sock.close()
    time.sleep(0.1)
