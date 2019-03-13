import numpy as np
import cv2
import sys
import pytesseract

#ip='rtsp://admin:@192.168.1.41/user=admin&password=&channel=1&stream=0.sdp?'
cap = cv2.VideoCapture(1)
config = ('-l eng --oem 1 --psm 3')

 
while(True):
    ret, frame = cap.read()
    print(ret) 
    if ret==True:
    #gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        text = pytesseract.image_to_string(frame, config=config)
        cv2.imshow('frame',frame)
        print(text)
        if cv2.waitKey(1) & 0xFF == ord('v'):
            break

cap.release()
cv2.destroyAllWindows()