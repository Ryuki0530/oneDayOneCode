from PyQt6 import QtWidgets, QtCore


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Music Player")
        self.setGeometry(100, 100, 800, 600)

        # 中央ウィジェットとレイアウト
        central_widget = QtWidgets.QWidget(self)
        self.setCentralWidget(central_widget)

        layout = QtWidgets.QVBoxLayout(central_widget)

        # コントロールボタンの配置
        controls_layout = QtWidgets.QHBoxLayout()

        self.btn_play = QtWidgets.QPushButton("▶ Play")
        self.btn_pause = QtWidgets.QPushButton("⏸ Pause")
        self.btn_stop = QtWidgets.QPushButton("⏹ Stop")

        controls_layout.addWidget(self.btn_play)
        controls_layout.addWidget(self.btn_pause)
        controls_layout.addWidget(self.btn_stop)

        layout.addLayout(controls_layout)

        # --- イベント接続（とりあえずprintで確認用） ---
        self.btn_play.clicked.connect(self.on_play_clicked)
        self.btn_pause.clicked.connect(self.on_pause_clicked)
        self.btn_stop.clicked.connect(self.on_stop_clicked)

    # 以下は確認用スロット
    def on_play_clicked(self):
        print("Play button clicked")

    def on_pause_clicked(self):
        print("Pause button clicked")

    def on_stop_clicked(self):
        print("Stop button clicked")


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
