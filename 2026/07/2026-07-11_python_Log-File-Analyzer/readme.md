# OneDayOneCode

## タイトル
Log File Analyzer

## 難易度
★★☆☆☆

## 問題

ログファイルを解析し、ログレベルごとの件数を集計するプログラムを作成してください。

入力ファイル(log.txt)の例

INFO: System started
INFO: Camera initialized
WARNING: Battery low
ERROR: Failed to open device
INFO: Retry connection
ERROR: Timeout
DEBUG: Packet received
WARNING: Temperature high
INFO: Shutdown

### 要件

- log.txt を読み込む
- 行頭のログレベルを取得する
- 各ログレベルの件数を表示する
- 出力はログレベル順ではなく、件数の多い順に表示する
- 同じ件数ならアルファベット順

期待される出力例

INFO : 4
ERROR : 2
WARNING : 2
DEBUG : 1

## 制約

collections.Counter を利用してよい。

## 発展課題

★
ERROR の行だけを errors.txt に保存する。

★★
ログレベルごとの割合(%)も表示する。

例

INFO : 4 (44.4%)
ERROR : 2 (22.2%)

★★★
コマンドライン引数でファイル名を指定できるようにする。

python main.py sample.log
