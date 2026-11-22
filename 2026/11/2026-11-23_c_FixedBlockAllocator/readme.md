
# FixedBlockAllocator

## 問題概要

C言語を使用して、固定サイズのメモリブロックを管理する
簡易メモリアロケータを実装してください。

組込みシステムでは、malloc()やfree()による
動的メモリ確保を避けることがあります。

今回は、あらかじめ確保したメモリ領域を使用し、
固定長ブロックの割り当てと解放を行う仕組みを実装します。

## 仕様

以下の定数を使用してください。

```c
#define BLOCK_SIZE 32
#define BLOCK_COUNT 8
```

### 1. メモリプールの定義

以下の構造体を使用してください。

```c
typedef struct {
    unsigned char memory[BLOCK_COUNT][BLOCK_SIZE];
    int free_queue[BLOCK_COUNT];
    int head;
    int tail;
    int free_count;
    unsigned char allocated[BLOCK_COUNT];
} MemoryPool;
```

- memory：実際のメモリ領域
- free_queue：空きブロック番号を管理するリングバッファ
- head：次に取り出す位置
- tail：次に追加する位置
- free_count：空きブロック数
- allocated：各ブロックの割り当て状態（0：空き、1：使用中）

### 2. 初期化

```c
void pool_init(MemoryPool *pool);
```

- 全ブロックを未使用状態にする
- free_queueに0〜7を格納する
- headとtailを0にする
- free_countをBLOCK_COUNTにする

### 3. メモリ割り当て

```c
void *pool_alloc(MemoryPool *pool);
```

- 空きブロックを1つ取り出す
- 取り出したブロックを使用中にする
- 対応するメモリ領域の先頭アドレスを返す
- 空きがない場合はNULLを返す

リングバッファの更新には以下を使用します。

```c
head = (head + 1) % BLOCK_COUNT;
```

### 4. メモリ解放

```c
int pool_free(MemoryPool *pool, void *ptr);
```

- ptrがメモリプール内のブロック先頭アドレスであるか確認する
- 対応するブロックが使用中なら解放する
- 解放したブロック番号をfree_queueの末尾に追加する
- 正常終了時は0を返す
- 不正なポインタや二重解放の場合は-1を返す

注意：
ブロックの途中を指すポインタは無効とします。

### 5. 空きブロック数取得

```c
int pool_available(const MemoryPool *pool);
```

現在の空きブロック数を返してください。

## 実行例

```text
Available: 8
Allocated: 8 blocks
Available: 0
Allocation failed
Available after free: 1
Reallocated successfully
Available: 0
Double free detected
```

## 制約

- malloc(), calloc(), realloc(), free()は禁止
- グローバル変数は禁止
- メモリ管理にはリングバッファを使用する
- 不正なポインタによる未定義動作を避ける
- C11規格でコンパイル可能にする
