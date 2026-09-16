# ImplementMemmove

## 問題

標準ライブラリの `memmove` を使わずに、同等の動作をする関数を実装してください。

```c
void *my_memmove(void *dest, const void *src, size_t n);
```

`src` から `n` バイトを `dest` にコピーします。

ただし、`src` と `dest` のメモリ領域が重なっている場合でも、
コピー結果が壊れないようにしてください。

## 条件

- `memmove()` は使用禁止
- `memcpy()` も使用禁止
- バイト単位でコピーすること
- コピー元とコピー先の領域が重なる場合を考慮すること
- 戻り値は `dest`

## 動作例

次の配列に対して、

```c
char text[] = "ABCDE";

my_memmove(text + 1, text, 4);
```

実行後、

```text
AABCD
```

となること。

## ヒント

コピー先がコピー元より後ろにあり、
領域が重なっている場合は、
後ろのバイトからコピーすると安全です。