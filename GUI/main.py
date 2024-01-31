import sys
from PyQt6.QtWidgets import QApplication, QMainWindow
from gui import Ui_mainUI
import serial, serial.tools.list_ports
from PyQt6.QtCore import QTimer
from uart import Uart_serial
from flash import Flash
from device import Device
class Mainwindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_mainUI()
        self.ui.setupUi(self)
        self.uart = Uart_serial(self.ui)
        self.flash = Flash(self.ui,self.uart)
        self.device = Device(self.ui)
        print(self.device.device_info['STM32_F411-DIS'])

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_win = Mainwindow()
    main_win.show()
    sys.exit(app.exec())
