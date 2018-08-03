# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'rovconsole.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication,QWidget,QLineEdit
import sys
import cv2

class Ui_widget_3(QWidget):

    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)    
    
    def setupUi(self, widget_3):
        widget_3.setObjectName("widget_3")
        widget_3.resize(878, 578)
        self.label = QtWidgets.QLabel(widget_3)
        self.label.setGeometry(QtCore.QRect(10, 10, 171, 91))
        self.label.setText("")
        self.label.setPixmap(QtGui.QPixmap("logo_final.png"))
        self.label.setScaledContents(True)
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(widget_3)
        self.label_2.setGeometry(QtCore.QRect(710, 10, 161, 81))
        self.label_2.setText("")
        self.label_2.setPixmap(QtGui.QPixmap("logo (1).png"))
        self.label_2.setScaledContents(True)
        self.label_2.setObjectName("label_2")
        self.label_3 = QtWidgets.QLabel(widget_3)
        self.label_3.setGeometry(QtCore.QRect(250, 90, 451, 71))
        font = QtGui.QFont()
        font.setFamily("Magneto")
        font.setBold(True)
        font.setWeight(75)
        self.label_3.setFont(font)
        self.label_3.setObjectName("label_3")
        self.groupBox = QtWidgets.QGroupBox(widget_3)
        self.groupBox.setGeometry(QtCore.QRect(40, 260, 381, 251))
        self.groupBox.setObjectName("groupBox")
        self.label_4 = QtWidgets.QLabel(self.groupBox)
        self.label_4.setGeometry(QtCore.QRect(10, 20, 361, 221))
        self.label_4.setText("")
        self.label_4.setScaledContents(True)
        self.label_4.setObjectName("label_4")
        self.groupBox_2 = QtWidgets.QGroupBox(widget_3)
        self.groupBox_2.setGeometry(QtCore.QRect(480, 260, 371, 251))
        self.groupBox_2.setObjectName("groupBox_2")
        self.label_5 = QtWidgets.QLabel(self.groupBox_2)
        self.label_5.setGeometry(QtCore.QRect(10, 20, 351, 221))
        self.label_5.setText("")
        self.label_5.setScaledContents(True)
        self.label_5.setObjectName("label_5")
        self.label_5.setPixmap(QtGui.QPixmap("frame.jpeg"))
        self.pushButton = QtWidgets.QPushButton(widget_3)
        self.pushButton.setGeometry(QtCore.QRect(370, 540, 171, 23))
        self.pushButton.setObjectName("pushButton")

        self.retranslateUi(widget_3)
        QtCore.QMetaObject.connectSlotsByName(widget_3)

    def retranslateUi(self, widget_3):
        _translate = QtCore.QCoreApplication.translate
        widget_3.setWindowTitle(_translate("widget_3", "ROV Console"))
        self.label_3.setText(_translate("widget_3", "<html><head/><body><p><span style=\" font-size:36pt; color:#ed1c24;\">ROV CONSOLE</span></p></body></html>"))
        self.groupBox.setTitle(_translate("widget_3", "Orientation"))
        self.groupBox_2.setTitle(_translate("widget_3", "Camera Feed"))
        self.pushButton.setText(_translate("widget_3", "Press to Start Operation"))
        cap=cv2.VideoCapture(1)
        while True:
            
            ret,frame=cap.read()
            cv2.imwrite("frame.jpeg",frame)
        self.pushButton.clicked.connect(self.hub)

    def hub(self):
        
            self.orientation()
            self.feed()

    def feed(self):
        print("y")
        
        
        print("done")

    def orientation(self):
        print("x")
        
        
        
if __name__ == '__main__':
    app=QApplication(sys.argv)
    ex=Ui_widget_3()
    ex.show()
    sys.exit(app.exec())
