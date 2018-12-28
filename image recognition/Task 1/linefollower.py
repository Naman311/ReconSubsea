import cv2
import numpy as np 
from math import sqrt
cap=cv2.VideoCapture(0)
kernelOpen=np.ones((5,5))
kernelClose=np.ones((20,20))

def find_contour(frame,x,y):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower = np.array([-20,100,100])
    upper = np.array([10,255,255])
    mask = cv2.inRange(hsv, lower, upper)
    res = cv2.bitwise_and(frame,frame, mask= mask)
    maskOpen=cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
    maskClose=cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)
    maskFinal=maskClose
    _,conts,h=cv2.findContours(maskFinal.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    if len(conts)!=0:    
        c=max(conts,key=cv2.contourArea)
        cv2.drawContours(frame,c,-1,(0,0,0),3)
        m=cv2.moments(c)
        cx=int(m["m10"]/m["m00"])
        #cy=int(m["m01"]/m["m00"])
        cv2.circle(frame,(cx,y),7,(255,0,0),2)
        dist=(sqrt((x-cx)**2+(y-y)**2))
        return dist
    return None
while True:
    _,frame=cap.read()
    res=cv2.resize(frame,(320,240))
    slices=[res[0:80,0:320],res[80:160,0:320],res[160:240,0:320]]
    cv2.circle(res,(160,40),7,(0,100,100),2)
    cv2.circle(res,(160,120),7,(0,100,100),2)
    cv2.circle(res,(160,200),7,(0,100,100),2)
    d1=find_contour(slices[0],160,40)
    d2=find_contour(slices[1],160,40)
    d3=find_contour(slices[2],160,40)
    #res[0:80,0:320]=[255,255,255]
    cv2.imshow("frame",res)
    print(d1,d2,d3)
    
    
    cv2.waitKey(1)   

