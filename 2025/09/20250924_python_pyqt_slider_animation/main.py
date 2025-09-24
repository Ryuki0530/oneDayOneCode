import sys
from PyQt6.QtCore import Qt, QPropertyAnimation, QEasingCurve
from PyQt6.QtGui import QIntValidator
from PyQt6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QLineEdit, QSlider
)

class SliderDemo(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("入力連動スライダー（アニメーション）")
        self.resize(420, 140)

        # ---- ウィジェット ----
        self.input_label = QLabel("値(0-100)：")
        self.input_edit = QLineEdit()
        self.input_edit.setPlaceholderText("例: 42（Enterで確定）")
        self.input_edit.setValidator(QIntValidator(0, 100, self))
        self.value_label = QLabel("現在値: 0")

        self.slider = QSlider(Qt.Orientation.Horizontal)
        self.slider.setRange(0, 100)
        self.slider.setValue(0)

        # スライダーを「見た目は有効のまま」操作不可にする
        self.slider.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
        self.slider.setFocusPolicy(Qt.FocusPolicy.NoFocus)

        # ---- レイアウト ----
        row = QHBoxLayout()
        row.addWidget(self.input_label)
        row.addWidget(self.input_edit)

        layout = QVBoxLayout(self)
        layout.addLayout(row)
        layout.addWidget(self.slider)
        layout.addWidget(self.value_label)

        # ---- シグナル ----
        self.input_edit.editingFinished.connect(self.handle_input_confirmed)
        self.slider.valueChanged.connect(
            lambda v: self.value_label.setText(f"現在値: {v}")
        )

        # 進行中アニメーションの参照
        self._anim: QPropertyAnimation | None = None

    def handle_input_confirmed(self):
        text = self.input_edit.text().strip()
        if text == "":
            return

        try:
            target = int(text)
        except ValueError:
            return

        # 0〜100 にクリップ
        target = max(0, min(100, target))
        self.input_edit.setText(str(target))

        self.animate_slider_to(target)

    def animate_slider_to(self, target: int):
        current = self.slider.value()
        if current == target:
            return

        # 進行中のアニメーションがあれば止める
        if self._anim is not None:
            self._anim.stop()
            self._anim.deleteLater()
            self._anim = None

        self._anim = QPropertyAnimation(self.slider, b"value", self)
        self._anim.setStartValue(current)
        self._anim.setEndValue(target)

        # 距離に応じた所要時間（一定速度っぽく）
        dist = abs(target - current)
        ms_per_unit = 14  # 調整ポイント（10〜20あたりが無難）
        duration = max(200, min(1000, dist * ms_per_unit))
        self._anim.setDuration(duration)

        # イージング（好みで変更可）
        self._anim.setEasingCurve(QEasingCurve.Type.OutCubic)

        self._anim.start()

def main():
    app = QApplication(sys.argv)
    w = SliderDemo()
    w.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()
