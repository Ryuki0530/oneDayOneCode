# 今日の課題（Python）: “CPUの気分”を顔文字で出す監視ツール

## 概要
PCの状態（CPU / メモリ / ディスク / ネットワーク）を定期的に取得し、その状態に応じた「気分（顔文字）」をターミナルに表示する監視ツールを作成してください。  
“監視”といっても、堅いダッシュボードではなく、**ゆるい感情表現**で状態変化が分かることが目的です。

---

## 要件

### 1) 実行方法
- コマンドラインで実行できること（例）
  - `py main.py`
- 終了は `Ctrl + C` でできること

### 2) 取得する指標（最低限）
次の4つを **一定間隔（例: 1秒〜5秒）** で取得してください。

- CPU使用率（%）
- メモリ使用率（%）
- ディスク使用率（%） ※Cドライブなど
- ネットワーク送受信量（B/s） ※前回との差分から算出

### 3) 顔文字ルール（例）
状態に応じて、顔文字と短いコメントを表示します。  
下は一例なので、**あなたのセンスで増やしてOK**。

- 平常: `(・ω・)`  
  - CPU < 40% かつ メモリ < 60% かつ ディスク < 80%
- 忙しい: `(；｀皿´)`  
  - CPU >= 80%
- 息切れ: `(；´Д｀)`  
  - メモリ >= 85%
- 断末魔: `(；ﾟДﾟ)`  
  - ディスク >= 95%
- 通信祭り: `＼(＾o＾)／`  
  - ネットワーク合計（送+受）が一定以上（例: 1MB/s）

※複数条件に当てはまる場合は、**優先順位**を自分で決めてください（例：断末魔が最優先）。

### 4) 出力形式（例）
毎回、同じ行を上書きして更新するようにすると“監視っぽさ”が出ます（任意）。  
最低限、以下が分かればOK：
- 現在時刻
- 各指標
- 気分（顔文字 + コメント）

例（イメージ）:
```
12:34:56 CPU 12% MEM 43% DISK 71% NET 12KB/s  (・ω・) 平常運転
```

---

## 発展要素（好きなのを1つ以上）
- 引数で更新間隔を変えられる（例：`--interval 2.0`）
- しきい値を設定ファイル（JSON）から読み込む
- ログをファイル保存（CSVなど）
- Windows通知（トースト通知）※余裕があれば
- “気分履歴”を集計して、最後に「今日の気分ランキング」を出す

---

## 禁止・制約
- OSは Windows 11 を前提
- VSCodeで実行できること
- なるべく **追加ライブラリを少なく**（ただし、下記 `psutil` は使用してOK）

---

# 使用ライブラリ解説（必要なもの）

## 1) `psutil`（必須）
OSの各種リソース情報を取るための定番ライブラリです。

### インストール
```bash
py -m pip install psutil
```

### CPU使用率
```python
import psutil

cpu = psutil.cpu_percent(interval=None)  # 直近のCPU使用率(%)
```
- `interval=None` は「前回呼び出し以降の平均との差分」を返すイメージです。
- 初回はブレることがあるため、**最初に1回捨て読み**するのもアリです。

### メモリ使用率
```python
mem = psutil.virtual_memory().percent  # メモリ使用率(%)
```

### ディスク使用率（Cドライブ例）
```python
disk = psutil.disk_usage("C:\\").percent
```
- Windowsでは `"C:\\ "` のように書きます（バックスラッシュはエスケープが必要）。

### ネットワーク送受信量（B/s）
`psutil.net_io_counters()` は累積値なので、前回との差分から速度を作ります。

```python
import psutil
import time

prev = psutil.net_io_counters()
time.sleep(1.0)
cur = psutil.net_io_counters()

sent_per_s = cur.bytes_sent - prev.bytes_sent
recv_per_s = cur.bytes_recv - prev.bytes_recv
total_per_s = sent_per_s + recv_per_s
```

---

## 2) `time`（標準ライブラリ）
スリープや時刻表示に使います。

```python
import time

time.sleep(1.0)
now_str = time.strftime("%H:%M:%S")
```

---

## 3) `argparse`（標準ライブラリ・任意）
更新間隔などをコマンドライン引数で受け取るための標準機能です。

```python
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--interval", type=float, default=1.0)
args = parser.parse_args()
interval = args.interval
```

---

## 4) ターミナル行の上書き（任意）
毎回新しい行を出すのではなく、同じ行を更新できます。

```python
print("\r" + line, end="", flush=True)
```

- `\r` はカーソルを行頭に戻します
- `end=""` で改行しない
- `flush=True` で即反映

終了時に表示が崩れないよう、`Ctrl+C` を捕まえて改行して終わるのがおすすめです。

```python
try:
    while True:
        ...
except KeyboardInterrupt:
    print()  # 改行して終了
```

---

## 目標
- 「いまPCがどんな感じか」が、数字だけでなく顔文字で直感的に分かる
- しきい値や表現にあなたの個性が出ている
