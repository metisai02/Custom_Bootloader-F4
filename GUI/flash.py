from PyQt6.QtWidgets import QFileDialog
import os


class Flash:
    def __init__(self, ui):
        self.ui = ui
        self.ui.btnBrowser.clicked.connect(self.browser_file)
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
                self.hex_file_handler(file_path)

    def hex_file_handler(self, file_path):
        try:
            with open(file_path, "r") as file:
                lines = file.readlines()
                for line in lines:
                    self.hex_data.append(line)
                    self.hex_string_handler(line)
                print(self.base_addr)
        except Exception as e:
            print(f"Lỗi khi xử lý tệp hex: {e}")

    def hex_string_handler(self, data):
        if data[0] != ":":
            return 1
        length = int((data[1] + data[2]), 16)
        offset_addr = data[3] + data[4] + data[5] + data[6]
        record_type = data[7] + data[8]
        print(f"record_type: {record_type}")
        print(f"length: {length}")
        if record_type == "00":  # data
            address = self.base_addr + offset_addr
            an_integer = int(address, 16)
            hex_value = hex(an_integer)
            string_data = ""
            i = 0
            for char in data:
                i += 1
                if i > 8:
                    string_data += char
            data_an_integer = int(string_data, 16)
            hex_string_data = hex(data_an_integer)
            self.send_hex_to_board(address, length, hex_string_data)

            # todo send data
        elif record_type == "01":  # End of File Record
            print("Done to write flash")
            return 0
        elif record_type == "02":  # Extended Segment Address Record
            pass
        elif record_type == "04":  # Extended Linear Address Record
            self.base_addr = data[9] + data[10] + data[11] + data[12]


    def send_hex_to_board(self,address, length, data):
        header = 0xFFFF
        print(data)
