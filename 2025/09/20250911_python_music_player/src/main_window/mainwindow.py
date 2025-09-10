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

        # 再生位置スライダー
        pos_layout = QtWidgets.QHBoxLayout()
        self.label_pos = QtWidgets.QLabel("00:00")
        self.slider_pos = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal)
        self.slider_pos.setRange(0, 100)  # ダミー範囲
        self.label_dur = QtWidgets.QLabel("03:45")

        pos_layout.addWidget(self.label_pos)
        pos_layout.addWidget(self.slider_pos, 1)
        pos_layout.addWidget(self.label_dur)
        layout.addLayout(pos_layout)

        # コントロールボタン
        controls_layout = QtWidgets.QHBoxLayout()
        self.btn_play = QtWidgets.QPushButton("▶ Play")
        self.btn_pause = QtWidgets.QPushButton("⏸ Pause")
        self.btn_stop = QtWidgets.QPushButton("⏹ Stop")
        controls_layout.addWidget(self.btn_play)
        controls_layout.addWidget(self.btn_pause)
        controls_layout.addWidget(self.btn_stop)
        layout.addLayout(controls_layout)

        # 音量スライダー
        vol_layout = QtWidgets.QHBoxLayout()
        vol_label = QtWidgets.QLabel("Volume")
        self.slider_vol = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal)
        self.slider_vol.setRange(0, 100)  # ダミー範囲
        self.slider_vol.setValue(50)
        self.label_vol = QtWidgets.QLabel("50%")

        vol_layout.addWidget(vol_label)
        vol_layout.addWidget(self.slider_vol, 1)
        vol_layout.addWidget(self.label_vol)
        layout.addLayout(vol_layout)

        # --- イベント接続（確認用の print 出力だけ） ---
        self.btn_play.clicked.connect(lambda: print("Play clicked"))
        self.btn_pause.clicked.connect(lambda: print("Pause clicked"))
        self.btn_stop.clicked.connect(lambda: print("Stop clicked"))

        self.slider_pos.valueChanged.connect(lambda v: print(f"Position slider: {v}"))
        self.slider_vol.valueChanged.connect(
            lambda v: self.label_vol.setText(f"{v}%")
        )


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
