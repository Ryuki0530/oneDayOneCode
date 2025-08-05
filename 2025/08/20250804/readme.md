# 🎥 問題: リアルタイムモザイク追跡カメラ

## 🎯 目的
カメラ映像に対して、顔を検出し自動的にモザイクをかけるリアルタイム映像処理アプリケーションを作成せよ。

## ✅ 要件

### 1. カメラ映像の取得
- OpenCV (`cv2.VideoCapture`) を用いて、PCのWebカメラからリアルタイムに映像を取得する。

### 2. 顔検出
- OpenCVに内蔵されている Haar Cascade を用いて、毎フレームごとに顔を検出する。
  - モデルファイル: `haarcascade_frontalface_default.xml`（OpenCVのインストール時に含まれている）
利用例
```python
import cv2
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
```


### 3. モザイク処理
- 検出された顔領域に対して、**モザイク処理**（画像の縮小→拡大で荒くする）を適用し、元映像に合成する。

### 4. 表示と終了操作
- 処理された映像をウィンドウ表示。
- キー `q` を押すと処理を終了するようにする。

## 🔄 オプション（任意）

- `--debug` オプションで、モザイクをかける代わりに検出矩形だけを描画するように変更。
- モザイクの荒さを調整できるように `--pixelation 10` のようなコマンドライン引数で指定できるようにする。

## 💡 ヒント

- 顔検出用モデルの読み込みには以下を使う：
```python
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
```
モザイク処理の方法（例）：

```python
def mosaic(img, ratio=0.1):
    small = cv2.resize(img, None, fx=ratio, fy=ratio, interpolation=cv2.INTER_NEAREST)
    return cv2.resize(small, img.shape[:2][::-1], interpolation=cv2.INTER_NEAREST)
```
🧪 実行例
```bash
python mosaic_camera.py
```
```bash
python mosaic_camera.py --debug
```
```bash
python mosaic_camera.py --pixelation 0.05
```

---

### 🧠 学べること

- OpenCVを用いたリアルタイム動画処理の基本
- Haar Cascadeによる物体検出
- モザイク処理の実装テクニック
- コマンドライン引数によるアプリケーションの柔軟性

---