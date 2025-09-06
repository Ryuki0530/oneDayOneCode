import sys
import argparse
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QListWidget, QPushButton
from PyQt6.QtCore import QTimer, QDateTime
from ui_main import MainWindow


def main():
    args = parse_args()
    app = QApplication(sys.argv)
    win = MainWindow(args)
    win.show()
    sys.exit(app.exec())

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--interval", type=int, default=30, help="判定間隔（秒）")
    parser.add_argument("--include-empty", action="store_true", help="空タイトルを含める")
    parser.add_argument("--all-styles", action="store_true", help="全てのウィンドウスタイルを対象にする")
    return parser.parse_args()


if __name__ == "__main__":
    main()