from PyQt5 import QtWidgets, uic,QtCore
import sys
import time
import datetime
import os
import websocket


class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__() # Call the inherited classes __init__ method
        uic.loadUi('car_ui.ui', self) # Load the .ui file
        
        

        self.start_btn = self.findChild(QtWidgets.QPushButton, 'start_btn') # Find the button
        self.start_btn.clicked.connect(self.start_btn_func) # Remember to pass the definition/method, not the return value!

        
        self.exit_btn = self.findChild(QtWidgets.QPushButton, 'exit_btn') # Find the button
        self.exit_btn.clicked.connect(self.exit_btn_func) # Remember to pass the definition/method, not the return value!
        self.exit_btn.setEnabled(False)
        
        self.fullname_txtbox = self.findChild(QtWidgets.QLineEdit, 'fullname_txtbox')
        self.age_txtbox = self.findChild(QtWidgets.QLineEdit, 'age_txtbox')
        self.phone_txtbox = self.findChild(QtWidgets.QLineEdit, 'phone_txtbox')

        self.male_radioButton = self.findChild(QtWidgets.QRadioButton, 'male_radioButton')
        self.female_radioButton = self.findChild(QtWidgets.QRadioButton, 'female_radioButton')
        self.testtime_lcd = self.findChild(QtWidgets.QLCDNumber, 'testtime_lcd')

        self.pd_groupBox = self.findChild(QtWidgets.QGroupBox, 'pd_groupBox')
        self.control_groupBox = self.findChild(QtWidgets.QGroupBox, 'control_groupBox')
        self.control_groupBox.setEnabled(False)
        


        self.file = None


        self.car_data_timer = QtCore.QTimer()
        self.car_data_timer.timeout.connect(self.car_data_timer_tick)
        
        self.groupbox_timer = QtCore.QTimer()
        self.groupbox_timer.timeout.connect(self.groupbox_timer_tick)
        self.groupbox_timer.start(100)

        
        self.testtime_lcd_timer = QtCore.QTimer()
        self.testtime_lcd_timer.timeout.connect(self.testtime_lcd_timer_tick)
        self.Ttime = 0


        self.show() # Show the GUI


    def testtime_lcd_timer_tick(self):
        self.Ttime += 0.1
        self.testtime_lcd.display(str(str(int(self.Ttime//60))+':'+str(int(self.Ttime%60))))
        self.testtime_lcd.repaint()


    def groupbox_timer_tick(self):
        if self.fullname_txtbox.text() == '' or self.age_txtbox.text() == '' or self.phone_txtbox.text() == '' or (not self.male_radioButton.isChecked() and not self.female_radioButton.isChecked()):
            self.control_groupBox.setEnabled(False)
        else :
            self.control_groupBox.setEnabled(True)

    def car_data_timer_tick(self):
        # pass
        try:
            ws.send("nothing")
            result = ws.recv()
            print(result)
            self.file.write(str(result + ',' + str(round(self.Ttime,1)) +'\n'))
        except Exception as e:
            print('error in tick',e)

    def start_btn_func(self):
        # This is executed when the button is pressed

        if self.male_radioButton.isChecked(): gender = 'male'
        else : gender = 'female'

        self.file = open(os.path.join(directory, str(self.fullname_txtbox.text() + '_' + datetime.datetime.now().strftime("%I_%M_%p_%B_%d_%Y") + '.csv')), "w") 
        self.file.write(str(self.fullname_txtbox.text() + ',' + self.age_txtbox.text() + ',' + self.phone_txtbox.text() + ',' + gender + '\n'))
        self.file.write(str('ACx,ACy,ACz,encoder1,encode2\n')) 
        self.pd_groupBox.setEnabled(False)
        self.start_btn.setEnabled(False)
        self.exit_btn.setEnabled(True)
        self.testtime_lcd_timer.start(100)
        self.car_data_timer.start(100)
        
        
    
    def exit_btn_func(self):
        # This is executed when the button is pressed
        self.file.write(str(str(int(self.Ttime//60))+':'+str(int(self.Ttime%60)))) 
        self.file.close() 
        print('record done!')
        self.fullname_txtbox.clear()
        self.age_txtbox.clear()
        self.phone_txtbox.clear()
        self.start_btn.setEnabled(True)
        self.exit_btn.setEnabled(False)
        self.pd_groupBox.setEnabled(True)
        self.testtime_lcd_timer.stop()
        self.Ttime = 0
        self.testtime_lcd.display(0)
        self.testtime_lcd.repaint()
        self.car_data_timer.stop()





if __name__ == "__main__":


    path = os.getcwd()
    directory = str(os.path.join(path, 'car_data'))
    if not os.path.exists(directory):
        os.makedirs(directory)


    while True:
        try:
            ws = websocket.WebSocket()
            ws.connect("ws://10.42.0.55/ws")
            # ws.connect("ws://192.168.4.1/ws") #nodmcu hotspot
            print('connected to car')
            break
        except Exception as e:
            print('cant connect to car',e)
            print('retrying!')



    app = QtWidgets.QApplication(sys.argv) # Create an instance of QtWidgets.QApplication
    window = Ui() # Create an instance of our class
    app.exec_() # Start the application