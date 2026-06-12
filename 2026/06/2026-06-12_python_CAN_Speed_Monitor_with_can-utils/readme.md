# Python CAN Speed Monitor with can-utils

## 目的

WSL上のcan-utilsを使い、Pythonから仮想CANの受信ログを監視する。

CAN ID `0x100` のデータ先頭1バイトを車速[km/h]として扱い、
60以上なら警告を表示する。

## 事前準備

vcan0を作成する。

```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

確認用に別ターミナルで送信する。

```bash
cansend vcan0 100#3C
cansend vcan0 100#50
cansend vcan0 200#FF
```

`3C` は16進数で60、`50` は16進数で80。

## 作成するファイル

```text
main.py
```

## 仕様

Pythonの `subprocess` を使って、以下のコマンドを起動する。

```bash
candump vcan0
```

受信した行を1行ずつ読み取り、次のような行を解析する。

```text
vcan0  100   [1]  3C
```

条件:

- CAN ID が `100` のときだけ処理する
- データ先頭1バイトを16進数として読み取る
- 60未満なら `OK`
- 60以上なら `WARN`
- CAN ID が `100` 以外なら無視する

## 出力例

```text
speed=60 km/h WARN
speed=80 km/h WARN
```

## ヒント

`subprocess.Popen()` を使うと、Pythonから外部コマンドを起動できる。

```python
subprocess.Popen(
    ["candump", "vcan0"],
    stdout=subprocess.PIPE,
    text=True
)
```

`candump` の出力は空白区切りで分解できる。

```python
parts = line.split()
```

データを16進数から10進数に変換するには次を使う。

```python
value = int("3C", 16)
```

## 余裕があれば

- しきい値をコマンドライン引数で変更できるようにする
- `0x101` をエンジン回転数として追加する
- ログを `can_log.txt` に保存する
```