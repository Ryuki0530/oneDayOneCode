import sys
import argparse
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QListWidget, QPushButton
from PyQt6.QtCore import QTimer, QDateTime

class MainWindow(QWidget):
    def __init__(self, args):

        super().__init__()
        self.setWindowTitle("window watcher")
        self.status_label = QLabel("状態: 初期化中")
        self.log_list = QListWidget()
        self.start_btn = QPushButton("開始")
        self.stop_btn = QPushButton("停止")
        self.stop_btn.setEnabled(False)

        layout = QVBoxLayout()
        layout.addWidget(self.status_label)
        layout.addWidget(self.log_list)
        layout.addWidget(self.start_btn)
        layout.addWidget(self.stop_btn)
        self.setLayout(layout)

        self.timer = QTimer(self)
        self.timer.setInterval(args.interval * 1000)
        self.timer.timeout.connect(self.check_window)

        self.start_btn.clicked.connect(self.start_detection)
        self.stop_btn.clicked.connect(self.stop_detection)

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

    def add_log(self, message):
        self.log_list.addItem(message)
        self.log_list.scrollToBottom()

    def check_window(self):
        now = QDateTime.currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
        self.add_log(f"[{now}] Checking window...")
        # ここにウィンドウ監視のロジックを追加

    def closeEvent(self, event):
        event.accept()
