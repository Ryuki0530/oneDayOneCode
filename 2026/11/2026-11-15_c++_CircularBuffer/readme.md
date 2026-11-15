
# CircularBuffer

## 問題概要

整数を格納する固定容量のリングバッファを実装してください。

リングバッファは、配列の末尾まで到達すると先頭に戻って
データを格納するデータ構造です。

組込みシステムにおけるセンサーデータの保持や、
通信データの一時保存などに利用されます。

## 仕様

以下のクラスを実装してください。

class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity);

    bool push(int value);
    bool pop(int& value);

    size_t size() const;
    bool empty() const;
    bool full() const;
};

### 1. コンストラクタ

CircularBuffer(size_t capacity)

- 指定された容量のバッファを確保する。
- capacityは1以上とする。
- バッファの初期状態は空とする。

### 2. push(int value)

- バッファの末尾に値を追加する。
- 追加に成功した場合はtrueを返す。
- バッファが満杯の場合はfalseを返す。
- 満杯の場合、既存データを上書きしない。

### 3. pop(int& value)

- 最も古い値を取り出す。
- 取り出した値をvalueに格納する。
- 成功した場合はtrueを返す。
- 空の場合はfalseを返し、valueを変更しない。

### 4. size()

現在格納されている要素数を返す。

### 5. empty()

バッファが空の場合はtrueを返す。

### 6. full()

バッファが満杯の場合はtrueを返す。

## 制約条件

- std::vector、std::deque、std::queueは使用禁止。
- データの格納には動的確保したint配列を使用する。
- new[] / delete[] を使用してメモリを管理する。
- push / pop の時間計算量はO(1)とする。
- バッファ内の要素を移動させてはならない。
- メモリリークを発生させないこと。
- コピーコンストラクタとコピー代入演算子はdeleteする。

## 動作例

CircularBuffer buffer(3);

buffer.push(10);  // true
buffer.push(20);  // true
buffer.push(30);  // true
buffer.push(40);  // false（満杯）

int value;

buffer.pop(value); // true, value = 10
buffer.pop(value); // true, value = 20

buffer.push(40);   // true
buffer.push(50);   // true

buffer.pop(value); // true, value = 30
buffer.pop(value); // true, value = 40
buffer.pop(value); // true, value = 50

buffer.pop(value); // false（空）

## 発展課題（任意）

1. テンプレート化して任意の型を格納可能にする。
2. ムーブコンストラクタを実装する。
3. バッファが満杯の場合、最古の値を上書きする
   pushOverwrite()を追加する。

## 学習目標

- リングバッファの仕組みを理解する。
- インデックスの循環処理を実装する。
- new[] / delete[]によるメモリ管理を理解する。
- RAIIとコピー禁止設計を理解する。
- O(1)で動作するデータ構造を実装する。
