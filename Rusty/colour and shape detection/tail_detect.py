import cv2
import numpy as np
from shape_detect.shapedetector import ShapeDetector
import imutils

cap = cv2.VideoCapture(1)
kernelOpen=np.ones((5,5))
kernelClose=np.ones((20,20))

while(1):
    _, frame = cap.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower2 = np.array([0,125,125])
    upper2 = np.array([8,255,255])


    lower1 = np.array([17,125,125])
    upper1 = np.array([32,255,255])

    lower = np.array([110,125,125])
    upper = np.array([130,255,255])

    mask1 =cv2.inRange(hsv, lower1, upper1)
    mask = cv2.inRange(hsv, lower, upper)
    mask2 = cv2.inRange(hsv, lower2, upper2)

    res = cv2.bitwise_and(frame,frame, mask= mask)
    res1 = cv2.bitwise_and(frame,frame, mask= mask1)
    res2 = cv2.bitwise_and(frame,frame, mask= mask2)

    maskOpen=cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
    maskOpen1=cv2.morphologyEx(mask1,cv2.MORPH_OPEN,kernelOpen)
    maskOpen2=cv2.morphologyEx(mask2,cv2.MORPH_OPEN,kernelOpen)
    
    maskClose=cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)
    maskClose1=cv2.morphologyEx(maskOpen1,cv2.MORPH_CLOSE,kernelClose)
    maskClose2=cv2.morphologyEx(maskOpen2,cv2.MORPH_CLOSE,kernelClose)

    maskFinal=maskClose
    maskFinal1=maskClose1
    maskFinal2=maskClose2
    
    _,conts,h=cv2.findContours(maskFinal.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    cv2.drawContours(frame,conts,-1,(230,150,0),3)
    _,conts1,h1=cv2.findContours(maskFinal1.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    cv2.drawContours(frame,conts1,-1,(0,255,255),3)
    _,conts2,h2=cv2.findContours(maskFinal2.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    cv2.drawContours(frame,conts2,-1,(0,0,255),3)    

###################################################################################################################

    resized = imutils.resize(res, width=300)
    ratio = res.shape[0] / float(resized.shape[0])

    resized1 = imutils.resize(res1, width=300)
    ratio1 = res1.shape[0] / float(resized1.shape[0])

    resized2 = imutils.resize(res2, width=300)
    ratio2 = res1.shape[0] / float(resized2.shape[0])

# convert the resized image to grayscale, blur it slightly,
# and threshold it
    gray = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]

    gray1 = cv2.cvtColor(resized1, cv2.COLOR_BGR2GRAY)
    blurred1 = cv2.GaussianBlur(gray1, (5, 5), 0)
    thresh1 = cv2.threshold(blurred1, 60, 255, cv2.THRESH_BINARY)[1]

    gray2 = cv2.cvtColor(resized2, cv2.COLOR_BGR2GRAY)
    blurred2 = cv2.GaussianBlur(gray2, (5, 5), 0)
    thresh2 = cv2.threshold(blurred2, 60, 255, cv2.THRESH_BINARY)[1]
# find contours in the thresholded image and initialize the
# shape detector
    cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)
    cnts = cnts[0] if imutils.is_cv2() else cnts[1]

    cnts1 = cv2.findContours(thresh1.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)
    cnts1 = cnts1[0] if imutils.is_cv2() else cnts1[1]

    cnts2 = cv2.findContours(thresh2.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)
    cnts2 = cnts1[0] if imutils.is_cv2() else cnts2[1]

    
    sd = ShapeDetector()
    sd1 = ShapeDetector()
    sd2 = ShapeDetector()

# loop over the contours
    for c in cnts:
	# compute the center of the contour, then detect the name of the
	# shape using only the contour
            '''
            M = cv2.moments(c)
            cX = int((M["m10"] / M["m00"]) * ratio)
            cY = int((M["m01"] / M["m00"]) * ratio)
            '''
            shape = sd.detect(c)
            shapex="blue "+shape
            '''
	# multiply the contour (x, y)-coordinates by the resize ratio,
	# then draw the contours and the name of the shape on the image
            c = c.astype("float")
            c *= ratio
            c = c.astype("int")
            #cv2.drawContours(frame, [c], -1, (0, 255, 0), 2)
            '''
            cv2.putText(frame, shapex, (100, 100), cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2)
    for c1 in cnts1:
	# compute the center of the contour, then detect the name of the
	# shape using only the contour
            '''
            M = cv2.moments(c)
            cX = int((M["m10"] / M["m00"]) * ratio)
            cY = int((M["m01"] / M["m00"]) * ratio)
            '''
            shape1 = sd1.detect(c1)
            shapex1="yellow "+shape1
            '''
	# multiply the contour (x, y)-coordinates by the resize ratio,
	# then draw the contours and the name of the shape on the image
            c = c.astype("float")
            c *= ratio
            c = c.astype("int")
            #cv2.drawContours(frame, [c], -1, (0, 255, 0), 2)
            '''
            cv2.putText(frame, shapex1, (100, 200), cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2)        

    for c2 in cnts2:
	# compute the center of the contour, then detect the name of the
	# shape using only the contour
            '''
            M = cv2.moments(c)
            cX = int((M["m10"] / M["m00"]) * ratio)
            cY = int((M["m01"] / M["m00"]) * ratio)
            '''
            shape2 = sd2.detect(c2)
            shapex2="red "+shape2
            '''
	# multiply the contour (x, y)-coordinates by the resize ratio,
	# then draw the contours and the name of the shape on the image
            c = c.astype("float")
            c *= ratio
            c = c.astype("int")
            #cv2.drawContours(frame, [c], -1, (0, 255, 0), 2)
            '''
            cv2.putText(frame, shapex2, (100, 300), cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2)
###################################################################################################################

    cv2.imshow('frame',frame)
    #cv2.imshow('mask',mask)
    cv2.imshow('yellow',res1)
    cv2.imshow('blue',res)
    cv2.imshow('red',res2)
    
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()
