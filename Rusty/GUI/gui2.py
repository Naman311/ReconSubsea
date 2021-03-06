# -*- coding: utf-8 -*-
"""
Created on Sun Apr  1 16:28:50 2018

@author: Saurav
"""

from PyQt5 import QtCore, QtGui,QtWidgets ,uic
from PyQt5.QtWidgets import QApplication,QWidget,QLineEdit,QApplication,QMainWindow

import sys
import cv2
import numpy as np
import threading
import time
from multiprocessing import Queue
from pyimagesearch.shapedetector import ShapeDetector
import imutils
from OpenGL.GL import *
from OpenGL.GLU import *
import serial
import pygame
from pygame.locals import *
###################################################################################################################################
                                                    #MPU
###################################################################################################################################
ser = serial.Serial('COM1',9600, timeout=1)

ax = ay = az = 0.0
#yaw_mode = False

def resize(width, height):
    if height==0:
        height=1
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, 1.0*width/height, 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

def init():
    glShadeModel(GL_SMOOTH)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

def drawText(position, textString):     
    font = pygame.font.SysFont ("Courier", 18, True)
    textSurface = font.render(textString, True, (255,255,255,255), (0,0,0,255))     
    textData = pygame.image.tostring(textSurface, "RGBA", True)     
    glRasterPos3d(*position)     
    glDrawPixels(textSurface.get_width(), textSurface.get_height(), GL_RGBA, GL_UNSIGNED_BYTE, textData)

def draw():
    global rquad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    
    glLoadIdentity()
    glTranslatef(0,0.0,-7.0)

    osd_text = "pitch: " + str("{0:.2f}".format(ay)) + ", roll: " + str("{0:.2f}".format(ax)) + ", yaw: " + str("{0:.2f}".format(az))

    #if yaw_mode:
    #osd_line = osd_text + ", yaw: " + str("{0:.2f}".format(az))
    #else:
    osd_line = osd_text

    drawText((-3,-2, 2), osd_line)

    # the way I'm holding the IMU board, X and Y axis are switched 
    # with respect to the OpenGL coordinate system
    #if yaw_mode:                             # experimental
    glRotatef(az, 0.0, 1.0, 0.0)  # Yaw,   rotate around y-axis
    #else:
    #glRotatef(0.0, 0.0, 1.0, 0.0)
    glRotatef(ay ,1.0,0.0,0.0)        # Pitch, rotate around x-axis
    glRotatef(-1*ax ,0.0,0.0,1.0)     # Roll,  rotate around z-axis
    #glRotatef(ax ,0.0,0.0,1.0) 

    glBegin(GL_QUADS)	
    glColor3f(0.0,1.0,0.0)
    glVertex3f( 1.0, 0.2,-1.0)
    glVertex3f(-1.0, 0.2,-1.0)		
    glVertex3f(-1.0, 0.2, 1.0)		
    glVertex3f( 1.0, 0.2, 1.0)		

    glColor3f(1.0,0.5,0.0)	
    glVertex3f( 1.0,-0.2, 1.0)
    glVertex3f(-1.0,-0.2, 1.0)		
    glVertex3f(-1.0,-0.2,-1.0)		
    glVertex3f( 1.0,-0.2,-1.0)		

    glColor3f(1.0,0.0,0.0)		
    glVertex3f( 1.0, 0.2, 1.0)
    glVertex3f(-1.0, 0.2, 1.0)		
    glVertex3f(-1.0,-0.2, 1.0)		
    glVertex3f( 1.0,-0.2, 1.0)		

    glColor3f(1.0,1.0,0.0)	
    glVertex3f( 1.0,-0.2,-1.0)
    glVertex3f(-1.0,-0.2,-1.0)
    glVertex3f(-1.0, 0.2,-1.0)		
    glVertex3f( 1.0, 0.2,-1.0)		

    glColor3f(0.0,0.0,1.0)	
    glVertex3f(-1.0, 0.2, 1.0)
    glVertex3f(-1.0, 0.2,-1.0)		
    glVertex3f(-1.0,-0.2,-1.0)		
    glVertex3f(-1.0,-0.2, 1.0)		

    glColor3f(1.0,0.0,1.0)	
    glVertex3f( 1.0, 0.2,-1.0)
    glVertex3f( 1.0, 0.2, 1.0)
    glVertex3f( 1.0,-0.2, 1.0)		
    glVertex3f( 1.0,-0.2,-1.0)		
    glEnd()	
         
def read_data():
    global ax, ay, az
    ax = ay = az = 0.0
    line_done = 0

    # request data by sending a dot

    #while not line_done:
    line = ser.readline().strip()
    b=line.decode('utf-8')
    angles = b.split(",")
    if len(angles) == 3:    
        az = float(angles[0])
        ay = float(angles[1])
        ax = float(angles[2])
        line_done = 1 

def main():
    #global yaw_mode

    video_flags = OPENGL|DOUBLEBUF
    
    pygame.init()
    screen = pygame.display.set_mode((480,320), video_flags)
    pygame.display.set_caption("Press Esc to quit, z toggles yaw mode")
    resize(480,320)
    init()
    frames = 0
    ticks = pygame.time.get_ticks()
    while 1:
        event = pygame.event.poll()
        """if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break       
        if event.type == KEYDOWN and event.key == K_z:
            yaw_mode = not yaw_mode
            ser.write("z")"""
        read_data()
        draw()
        pygame.image.save(screen,"screen.jpg")
      
        pygame.display.flip()
        frames = frames+1

    print ("fps:  %d" % ((frames*1000)/(pygame.time.get_ticks()-ticks)))
    ser.close()

###################################################################################################################################
###################################################################################################################################



###################################################################################################################################
###################################################################################################################################
cap=cv2.VideoCapture(0)
def colour_detect():
    #cap = cv2.VideoCapture(0)
    kernelOpen=np.ones((5,5))
    kernelClose=np.ones((20,20))
    
    while(True):
        ret, frame = cap.read()
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
        frameX=cv2.resize(frame,(600,400))
        cv2.imshow('frame',frameX)
        #cv2.imshow('mask',mask)
        #cv2.imshow('yellow',res1)
        #cv2.imshow('blue',res)
        #cv2.imshow('red',res2)
    
        if cv2.waitKey(1) & 0xFF==ord('b'):
            break
    #cv2.destroyAllWindows()
    
###################################################################################################################################
###################################################################################################################################





running = False
running1 = False
running2 = False

capture_thread = None
form_class = uic.loadUiType("simple11.ui")[0]
q = Queue()
q1=Queue()

def grab(cam, queue, width, height, fps):
    global running
    capture = cv2.VideoCapture(cam)
    capture.set(cv2.CAP_PROP_FRAME_WIDTH, width)
    capture.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
    capture.set(cv2.CAP_PROP_FPS, fps)

    while(running):
        frame = {}        
        capture.grab()
        retval, img = capture.retrieve(0)
        frame["img"] = img

        if queue.qsize() < 10:
            queue.put(frame)
        else:
            print (queue.qsize())

def grab1(queue, width, height, fps):
    global running2
    capture = cv2.imread('screen.jpg')
    capture.set(cv2.CAP_PROP_FRAME_WIDTH, width)
    capture.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
    #capture.set(cv2.CAP_PROP_FPS, fps)

    while(running2):
        frame = {}        
        capture.grab()
        retval, img = capture.retrieve(0)
        frame["img"] = img

        if queue.qsize() < 10:
            queue.put(frame)
        else:
            print (queue.qsize())

def printx():
    global running1
    while(running1):
        print("x")
    

class OwnImageWidget(QWidget):
    def __init__(self, parent=None):
        super(OwnImageWidget, self).__init__(parent)
        self.image = None

    def setImage(self, image):
        self.image = image
        sz = image.size()
        self.setMinimumSize(sz)
        self.update()

    def paintEvent(self, event):
        qp = QtGui.QPainter()
        qp.begin(self)
        if self.image:
            qp.drawImage(QtCore.QPoint(0, 0), self.image)
        qp.end()

class OwnImageWidget1(QWidget):
    def __init__(self, parent=None):
        super(OwnImageWidget1, self).__init__(parent)
        self.image = None

    def setImage(self, image):
        self.image = image
        sz = image.size()
        self.setMinimumSize(sz)
        self.update()

    def paintEvent(self, event):
        qp = QtGui.QPainter()
        qp.begin(self)
        if self.image:
            qp.drawImage(QtCore.QPoint(0, 0), self.image)
        qp.end()



class MyWindowClass(QMainWindow, form_class):
    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        self.setupUi(self)

        self.startButton.clicked.connect(self.start_clicked)
        self.pushButton_5.clicked.connect(self.start_clicked1)
        self.pushButton.clicked.connect(self.printer)
        self.pushButton_2.clicked.connect(self.mpu_feed)
        self.pushButton_3.clicked.connect(self.cvfeed1)
        self.pushButton_4.clicked.connect(self.ct)
        self.window_width = self.ImgWidget.frameSize().width()
        self.window_height = self.ImgWidget.frameSize().height()
        self.ImgWidget = OwnImageWidget(self.ImgWidget)
        self.ImgWidget_2 = OwnImageWidget1(self.ImgWidget_2)

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.update_frame)
        self.timer.timeout.connect(self.update_frame1)
        self.timer.start(1)

    def ct(self):
        mpu_thread.start()


    def cvfeed1(self):
        
        detect_1_thread.start()
        #self.pushButton_3.setEnabled(False)
         

    def printer(self):
        global running1
        running1=True
        printer_thread.start()

    def start_clicked(self):
        global running
        running = True
        capture_thread.start()
        self.startButton.setEnabled(False)
        self.startButton.setText('Starting...')

    def start_clicked1(self):
        global running2
        running2 = True
        mpu_display.start()
        self.pushButton_5.setEnabled(False)
        self.pushButton_5.setText('Starting...')

    def mpu_feed(self):
        print('aaa')
        self.label_4.setPixmap(QtGui.QPixmap("logo_final.png"))


    def update_frame(self):
        if not q.empty():
            self.startButton.setText('Camera is live')
            frame = q.get()
            img = frame["img"]

            img_height, img_width, img_colors = img.shape
            scale_w = float(self.window_width) / float(img_width)
            scale_h = float(self.window_height) / float(img_height)
            scale = min([scale_w, scale_h])

            if scale == 0:
                scale = 1
            
            img = cv2.resize(img, None, fx=scale, fy=scale, interpolation = cv2.INTER_CUBIC)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
            height, width, bpc = img.shape
            bpl = bpc * width
            image = QtGui.QImage(img.data, width, height, bpl, QtGui.QImage.Format_RGB888)
            self.ImgWidget.setImage(image)

    def update_frame1(self):
        if not q.empty():
            #self.startButton.setText('Camera is live')
            frame = q1.get()
            img = frame["img"]

            img_height, img_width, img_colors = img.shape
            scale_w = float(self.window_width) / float(img_width)
            scale_h = float(self.window_height) / float(img_height)
            scale = min([scale_w, scale_h])

            if scale == 0:
                scale = 1
            
            img = cv2.resize(img, None, fx=scale, fy=scale, interpolation = cv2.INTER_CUBIC)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
            height, width, bpc = img.shape
            bpl = bpc * width
            image = QtGui.QImage(img.data, width, height, bpl, QtGui.QImage.Format_RGB888)
            self.ImgWidget.setImage(image)

    def closeEvent(self, event):
        global running
        running = False



capture_thread = threading.Thread(target=grab, args = (0, q, 1920, 1080, 30))
mpu_display=threading.Thread(target=grab1,args=(q1,480,320,30))
printer_thread = threading.Thread(target=printx)
detect_1_thread=threading.Thread(target=colour_detect)
mpu_thread=threading.Thread(target=main)

app = QApplication(sys.argv)
w = MyWindowClass(None)
w.setWindowTitle('GUI')
w.show()
app.exec_()
