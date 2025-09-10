# 🎵 QMediaPlayer 解説ドキュメント（PyQt版）

## ✅ 目的
`QMediaPlayer` は、PyQt（PyQt5 / PyQt6）における**音声・動画再生を制御する中核クラス**です。

本ドキュメントでは以下の点をカバーします：

- `QMediaPlayer` の基本的な使い方
- 必須補助クラス `QAudioOutput`（PyQt6）
- 再生操作、シーク、音量調整
- シグナル（再生状態、位置変更など）
- よくあるエラーと注意点（特にマルチスレッドやコーデック問題）

## 🧱 基本構成（PyQt6）

```python
from PyQt6.QtMultimedia import QMediaPlayer, QAudioOutput
from PyQt6.QtCore import QUrl

audio_output = QAudioOutput()
player = QMediaPlayer()
player.setAudioOutput(audio_output)

# ファイル読み込み
url = QUrl.fromLocalFile("music/track1/sample1.mp3")
player.setSource(url)

# 操作
player.play()
player.pause()
player.stop()
```

> ⚠ PyQt5 では `QAudioOutput` は不要で、`setMedia(QMediaContent)` を使います。

## 🛠 主な操作メソッド

| メソッド                      | 概要                                    |
|------------------------------|-----------------------------------------|
| `setSource(QUrl)`            | （PyQt6）音源ファイルやストリームの設定 |
| `play()`                     | 再生開始                                |
| `pause()`                    | 一時停止                                |
| `stop()`                     | 停止（先頭に戻る）                      |
| `setPosition(ms)`            | 再生位置（ミリ秒）を指定                |
| `position()`                 | 現在の再生位置を取得                    |
| `duration()`                 | トラックの総時間（ms）を取得            |
| `setVolume(0.0 ~ 1.0)`       | （PyQt6）音量調整                        |
| `volume()`                   | 現在の音量を取得（0.0～1.0）            |

## 📡 主なシグナル（signal）

| シグナル名              | 説明                                                 |
|-------------------------|------------------------------------------------------|
| `durationChanged(int)`  | 曲の長さが変わったとき（読み込み後に通知）          |
| `positionChanged(int)`  | 再生位置が変わったとき（スライダー更新などに）       |
| `mediaStatusChanged(Enum)` | 再生終了・エラー・バッファ状態などのステータス通知 |
| `errorOccurred(error, str)` | 再生失敗時のエラー                                 |

```python
player.durationChanged.connect(lambda d: print(f"Duration: {d} ms"))
player.positionChanged.connect(lambda p: print(f"Now: {p} ms"))
```

## 🔄 状態確認用プロパティ

| プロパティ / メソッド      | 説明                                    |
|---------------------------|-----------------------------------------|
| `mediaStatus()`           | `QMediaPlayer.MediaStatus` 列挙型       |
| `playbackState()`         | `QMediaPlayer.PlaybackState` 列挙型    |
| `isPlaying()`（自作）     | 再生中かどうか（再生中なら True）       |

```python
from PyQt6.QtMultimedia import QMediaPlayer

if player.playbackState() == QMediaPlayer.PlaybackState.PlayingState:
    print("Playing")
```

## 🧨 よくある注意点・エラー

### ① ファイルを再生できない
- **原因**: OS に必要なコーデックが存在しない。
- **対処**:
  - Windows: `.mp3` や `.wav` は Media Foundation が対応済み（再生できるはず）
  - `.flac`, `.m4a`, `.ogg` などは失敗することも（OS依存）

### ② 再生が始まらない（`play()`しても無音）
- `setSource()` の呼び出し直後は**まだ再生可能状態でない**ことがある。
- 対処：`mediaStatusChanged()` で `LoadedMedia` 状態を検知してから `play()` を呼ぶか、ユーザー操作で再生。

### ③ マルチスレッドでクラッシュ
- **QMediaPlayerはスレッド移動不可（特にGUI外）！**
- 常に **GUIスレッド上で生成・操作すること**。
- 時間のかかる処理（曲一覧取得、波形生成など）を `QThread` にするのはOK。

## 📦 状態列挙型（MediaStatus / PlaybackState）

### `QMediaPlayer.MediaStatus`（例）
- `NoMedia`
- `LoadingMedia`
- `LoadedMedia`
- `BufferingMedia`
- `BufferedMedia`
- `EndOfMedia`
- `InvalidMedia`

### `QMediaPlayer.PlaybackState`（例）
- `PlayingState`
- `PausedState`
- `StoppedState`

## ⚖ PyQt6 と PyQt5 の違い

| 機能                 | PyQt5                          | PyQt6                             |
|----------------------|--------------------------------|-----------------------------------|
| 初期化              | `QMediaPlayer()`               | `QMediaPlayer()` + `QAudioOutput()` |
| 音源設定            | `setMedia(QMediaContent(...))` | `setSource(QUrl(...))`            |
| 音量範囲            | `0～100`（整数）               | `0.0～1.0`（float）               |
| ライブラリ名         | `PyQt5.QtMultimedia`           | `PyQt6.QtMultimedia`              |

## 📋 再生の最小構成（PyQt6）

```python
from PyQt6.QtWidgets import QApplication
from PyQt6.QtMultimedia import QMediaPlayer, QAudioOutput
from PyQt6.QtCore import QUrl
import sys

app = QApplication(sys.argv)

audio = QAudioOutput()
player = QMediaPlayer()
player.setAudioOutput(audio)
player.setSource(QUrl.fromLocalFile("music/track1/sample1.mp3"))
player.play()

sys.exit(app.exec())
```

## 🎯 実装上のポイント

- **再生位置の取得** → `player.position()` を `QTimer` で定期取得
- **シーク** → `player.setPosition(ms)`
- **音量スライダー**と連携 → `slider.valueChanged.connect(lambda v: player.setVolume(v / 100.0))`
- **再生終了後に次の曲へ** → `mediaStatusChanged` で `EndOfMedia` を検知

## 🔚 まとめ：`QMediaPlayer` を使う際の心得

✅ **GUIスレッド上で使うこと**  
✅ **メディア状態の遷移を監視すること**（再生開始タイミングを誤らない）  
✅ **コーデック対応はOS依存なので注意**  
✅ **PyQt5と6でAPIが異なるため、明示的に吸収層をつくると安全**  
