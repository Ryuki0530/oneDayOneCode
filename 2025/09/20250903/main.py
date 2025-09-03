import sys
import argparse
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QListWidget, QPushButton
from PyQt6.QtCore import QTimer, QDateTime
from sleep_detector import SleepDetector

class MainWindow(QWidget):
    def __init__(self, args):
        super().__init__()
        self.setWindowTitle("寝落ち検出GUI")
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

        self.detector = SleepDetector(args)
        self.timer = QTimer(self)
        self.timer.setInterval(args.interval * 1000)
        self.timer.timeout.connect(self.check_sleep)

        self.start_btn.clicked.connect(self.start_detection)
        self.stop_btn.clicked.connect(self.stop_detection)

        self.status_label.setText("状態: 待機中")

    def start_detection(self):
        self.status_label.setText("状態: 監視中")
        self.timer.start()
        self.start_btn.setEnabled(False)
        self.stop_btn.setEnabled(True)
        self.check_sleep()  # 起動直後にも1回判定

    def stop_detection(self):
        self.status_label.setText("状態: 停止中")
        self.timer.stop()
        self.start_btn.setEnabled(True)
        self.stop_btn.setEnabled(False)

    def check_sleep(self):
        now = QDateTime.currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
        detected, info = self.detector.detect()
        if detected:
            msg = f"{now}：寝落ちを検出 {info}"
            self.log_list.addItem(msg)
        else:
            self.status_label.setText(f"状態: 監視中（{now} 判定OK）")

    def closeEvent(self, event):
        self.detector.release()
        event.accept()

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--view", action="store_true", help="デバッグ表示")
    parser.add_argument("--device", type=int, default=0, help="カメラデバイス番号")
    parser.add_argument("--interval", type=int, default=30, help="判定間隔（秒）")
    parser.add_argument("--csv", type=str, default=None, help="CSVログ出力先")
    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()
    app = QApplication(sys.argv)
    win = MainWindow(args)
    win.show()
    sys.exit(app.exec())