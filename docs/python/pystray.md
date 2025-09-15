# pystray 入門ドキュメント

## 概要
`pystray` は、システムトレイ（通知領域）にアイコンを置き、ツールチップや右クリックメニューを提供するためのクロスプラットフォームなライブラリです。  
Windows, macOS, Linux で動作します。

---

## インストール
```bash
pip install pystray pillow
```

- `pystray` … トレイアイコン制御ライブラリ
- `Pillow` … アイコン画像を生成するために利用（必須）

---

## 基本の使い方

### 1. 必要なモジュールをインポート
```python
import pystray
from PIL import Image, ImageDraw
```

### 2. アイコン用の画像を作成
アイコン画像は `PIL.Image` オブジェクトで作る必要があります。

```python
def create_image():
    # 16x16 のシンプルな赤丸アイコン
    img = Image.new('RGB', (64, 64), color=(255, 255, 255))
    draw = ImageDraw.Draw(img)
    draw.ellipse((16, 16, 48, 48), fill=(255, 0, 0))
    return img
```

### 3. メニューの定義
右クリックメニューを `pystray.Menu` で作成します。

```python
from pystray import MenuItem as item

def on_quit(icon, item):
    icon.stop()

menu = (item('終了', on_quit),)
```

### 4. アイコンを生成して表示
```python
icon = pystray.Icon("令和表示", create_image(), "令和7年", menu)
icon.run()
```

---

## よく使うメソッド・プロパティ

- `Icon(name, image, title, menu)`  
  - `name`: 内部名（任意の文字列）  
  - `image`: PIL で作った画像  
  - `title`: ツールチップ文字列  
  - `menu`: 右クリックメニュー（`MenuItem` のタプル）

- `icon.run()`  
  - 常駐開始。ブロッキングで処理が止まる。

- `icon.stop()`  
  - アイコンを終了（メニューから呼ぶのが一般的）。

- `icon.title = "新しいタイトル"`  
  - ツールチップを変更できる。

- `icon.icon = 新しいPIL画像`  
  - アイコン画像を変更できる。

---

## サンプルコード

```python
import pystray
from pystray import MenuItem as item
from PIL import Image, ImageDraw

def create_image():
    # 青い四角のアイコン
    img = Image.new('RGB', (64, 64), color=(255, 255, 255))
    draw = ImageDraw.Draw(img)
    draw.rectangle((16, 16, 48, 48), fill=(0, 0, 255))
    return img

def on_quit(icon, item):
    icon.stop()

# メニュー作成
menu = (item('終了', on_quit),)

# アイコン生成
icon = pystray.Icon("test", create_image(), "ツールチップ表示テキスト", menu)

# 実行
icon.run()
```

---

## 注意点
- `icon.run()` は無限ループに入るため、後続処理は実行されない。  
  → 別スレッドを使うか、`threading` で更新処理を並行実行する必要がある。
- 画像サイズは 16x16, 32x32, 64x64 あたりを推奨。大きすぎると縮小されてぼやける。
- Windows ではタスクバーの隠れたアイコンの中に入る場合がある。

---
