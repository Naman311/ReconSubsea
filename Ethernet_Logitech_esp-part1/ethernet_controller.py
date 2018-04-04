import time
import pygame
import socket
import sys

count=0
# Create a UDP socket
laststate=0
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
    global count,laststate
    out = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0] #17 values
    it = 0 #iterator
    pygame.event.pump()
    
    #Read input from the two joysticks       
    for i in range(0, j.get_numaxes()):
        out[it] = round(j.get_axis(i))
        it+=1
    #Read input from button
    for i in range(0, j.get_numbuttons()):
        out[it] = j.get_button(i)
        it+=1
    if(out[1]==0.1 or out[1]==-0.1):
        out[1]=0.0
    if(out[0]==0.1 or out[0]==-0.1):
        out[0]=0.0
    if(out[3]==0.1 or out[3]==-0.1):
        out[3]=0.0
    if(out[4]==0.1 or out[4]==-0.1):
        out[4]=0.0
    if(out[9]==1):
        if(laststate==0):
            count+=1
            laststate=1
            if(count>3):
                count=0
    if(out[9]==0):
        laststate=0
    #print(count)
    out[9]=count
    s=str(out).strip('[]')
    #print(s)
    return s
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while True:    
    # Send data
    #get()
    sent= sock.sendto(bytes(get(), "utf-8"), (server_address))
    #while True:
    try:
        # Receive response
        data, server = sock.recvfrom(4096)
        print(data.decode())
    except:
        #print("Data is passed")
        pass
