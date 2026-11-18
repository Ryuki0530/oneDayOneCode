# ThreadSafeQueue

## 問題概要

複数のスレッドから安全にアクセスできるキュー
`ThreadSafeQueue<T>` を実装してください。

このキューは、データを追加するProducerスレッドと、
データを取り出すConsumerスレッドの間で使用します。

Consumerはキューが空の場合、新しいデータが追加されるまで
待機する必要があります。

ただし、CPUを無駄に消費するビジーウェイトは禁止します。

## 実装するクラス

template <typename T>
class ThreadSafeQueue;

以下のメソッドを実装してください。

### 1. void push(T value)

- キューの末尾に要素を追加する
- 複数スレッドから同時に呼び出されても安全であること
- 待機中のConsumerが存在する場合は通知すること

### 2. T pop()

- キューの先頭から要素を取り出して返す
- キューが空の場合は要素が追加されるまで待機する
- 複数のConsumerが同時に呼び出しても安全であること

### 3. size_t size() const

- 現在のキューの要素数を返す
- 他のスレッドからpush/popが呼ばれていても安全であること

## 制約条件

- C++17を使用すること
- std::queue<T>を内部コンテナとして使用すること
- std::mutexを用いて排他制御を行うこと
- std::condition_variableを用いて待機・通知を行うこと
- ビジーウェイトは禁止
- 外部ライブラリは禁止

## 動作確認

以下の処理を実装してください。

1. Producerスレッドを1つ起動する
2. Producerは1から10までの整数を順番にpushする
3. Consumerスレッドを1つ起動する
4. Consumerはpopを10回実行する
5. Consumerが取得した値を標準出力する
6. 両スレッドの終了を待機する

### 出力例

Consumed: 1
Consumed: 2
Consumed: 3
Consumed: 4
Consumed: 5
Consumed: 6
Consumed: 7
Consumed: 8
Consumed: 9
Consumed: 10

## 発展課題（任意）

Producerを3つ、Consumerを2つに増やしてください。

- 各Producerは10個の整数を追加する
- Consumer全体で30個の整数を取り出す
- 同じ要素が二重に取り出されないこと
- すべての要素が正常に処理されること

Consumerの終了条件も自分で設計してください。
