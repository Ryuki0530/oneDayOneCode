# Forked Counter Manager

## 概要

C++のクラスとfork()を利用して、
親プロセスと子プロセスがそれぞれカウンタを管理するプログラムを作成せよ。

## 要件

Counterクラスを作成すること。

### メンバ

- int count

### メソッド

- increment()
  - countを1増やす

- getCount()
  - countを返す

- print()
  - PIDとcountを表示する

表示例:

PID=1234 Count=5

## 処理

1. Counterオブジェクトを生成
2. countを0で初期化
3. fork()を実行

### 子プロセス

- 3回increment()
- print()

### 親プロセス

- 5回increment()
- 子プロセス終了をwait()で待機
- print()

## 実行例

PID=1001 Count=3
PID=1000 Count=5

※ PIDは環境によって異なる

## 学習ポイント

- fork()後はメモリ空間がコピーされる
- 親と子で同じオブジェクトを持っているように見えても実体は別
- クラスのインスタンスもfork後は独立して動作する

## 発展課題（時間があれば）

Counterクラスに以下を追加せよ。

- コンストラクタで初期値を受け取る
- デストラクタで

Destroy PID=xxxx

を表示する

親プロセスと子プロセスでそれぞれデストラクタが呼ばれることを確認せよ。