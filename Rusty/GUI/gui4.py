# -*- coding: utf-8 -*-
"""
Created on Sun Apr  1 16:28:50 2018

@author: Saurav
"""

from PyQt5 import QtCore, QtGui,QtWidgets ,uic
from PyQt5.QtWidgets import QApplication,QWidget,QLineEdit,QApplication,QMainWindow,QPushButton,QMessageBox
from PyQt5.QtCore import QTime,QTimer
import random
import sys
import cv2
import numpy as np
import threading
import time
from multiprocessing import Queue
import serial
from pyimagesearch.shapedetector import ShapeDetector
import imutils
from OpenGL.GL import *
from OpenGL.GLU import *
import pygame
from pygame.locals import *
#ser = serial.Serial('COM1', 9600)
check=0
k=0
l=0
m=0
z1=0
z2=0
z3=0
z4=0
z5=0
z6=0
z7=0
###################################################################################################################################
                                                    #MPU
###################################################################################################################################
#ser = serial.Serial('COM1',9600, timeout=1)

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

    osd_text = " yaw: " + str("{0:.2f}".format(az)) + ", pitch: " + str("{0:.2f}".format(ay)) + ", roll: " + str("{0:.2f}".format(ax))

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
    global z1,z2,z3
    # request data by sending a dot

    #while not line_done:
    #line = ser.readline().strip()
    #b=line.decode('utf-8')
    #angles = b.split(",")
    #if len(angles) == 3:    
        #az = float(angles[0])
        #ay = float(angles[1])
        #ax = float(angles[2])
    az=z1
    ay=z2
    ax=z3
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
        #pygame.image.save(screen,"screen.jpg")
      
        pygame.display.flip()
        frames = frames+1

    print ("fps:  %d" % ((frames*1000)/(pygame.time.get_ticks()-ticks)))
    ser.close()

###################################################################################################################################
###################################################################################################################################

###################################################################################################################################
                                                #Colour Detection
###################################################################################################################################
cap = cv2.VideoCapture(0)
def colour_detect():
    
    kernelOpen=np.ones((5,5))
    kernelClose=np.ones((20,20))
    
    while(True):
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

        area=area1=area2=0
        _,conts,h=cv2.findContours(maskFinal.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
        for i in range(len(conts)):
            cnt=conts[i]
            area=cv2.contourArea(cnt)
        
        _,conts1,h1=cv2.findContours(maskFinal1.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)    
        for i in range(len(conts1)):
            cnt1=conts1[i]
            area1=cv2.contourArea(cnt1)
            
        _,conts2,h2=cv2.findContours(maskFinal2.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
        for i in range(len(conts2)):
            cnt2=conts2[i]
            area2=cv2.contourArea(cnt2)

        sflag=0
        
        if((area>area1)and(area>area2)):
            cv2.drawContours(frame,conts,-1,(230,150,0),3)
            sflag=1
        elif((area1>area)and(area1>area2)):
            cv2.drawContours(frame,conts1,-1,(0,255,255),3)
            sflag=2
        elif((area2>area1)and(area2>area)):
            cv2.drawContours(frame,conts2,-1,(0,0,255),3)
            sflag=3 

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
        if sflag==1:
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
                    if shape:
                        if shape=="triangle":
                            cv2.putText(frame, "C", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                                4, (230,150,0), 3)
                        elif shape=="rectangle":
                            cv2.putText(frame, "F", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                                4, (230,150,0), 3)
                        
        elif sflag==2:
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
                    if shape1:
                        if shape1=="triangle":
                            cv2.putText(frame, "B", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                                4, (0,255,255), 3)
                        elif shape1=="rectangle":
                            cv2.putText(frame, "E", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                                4, (0,255,255), 3)     
        elif sflag==3:
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
                    if shape2:
                        if shape2=="triangle":
                            cv2.putText(frame, "A", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                                4, (0,0,255), 3)
                        elif shape2=="rectangle":
                            cv2.putText(frame, "D", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                                4, (0,0,255), 3)

###################################################################################################################
        frameX=cv2.resize(frame,(600,350))
        cv2.imshow('frame',frameX)
        cv2.moveWindow('frame',750,300)
        #cv2.imshow('mask',mask)
        #cv2.imshow('yellow',res1)
        #cv2.imshow('blue',res)
        #cv2.imshow('red',res2)
    
        if cv2.waitKey(1) & 0xFF==ord('b'):
            global check
            check=check+1
            break
    #cv2.destroyAllWindows()
    
###################################################################################################################################
###################################################################################################################################


#################################################################################################################################
                                            #Character Detection
#################################################################################################################################
import operator
import os
import time

# module level variables ##########################################################################
MIN_CONTOUR_AREA = 100

RESIZED_IMAGE_WIDTH = 20
RESIZED_IMAGE_HEIGHT = 30

cap=cv2.VideoCapture(0)

###################################################################################################
class ContourWithData():

    # member variables ############################################################################
    npaContour = None           # contour
    boundingRect = None         # bounding rect for contour
    intRectX = 0                # bounding rect top left corner x location
    intRectY = 0                # bounding rect top left corner y location
    intRectWidth = 0            # bounding rect width
    intRectHeight = 0           # bounding rect height
    fltArea = 0.0               # area of contour

    def calculateRectTopLeftPointAndWidthAndHeight(self):               # calculate bounding rect info
        [intX, intY, intWidth, intHeight] = self.boundingRect
        self.intRectX = intX
        self.intRectY = intY
        self.intRectWidth = intWidth
        self.intRectHeight = intHeight

    def checkIfContourIsValid(self):                            # this is oversimplified, for a production grade program
        if self.fltArea < MIN_CONTOUR_AREA: return False        # much better validity checking would be necessary
        return True

###################################################################################################
def detect():
    while True:
        ret,frame=cap.read()
        #frame=frame[160:320,160:480]
        if ret==True:
            
            allContoursWithData = []                # declare empty lists,
            validContoursWithData = []              # we will fill these shortly

            try:
                npaClassifications = np.loadtxt("classifications.txt", np.float32)                  # read in training classifications
            except:
                print ("error, unable to open classifications.txt, exiting program\n")
                os.system("pause")
                return
    # end try

            try:
                npaFlattenedImages = np.loadtxt("flattened_images.txt", np.float32)                 # read in training images
            except:
                print ("error, unable to open flattened_images.txt, exiting program\n")
                os.system("pause")
                return
    # end try

            npaClassifications = npaClassifications.reshape((npaClassifications.size, 1))       # reshape numpy array to 1d, necessary to pass to call to train
    
            kNearest = cv2.ml.KNearest_create()                   # instantiate KNN object

            kNearest.train(npaFlattenedImages, cv2.ml.ROW_SAMPLE, npaClassifications)

        #imgTestingNumbers = cv2.imread("frame.jpg")          # read in testing numbers image

            if frame is None:                           # if image was not read successfully
                print ("error: image not read from file \n\n")        # print error message to std out
                os.system("pause")                                  # pause so user can see error message
                return                                              # and exit function (which exits program)
    # end if

            imgGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)       # get grayscale image
            imgBlurred = cv2.GaussianBlur(imgGray, (5,5), 0)                    # blur

                                                        # filter image from grayscale to black and white
            imgThresh = cv2.adaptiveThreshold(imgBlurred,                           # input image
                                      255,                                  # make pixels that pass the threshold full white
                                      cv2.ADAPTIVE_THRESH_GAUSSIAN_C,       # use gaussian rather than mean, seems to give better results
                                      cv2.THRESH_BINARY_INV,                # invert so foreground will be white, background will be black
                                      11,                                   # size of a pixel neighborhood used to calculate threshold value
                                      2)                                    # constant subtracted from the mean or weighted mean

            imgThreshCopy = imgThresh.copy()        # make a copy of the thresh image, this in necessary b/c findContours modifies the image

            imgContours, npaContours, npaHierarchy = cv2.findContours(imgThreshCopy,             # input image, make sure to use a copy since the function will modify this image in the course of finding contours
                                                 cv2.RETR_EXTERNAL,         # retrieve the outermost contours only
                                                 cv2.CHAIN_APPROX_SIMPLE)   # compress horizontal, vertical, and diagonal segments and leave only their end points

            for npaContour in npaContours:                             # for each contour
                contourWithData = ContourWithData()                                             # instantiate a contour with data object
                contourWithData.npaContour = npaContour                                         # assign contour to contour with data
                contourWithData.boundingRect = cv2.boundingRect(contourWithData.npaContour)     # get the bounding rect
                contourWithData.calculateRectTopLeftPointAndWidthAndHeight()                    # get bounding rect info
                contourWithData.fltArea = cv2.contourArea(contourWithData.npaContour)           # calculate the contour area
                allContoursWithData.append(contourWithData)                                     # add contour with data object to list of all contours with data
    # end for

            for contourWithData in allContoursWithData:                 # for all contours
                if contourWithData.checkIfContourIsValid():             # check if valid
                    validContoursWithData.append(contourWithData)       # if so, append to valid contour list
        # end if
    # end for

            validContoursWithData.sort(key = operator.attrgetter("intRectX"))         # sort contours from left to right

            strFinalString = ""         # declare final string, this will have the final number sequence by the end of the program

            for contourWithData in validContoursWithData:            # for each contour
                                                # draw a green rect around the current char
                cv2.rectangle(frame,                                        # draw rectangle on original testing image
                      (contourWithData.intRectX, contourWithData.intRectY),     # upper left corner
                      (contourWithData.intRectX + contourWithData.intRectWidth, contourWithData.intRectY + contourWithData.intRectHeight),      # lower right corner
                      (0, 255, 0),              # green
                      2)                        # thickness

                imgROI = imgThresh[contourWithData.intRectY : contourWithData.intRectY + contourWithData.intRectHeight,     # crop char out of threshold image
                           contourWithData.intRectX : contourWithData.intRectX + contourWithData.intRectWidth]

                imgROIResized = cv2.resize(imgROI, (RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT))             # resize image, this will be more consistent for recognition and storage
    
                npaROIResized = imgROIResized.reshape((1, RESIZED_IMAGE_WIDTH * RESIZED_IMAGE_HEIGHT))      # flatten image into 1d numpy array

                npaROIResized = np.float32(npaROIResized)       # convert from 1d numpy array of ints to 1d numpy array of floats

                retval, npaResults, neigh_resp, dists = kNearest.findNearest(npaROIResized, k = 1)     # call KNN function find_nearest
                #print(dists)
                strCurrentChar = str(chr(int(npaResults[0][0])))                                             # get character from results

                strFinalString = strFinalString + strCurrentChar            # append current char to full string
    # end for
                struh1="UH8"
                struh2="L6R"
                struh3="G7C"
                struh4="S1P"
                struh5="JW3"
                struh6="A2X"

                if strFinalString.find(struh1)>0:
                    print(struh1)
                    cv2.putText(frame, "A", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                            2, (0,0,255), 2)
                elif strFinalString.find(struh2)>0:
                    print(struh2)
                    cv2.putText(frame, "B", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                            2, (0,255,255), 2)
                elif strFinalString.find(struh3)>0:
                    print(struh3)
                    cv2.putText(frame, "C", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                            2, (230,150,0), 2)
                elif strFinalString.find(struh4)>0:
                    print(struh4)
                    cv2.putText(frame, "D", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                            2, (0,0,255), 2)
                elif strFinalString.find(struh5)>0:
                    print(struh5)
                    cv2.putText(frame, "E", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                            2, (0,255,255), 2)
                elif strFinalString.find(struh6)>0:
                    print(struh6)
                    cv2.putText(frame, "F", (200, 200), cv2.FONT_HERSHEY_SIMPLEX,
                            2, (230,150,0), 2)
                
                #print ("\n" + strFinalString + "\n")                  # show the full string

                cv2.imshow("imgTestingNumbers", frame)      # show input image with green boxes drawn around found digits
            if cv2.waitKey(1) & 0xFF==ord('v'):                                          # wait for user key press
                break
            # remove windows from memory
    #cv2.destroyAllWindows()       
#################################################################################################################################
#################################################################################################################################

running = False
running1 = False

capture_thread = None
form_class = uic.loadUiType("simple111.ui")[0]
q = Queue()
i=0
j=0


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
            print("x")
            print (queue.qsize())

def printx():
    global running1
    global i
    global j
    global k
    global l
    global m
    global z1,z2,z3,z4,z5,z6,z7
    
    while(running1):
        #data = ser.readline()
        #data = data.decode('utf-8')
        #Data=data.split(',')
        #x=float(Data[0])
        i=random.uniform(0,100)
        #y=float(Data[1])
        j=random.uniform(0,100)
        #z=float(Data[2])
        k=random.uniform(0,100)
        #a=float(Data[3])
        l=random.uniform(0,100)
        #b=float(Data[4])
        m=random.uniform(0,100)

        z1=random.uniform(0,100)
        z2=random.uniform(0,100)
        z3=random.uniform(0,100)
        z4=random.uniform(0,100)
        z5=random.uniform(0,100)
        z6=random.uniform(0,100)
        z7=random.uniform(0,100)
        
        #print("k")
        #print(k)
        #print("l")
        #print(l)
        #print("m")
        #print(m)
        time.sleep(1)

'''

def printx():
    global running1
    global i
    global j
    global k
    global l
    global m
    global z1,z2,z3,z4,z5,z6,z7
    
    while(running1):
        data = ser.readline()
        data = data.decode('utf-8')
        Data=data.split(',')
        i=float(Data[0])
        #i=random.uniform(0,100)
        j=float(Data[1])
        #j=random.uniform(0,100)
        k=float(Data[2])
        #k=random.uniform(0,100)
        m=float(Data[3])
        #l=random.uniform(0,100)
        m=float(Data[4])
        #m=random.uniform(0,100)

        #z1=random.uniform(0,100)
        #z2=random.uniform(0,100)
        #z3=random.uniform(0,100)
        #z4=random.uniform(0,100)
        #z5=random.uniform(0,100)
        #z6=random.uniform(0,100)
        #z7=random.uniform(0,100)        
        z1=float(Data[5])
        z2=float(Data[6])
        z3=float(Data[7])
        z4=float(Data[8])
        z5=float(Data[9])
        z6=float(Data[10])
        z7=float(Data[11])

'''
'''
import time
#import pygame
import socket
import sys

server_address = ('169.254.234.100', 5000)
# Initializing controller
pygame.init()
jj = pygame.joystick.Joystick(0)
jj.init()
print ('Initialized Joystick : %s' % jj.get_name())


def get():
    out = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    it = 0 #iterator
    pygame.event.pump()
    
    #Read input from the two joysticks       
    for i in range(0, jj.get_numaxes()):
        out[it] = round(jj.get_axis(i),1)
        it+=1
    #Read input from buttons
    for i in range(0, jj.get_numbuttons()):
        out[it] = jj.get_button(i)
        it+=1
    if(abs(out[1])>abs(out[0])):
        out[0]=0.0
    else:
        out[1]=0.0
    if(out[1]==0.1 or out[1]==-0.1):
        out[1]=0.0
    if(out[0]==0.1 or out[0]==-0.1):
        out[0]=0.0
    if(out[3]==0.1 or out[3]==-0.1):
        out[3]=0.0
    if(out[4]==0.1 or out[4]==-0.1):
        out[4]=0.0
    out[1]=out[1]*10
    s=str(out).strip('[]')
    #print(s)
    return s
#print("bhen")
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print ("Socket successfully created")
except socket.error as err:
    print ("socket creation failed with error ")
#sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#print("ke")


    
def printx():
    global running1
    global i
    global j
    global k
    global l
    global m
    global z1,z2,z3,z4,z5,z6,z7
    while True:    
    # Send data
    #get()
    #print("x")
        sent= sock.sendto(bytes(get(), "utf-8"), (server_address))
        #while True:
        try:
            # Receive response
        #print("try")
            scam, server = sock.recvfrom(4096)
        #print('y')
            #print(data.decode())
            scam1=scam.decode()
            scamx=scam1.split(',')
            i=float(scamx[0])
            j=float(scamx[1])
            k=float(scamx[2])
            l=float(scamx[3])
            m=float(scamx[4])
            z1=float(scamx[5])
            z2=float(scamx[6])
            z3=float(scamx[7])
            z4=float(scamx[8])
            z5=float(scamx[9])
            z6=float(scamx[10])
            z7=float(scamx[11])
            
        except:
        
            #print("Data is passed")
            pass
        
        #time.sleep(0.1)
    
    
##############################################################################
'''
        
    

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



class MyWindowClass(QMainWindow, form_class):
    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        self.setupUi(self)

        self.startButton.clicked.connect(self.start_clicked)
        self.pushButton.clicked.connect(self.printer)
        self.pushButton_2.clicked.connect(self.message)
        self.pushButton_3.clicked.connect(self.message1)
        self.pushButton_4.clicked.connect(self.ct)
        self.window_width = self.ImgWidget.frameSize().width()
        self.window_height = self.ImgWidget.frameSize().height()
        self.ImgWidget = OwnImageWidget(self.ImgWidget)       
        
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.update_frame)
        self.timer.timeout.connect(self.update_lcd)
        self.timer.start(1)

    def cvfeed(self):
    
        detect_thread.start()
        
        self.pushButton_2.setEnabled(False)    

    def message(self):
        choice=QMessageBox.question(self,'Start processing?',
                                    "Start Detection?",
                                    QMessageBox.Yes|QMessageBox.No)
        if choice==QMessageBox.Yes:
            self.cvfeed()
        else:
            pass

    def message1(self):
        choice=QMessageBox.question(self,'Start processing?',
                                    "Start Detection?",
                                    QMessageBox.Yes|QMessageBox.No)
        if choice==QMessageBox.Yes:
            self.cvfeed1()
        else:
            pass

    def ct(self):
        mpu_thread.start()

    

    def cvfeed1(self):
        global check
        if check==0:
            detect_1_thread.start()
        else:
            scam_thread.start()
        self.pushButton_3.setEnabled(False)
        
        
        
    
    def printer(self):
        global running1
        running1=True
        printer_thread.start()
        self.pushButton.setEnabled(False)

    def start_clicked(self):
        global running
        running = True
        capture_thread.start()
        self.startButton.setEnabled(False)
        self.startButton.setText('Starting...')

    def update_lcd(self):
        global i
        global j
        global k
        global l
        global m
        global z1,z2,z3,z4,z5,z6
        self.lcdNumber.display(i)
        self.lcdNumber_2.display(j)
        self.lcdNumber_3.display(k)
        self.lcdNumber_4.display(l)
        self.lcdNumber_5.display(m)
        self.lcdNumber_6.display(z1)
        self.lcdNumber_7.display(z2)
        self.lcdNumber_8.display(z3)
        self.lcdNumber_9.display(z4)
        self.lcdNumber_10.display(z5)
        self.lcdNumber_11.display(z6)
        self.lcdNumber_12.display(z7)
        stime="3:23"
        self.lcdNumber_14.display(stime)
        self.lcdNumber_13.display(stime)
        stime="0:00"
        self.lcdNumber_15.display(stime)
        self.lcdNumber_16.display(stime)
    
        
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

    def closeEvent(self, event):
        global running
        running = False



capture_thread = threading.Thread(target=grab, args = (0, q, 1920, 1080, 30))
printer_thread = threading.Thread(target=printx)
detect_thread=threading.Thread(target=detect)
scam_thread=threading.Thread(target=colour_detect)
detect_1_thread=threading.Thread(target=colour_detect)
mpu_thread=threading.Thread(target=main)

print("LODE")
app = QApplication(sys.argv)
print("LODE2")
w = MyWindowClass(None)
print("LODE3")
w.setWindowTitle('GUI')
print("LODE4")
w.show()
print("LwODE5")
app.exec_()
print("LODE6")
 
