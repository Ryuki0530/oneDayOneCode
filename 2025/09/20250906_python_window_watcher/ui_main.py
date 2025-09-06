import sys
import argparse
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QListWidget, QPushButton
from PyQt6.QtCore import QTimer, QDateTime
from winenum import WinEnumService
from models.model import WindowEvent, WindowWatcherModel
class MainWindow(QWidget):
    def __init__(self, args):

        super().__init__()
        self.args = args
        self.setWindowTitle("window watcher")
        self.setFixedSize(900, 600)
        self.status_label = QLabel("状態: 初期化中")
        self.log_list = QListWidget()
        self.start_btn = QPushButton("開始")
        self.stop_btn = QPushButton("停止")
        self.save_log_btn = QPushButton("イベントログ保存")
        self.stop_btn.setEnabled(False)

        layout = QVBoxLayout()
        layout.addWidget(self.status_label)
        layout.addWidget(self.log_list)
        layout.addWidget(self.start_btn)
        layout.addWidget(self.stop_btn)
        layout.addWidget(self.save_log_btn)
        self.setLayout(layout)

        self.timer = QTimer(self)
        self.timer.setInterval(args.interval * 1000)
        self.timer.timeout.connect(self.check_window)

        self.start_btn.clicked.connect(self.start_detection)
        self.stop_btn.clicked.connect(self.stop_detection)
        self.save_log_btn.clicked.connect(self.save_log)

        self.WinEnumService = WinEnumService()
        self.model = WindowWatcherModel()
        self.model_initialized = False

        self.status_label.setText("状態: 待機中")

    def start_detection(self):
        self.status_label.setText("状態: 監視中")
        self.timer.start()
        self.start_btn.setEnabled(False)
        self.stop_btn.setEnabled(True)
        self.check_window()  # 起動直後にも1回実行        

    def stop_detection(self):
        self.status_label.setText("状態: 停止中")
        self.timer.stop()
        self.start_btn.setEnabled(True)
        self.stop_btn.setEnabled(False)
    
    def save_log(self):
        filepath = ".exports/window_events_log.json"
        self.model.save_log(filepath)
        self.add_log(f"イベントログを保存しました: {filepath}")

    def add_log(self, message):
        self.log_list.addItem(message)
        self.log_list.scrollToBottom()

    def init_log(self):
        self.log_list.clear()

    def check_window(self):
        now = QDateTime.currentDateTime().toString("yyyy-MM-dd HH:mm:ss")

        include_empty = self.args.include_empty
        all_styles = self.args.all_styles

        windows = self.WinEnumService.enumerate(include_empty=include_empty, all_styles=all_styles, include_exe=False)
        
        self.init_log()
        self.add_log(f"[{now}] Checked windows.")

        for window in windows:
            self.add_log(f"name: {window.title} pid: {window.pid} class: {window.class_name} fg: {window.is_foreground}")
        self.add_log(f"==================\nTotal windows: {len(windows)}")

        if not self.model_initialized:
            self.model_initialized = True
            self.model.initialize(windows)
        else:
            self.model.update(windows)
        return


    def closeEvent(self, event):
        event.accept()
