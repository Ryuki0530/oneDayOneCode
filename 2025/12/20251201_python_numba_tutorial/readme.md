# Python 課題：numba でループ処理を加速せよ

Python の for ループは素直だが遅い。  
しかし、`numba` の Just-In-Time (JIT) コンパイルを用いると、  
同じロジックが C 並みに高速化されることがある。

本課題では、以下の 3 つを実装し比較せよ。

---

## 課題内容

### 1. 通常の Python for ループで計算する関数を作成せよ

次のような「0 〜 N-1 の平方和」を求める関数を自分で実装する。

```python
def normal_sum_squares(n):
    s = 0
    for i in range(n):
        s += i * i
    return s
```

---

### 2. numba の `@njit` を付けて高速化した関数を作成せよ

上の関数と同じ処理を行うが、`numba` の `njit` を使って最適化する。

```python
from numba import njit

@njit
def numba_sum_squares(n):
    s = 0
    for i in range(n):
        s += i * i
    return s
```

---

### 3. 実行時間の比較スクリプトを作成せよ

`time.time()` または `time.perf_counter()` を用いて、  
以下の3点を計測し、結果を表示する。

- 通常の関数の実行時間  
- numba JIT “初回呼び出し”の実行時間（コンパイルがあるため重い）  
- numba JIT “2回目以降”の実行時間（本当の速さ）  

N は少なくとも **10,000,000（1e7）** 以上で測定すること。

---

## 出力例（イメージ）

```
Normal loop:          0.85 sec
Numba (first call):   0.42 sec
Numba (second call):  0.02 sec
```

※環境によって結果は変わる。

---

## ヒント

- numba の JIT は、**初回だけコンパイル時間を含む**ため遅い  
- **2回目以降で真価を発揮**する  
- for 文を高速化するのにもっとも向いている  
- Python らしからぬ速度を体感できる

---

## 追加課題（任意）

- 和ではなく、`sin(i)`、`i % 7` などの混合計算でも試す  
- 辞書やリストを扱った場合に JIT が効かない理由をコメントで説明する  
- 型アノテーション付き版（`@njit(float64(int64))`）も試す  
