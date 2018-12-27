import cv2
import numpy as np
from shape_detect.shapedetector import ShapeDetector
import imutils

#frame=cv2.imread("shapes2.jpg")
cap = cv2.VideoCapture(0)
kernelOpen=np.ones((5,5))
kernelClose=np.ones((20,20))

while(1):
    _, frame = cap.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower = np.array([0,0,0])
    upper = np.array([180,255,30])
    mask = cv2.inRange(hsv, lower, upper)
    res = cv2.bitwise_and(frame,frame, mask= mask)
    maskOpen=cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
    maskClose=cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)
    maskFinal=maskClose
    _,conts,h=cv2.findContours(maskFinal.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    cv2.drawContours(frame,conts,-1,(230,0,0),3)



    #resized = imutils.resize(res, width=300)
    #ratio = frame.shape[0] / float(resized.shape[0])
    #gray = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
    #blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    #thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]
    #cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
	#cv2.CHAIN_APPROX_SIMPLE)
    #cnts = cnts[0] if imutils.is_cv2() else cnts[1]
    sd = ShapeDetector()
    shapesf=dict()
    #sd1 = ShapeDetector()
    #sd2 = ShapeDetector()

    for c in conts:
        shape = sd.detect(c)
        if shape in shapesf:
                shapesf[shape]+=1
        else:
            shapesf[shape]=1

    cv2.imshow('frame',frame)
    print(shapesf)
    #cv2.imshow('mask',gray)
    #cv2.imshow('yellow',res1)
    cv2.imshow('blue',res)
    #cv2.imshow('red',res2)
    
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()
