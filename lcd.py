import threading
import time
num=0
i=0
c=0
mi=0
se=0
cb=0
cb1=0
mi1=0
se1=0
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'lcd.ui'
#
# Created by: PyQt5 UI code generator 5.11.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.lcdNumber = QtWidgets.QLCDNumber(self.centralwidget)
        self.lcdNumber.setGeometry(QtCore.QRect(520, 60, 64, 23))
        self.lcdNumber.setSmallDecimalPoint(True)
        self.lcdNumber.setObjectName("lcdNumber")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(230, 70, 75, 23))
        self.pushButton.setObjectName("pushButton")
        self.checkBox = QtWidgets.QCheckBox(self.centralwidget)
        self.checkBox.setGeometry(QtCore.QRect(530, 100, 70, 17))
        self.checkBox.setObjectName("checkBox")
        self.checkBox_2 = QtWidgets.QCheckBox(self.centralwidget)
        self.checkBox_2.setGeometry(QtCore.QRect(530, 130, 70, 17))
        self.checkBox_2.setObjectName("checkBox_2")
        self.checkBox_3 = QtWidgets.QCheckBox(self.centralwidget)
        self.checkBox_3.setGeometry(QtCore.QRect(530, 160, 70, 17))
        self.checkBox_3.setObjectName("checkBox_3")
        self.lcdNumber_3 = QtWidgets.QLCDNumber(self.centralwidget)
        self.lcdNumber_3.setGeometry(QtCore.QRect(620, 230, 64, 23))
        self.lcdNumber_3.setObjectName("lcdNumber_3")
        self.checkBox_4 = QtWidgets.QCheckBox(self.centralwidget)
        self.checkBox_4.setGeometry(QtCore.QRect(530, 220, 70, 17))
        self.checkBox_4.setObjectName("checkBox_4")
        self.checkBox_5 = QtWidgets.QCheckBox(self.centralwidget)
        self.checkBox_5.setGeometry(QtCore.QRect(530, 250, 70, 17))
        self.checkBox_5.setObjectName("checkBox_5")
        self.checkBox_6 = QtWidgets.QCheckBox(self.centralwidget)
        self.checkBox_6.setGeometry(QtCore.QRect(530, 280, 70, 17))
        self.checkBox_6.setObjectName("checkBox_6")
        self.lcdNumber_2 = QtWidgets.QLCDNumber(self.centralwidget)
        self.lcdNumber_2.setGeometry(QtCore.QRect(620, 130, 64, 23))
        self.lcdNumber_2.setObjectName("lcdNumber_2")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 21))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.pushButton.clicked.connect(self.m)
        self.checkBox.clicked.connect(self.task)
        self.checkBox_2.clicked.connect(self.task)
        self.checkBox_3.clicked.connect(self.task)
        self.checkBox_4.clicked.connect(self.task_1)
        self.checkBox_5.clicked.connect(self.task_1)
        self.checkBox_6.clicked.connect(self.task_1)
        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.pushButton.setText(_translate("MainWindow", "PushButton"))
        self.checkBox.setText(_translate("MainWindow", "task1"))
        self.checkBox_2.setText(_translate("MainWindow", "task2"))
        self.checkBox_3.setText(_translate("MainWindow", "task3"))
        self.checkBox_4.setText(_translate("MainWindow", "task4"))
        self.checkBox_5.setText(_translate("MainWindow", "task5"))
        self.checkBox_6.setText(_translate("MainWindow", "task6"))
    def m(self):
        t = threading.Thread(target = self.y)
        t.start()
        global c
        c=c+1

    def y(self):
        global i
        global num
        while i<1000000000000000000000:
            if i<10:
                self.lcdNumber.display(str(num)+":0"+str(i))
            else:
                self.lcdNumber.display(str(num)+":"+str(i))
            
            if i==60:
                num = num+1
                i=0
                self.lcdNumber.display(str(num)+":"+str(i))
            i=i+1
            time.sleep(1)
            if c==2 and self.checkBox.isChecked() and self.checkBox_2.isChecked()and self.checkBox_3.isChecked() and self.checkBox_4.isChecked() and self.checkBox_5.isChecked()and self.checkBox_6.isChecked():
                i=1000000000000000000000
                self.pushButton.setEnabled(False)
            if c==2 and (self.checkBox.isChecked()==False or self.checkBox_2.isChecked()==False or self.checkBox_3.isChecked()==False or self.checkBox_4.isChecked()==False or self.checkBox_5.isChecked()==False or self.checkBox_6.isChecked()==False):
                i=1000000000000000000000
                self.pushButton.setEnabled(False)
            
    def task(self):
        global num
        global i
        global mi
        global se
        global cb
        global mi1
        global se1
        cb=cb+1
        print(cb)
        if cb==3:
            self.lcdNumber_2.display(str(num)+":"+str(i))
            mi1 = num
            se1 = i
        if cb==3 and cb1==3:
            mi1 = num-mi
            se1 = i-se
            self.lcdNumber_2.display(str(mi1)+":"+str(se1))
        '''if cb==6:
            self.lcdNumber_3.display(str(num-mi)+":"+str(i-se))'''
        #if cb/3==1:
        #loop()   
        #if self.checkBox_4.isChecked() and self.checkBox_5.isChecked() and self.checkBox_6.isChecked():
           #mi=num
           #se = i
        #if cb/3==2:
         #   mi=num-mi
          #  se=i-se
        #if self.checkBox.isChecked() and self.checkBox_2.isChecked()and self.checkBox_3.isChecked():
            
           # if i<10:
                
            #    self.lcdNumber_2.display(str(mi)+":0"+str(se))
            #else:
             #   self.lcdNumber_2.display(str(mi)+":"+str(se))
            
    def task_1(self):
        global num
        global i
        global mi
        global se
        global cb
        global cb1
        global mi1
        global se1
        cb1 = cb1+1
        if cb1==3 and cb!=3:
            self.lcdNumber_3.display(str(num)+":"+str(i))
            mi = num
            se = i
        if cb1==3 and cb==3:
            mi=num-mi1
            se = i-se1
            self.lcdNumber_3.display(str(mi)+":"+str(se))
            
        '''if cb==6:
            self.lcdNumber_3.display(str(num-mi)+":"+str(i-se))'''
        print("cb1",cb1)
        #if cb/3==1:
        #loop()   
        #if self.checkBox.isChecked() and self.checkBox_2.isChecked() and self.checkBox_3.isChecked():
            
          #mi=num
          #se = i
        #if cb/3==2:
            
         #   mi=num-mi
          #  se=i-se
        #if self.checkBox_4.isChecked() and self.checkBox_5.isChecked()and self.checkBox_6.isChecked():
            
         #   if i<10:
                
          #      self.lcdNumber_3.display(str(mi)+":0"+str(se))
           # else:
            #    self.lcdNumber_3.display(str(mi)+":"+str(se))
    def loop(self):
        global cb
        global mi
        global se
        
        cb=cb+1
        print(cb)
        
           
        
           
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())

