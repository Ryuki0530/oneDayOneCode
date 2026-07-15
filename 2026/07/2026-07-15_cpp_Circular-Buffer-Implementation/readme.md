# OneDayOneCode

## タイトル
Circular Buffer Implementation

## 問題

固定長リングバッファを実装してください。

以下の構造体を用意します。

```c
typedef struct {
    int data[5];
    int head;
    int tail;
    int size;
} CircularBuffer;
```

以下の関数を実装してください。

```c
void init_buffer(CircularBuffer *buf);
int push(CircularBuffer *buf, int value);
int pop(CircularBuffer *buf, int *value);
```

### 動作仕様

- init_buffer
    - バッファを初期化する。

- push
    - データを末尾へ追加する。
    - 成功なら0を返す。
    - バッファ満杯なら-1を返す。

- pop
    - 先頭データを取り出す。
    - 成功なら0を返す。
    - 空なら-1を返す。

head・tailは配列終端まで行ったら先頭へ戻ること。

---

## テスト例

```
push 10
push 20
push 30
pop -> 10

push 40
push 50
push 60

pop -> 20
pop -> 30
pop -> 40
pop -> 50
pop -> 60
```

---

## 発展課題（任意）

現在のバッファ内容を

```
[20][30][40]
```

のように表示する

```c
void print_buffer(const CircularBuffer *buf);
```

を実装してください。

---

## 学べること

- 配列管理
- インデックス計算
- `%` を用いた循環処理
- FIFOデータ構造
- 組込み・RTOS・通信ドライバで頻出の実装