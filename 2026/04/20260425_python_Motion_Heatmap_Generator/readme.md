# 今日の課題：Motion Heatmap Generator

## 概要

Webカメラまたは動画ファイルを入力として、動きが多かった場所を可視化する
「モーションヒートマップ生成ツール」を作成してください。

動画内でフレーム間差分を計算し、動きが検出された領域を蓄積していきます。
最後に、動きが多かった場所ほど強く表示されるヒートマップ画像を出力します。

## Branch name

```text
opencv-motion-heatmap
```

## 使用言語

Python

## 使用ライブラリ

- opencv-python
- numpy

## 入力

動画ファイルをコマンドライン引数で受け取ります。

```bash
python main.py input.mp4
```

引数がない場合は、Webカメラを使うようにしてもよいです。

```bash
python main.py
```

## 処理内容

1. 動画を1フレームずつ読み込む
2. 各フレームをグレースケールに変換する
3. 前のフレームとの差分を取る
4. 差分画像を二値化して、動きがある部分だけを抽出する
5. 動きがあった画素を累積用配列に加算する
6. 最後に累積結果を0〜255の範囲に正規化する
7. カラーマップを適用してヒートマップ画像として保存する

## 出力

以下の画像を保存してください。

```text
motion_heatmap.png
```

余裕があれば、以下も保存してください。

```text
last_frame.png
overlay_heatmap.png
```

`overlay_heatmap.png` は、最後のフレームにヒートマップを半透明で重ねた画像です。

## 実装条件

- OpenCVの `cv2.VideoCapture` を使うこと
- フレーム間差分には `cv2.absdiff()` を使うこと
- 二値化には `cv2.threshold()` を使うこと
- カラーマップには `cv2.applyColorMap()` を使うこと
- ヒートマップの累積には NumPy 配列を使うこと

## 追加仕様

余裕があれば、以下も実装してください。

- `q` キーで途中終了できるようにする
- 処理中の差分画像をウィンドウ表示する
- 動き検出のしきい値をコマンドライン引数で変更できるようにする
- 最後のフレームにヒートマップを重ねた画像を保存する
- 動きが多かった領域を四角形で囲む

## 実行例

```bash
python main.py sample.mp4
```

Webカメラを使う場合：

```bash
python main.py
```

## 期待する動作

動画を解析したあと、動きが多かった場所が赤や黄色で強調された
ヒートマップ画像が生成されます。

例えば、人が画面の中央をよく通る動画では、中央付近が強く反応します。

## ヒント

フレーム間差分の例：

```python
diff = cv2.absdiff(prev_gray, gray)
```

二値化の例：

```python
_, mask = cv2.threshold(diff, 30, 255, cv2.THRESH_BINARY)
```

累積の例：

```python
motion_accum += mask.astype(np.float32)
```

正規化の例：

```python
heatmap_gray = cv2.normalize(motion_accum, None, 0, 255, cv2.NORM_MINMAX)
heatmap_gray = heatmap_gray.astype(np.uint8)
```

カラーマップの例：

```python
heatmap_color = cv2.applyColorMap(heatmap_gray, cv2.COLORMAP_JET)
```

画像を重ねる例：

```python
overlay = cv2.addWeighted(last_frame, 0.6, heatmap_color, 0.4, 0)
```

## ゴール

OpenCVで動画を扱いながら、以下を練習してください。

- フレーム差分
- 二値化
- NumPyによる累積処理
- カラーマップによる可視化
- 画像保存