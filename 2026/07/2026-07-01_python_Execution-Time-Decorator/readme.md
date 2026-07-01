# OneDayOneCode

# 2026-07-01
# Python
# Title: Execution-Time-Decorator

## 問題

関数の実行時間を表示するデコレータ `measure_time` を実装してください。

### 要件

- `time.perf_counter()` を使用すること
- 関数の実行前後で時間を測定すること
- 実行時間をミリ秒(ms)で表示すること
- 元の関数の戻り値はそのまま返すこと

### 実装対象

```python
@measure_time
def slow_sum(n):
    total = 0
    for i in range(n):
        total += i
    return total
```

### 実行例

```
Execution time: 12.34 ms
Result = 499999500000
```

## 余裕があれば

デコレータを改良し、

```
slow_sum took 12.34 ms
```

のように関数名も表示してください。

## 学べること

- デコレータの基本
- `*args`, `**kwargs`
- `time.perf_counter()`
- 関数をラップする考え方