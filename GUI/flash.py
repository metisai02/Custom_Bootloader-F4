from PyQt6.QtWidgets import QFileDialog ,QMessageBox
import os


class Flash:
    def __init__(self, ui, uart):
        self.ui = ui
        self.uart = uart
        self.message_box = QMessageBox() 
        self.ui.btnBrowser.clicked.connect(self.browser_file)
        self.ui.chbHexfile.clicked.connect(self.value_handler)
        self.ui.btnProgram.clicked.connect(self.program_handler)
        self.hex_data = []
        self.base_addr = ""

    def browser_file(self):
        file_dialog = QFileDialog()
        file_dialog.setFileMode(QFileDialog.FileMode.ExistingFiles)
        file_dialog.setNameFilter("Data file (*.hex)")
        if file_dialog.exec():
            selected_files = file_dialog.selectedFiles()
            if selected_files:
                file_path = selected_files[0]
                self.ui.tedHexfile.setText(file_path)

    def hex_file_handler(self):
        file_path = self.ui.tedHexfile.toPlainText()
        try:
            with open(file_path, "r") as file:
                lines = file.readlines()
                for line in lines:
                    self.hex_data.append(line)
                    self.hex_string_handler(line)
        except Exception as e:
            self.message_box.setWindowTitle("Error")
            self.message_box.setText(f"Error while processing hex file: {e}")
            self.message_box.exec()

    def hex_string_handler(self, data):
        if data[0] != ":":
            return
        length = data[1:3]
        offset_addr = data[3:7]
        record_type = data[7:9]
        if record_type == "00":  # data
            address = self.base_addr + offset_addr
            substring_data = data[9:]
            self.send_data_to_board(address, length, substring_data)
            # todo send data
        elif record_type == "01":  # End of File Record
            print("Done to write flash")
            return
        elif record_type == "02":  # Extended Segment Address Record
            pass
        elif record_type == "04":  # Extended Linear Address Record
            self.base_addr = data[9:13]

    def specific_value_handler(self):
        data = self.ui.tedValue.toPlainText()
        start_addr = self.ui.tedSAddr.toPlainText()
        length = self.ui.tedLength.toPlainText()
        if not all([data, start_addr, length]):
            self.message_box.setWindowTitle("Error")
            self.message_box.setText("Please enter all values")
            self.message_box.exec()
            return
        length = int(length)
        hex_length = hex(length)[2:].zfill(2)  
        self.send_data_to_board(start_addr, hex_length, data)

    def send_data_to_board(self, address, length, data):
        HEADER = "FFFF"
        data_frame = HEADER + length + address + data
        data_an_integer = int(data_frame, 16)
        data_bytes = bytes.fromhex(data_frame)
        print(data_bytes)
        #self.uart.serialPort.write(data_bytes)

    def value_handler(self):
        if self.ui.chbHexfile.isChecked():
            self.ui.grFlash.setEnabled(False)
            self.ui.btnBrowser.setEnabled(True)
        else:
            self.ui.grFlash.setEnabled(True)
            self.ui.btnBrowser.setEnabled(False)

    def program_handler(self):
        print("hello")
        if self.ui.chbHexfile.isChecked():
            self.hex_file_handler()
        else:
            self.specific_value_handler()
