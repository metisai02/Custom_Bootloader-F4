from PyQt6.QtWidgets import QFileDialog
import os


class Flash:
    def __init__(self, ui, uart):
        self.ui = ui
        self.uart = uart
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
        length = data[1] + data[2]
        offset_addr = data[3:7]
        record_type = data[7:9]
        if record_type == "00":  # data
            address = self.base_addr + offset_addr
            substring_data = data[9:]  # cắt từ vị trí 9 đến hết chuỗi
            self.send_hex_to_board(address, length, substring_data)
            # todo send data
        elif record_type == "01":  # End of File Record
            print("Done to write flash")
            return 0
        elif record_type == "02":  # Extended Segment Address Record
            pass
        elif record_type == "04":  # Extended Linear Address Record
            self.base_addr = data[9:13]

    def send_hex_to_board(self, address, length, data):
        HEADER = "FFFF"
        data_frame = HEADER + length + address + data
        data_an_integer = int(data_frame, 16)
        data_bytes = bytes.fromhex(data_frame)
        print(data_bytes)
        self.uart.serialPort.write(data_bytes)
