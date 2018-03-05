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
out = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]


def get():
    
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
    if(out[1]==0.1 or out[1]==-0.1):
        out[1]=0.0
    if(out[0]==0.1 or out[0]==-0.1):
        out[0]=0.0
    if(out[3]==0.1 or out[3]==-0.1):
        out[3]=0.0
    if(out[4]==0.1 or out[4]==-0.1):
        out[4]=0.0
    s=str(out).strip('[]')
    return s
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while True:    
    # Send data
    sent= sock.sendto(bytes(get(), "utf-8"), (server_address))
    #while True:

    try:
        # Receive response
        data, server = sock.recvfrom(4096)
        print(data.decode())
    except:
        #print("Data is passed")
        pass
