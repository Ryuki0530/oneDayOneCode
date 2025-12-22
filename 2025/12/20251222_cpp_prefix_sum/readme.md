# 今日の課題：区間和クエリ（Prefix Sum 入門）

## 問題
長さ `N` の整数列 `A` と、`Q` 個のクエリが与えられます。  
各クエリは `l r`（1-indexed）で表され、`A_l + A_{l+1} + ... + A_r` を求めてください。

ただし、`Q` が大きいので **各クエリを愚直に足す（O(N)）** と間に合いません。  
前処理をして **各クエリを O(1)** で答える方法を実装してください。

---

## 入力形式
```
N Q
A1 A2 ... AN
l1 r1
l2 r2
...
lQ rQ
```

---

## 出力形式
各クエリの答えを1行ずつ出力してください。
```
ans1
ans2
...
ansQ
```

---

## 制約
- 1 ≤ N, Q ≤ 200000
- -10^9 ≤ Ai ≤ 10^9
- 1 ≤ li ≤ ri ≤ N
- 64bit整数で収まる（例：Javaなら `long`）

---

## 例

### 入力
```
5 3
2 1 3 5 4
1 3
2 5
4 4
```

### 出力
```
6
13
5
```

---

## ヒント
- `prefix[i] = A1 + ... + Ai` を作る（`prefix[0]=0`）
- 区間和は `prefix[r] - prefix[l-1]`

---

## 追加チャレンジ（余裕があれば）
- 入力が非常に大きいので「高速入力」を意識する  
  - Java: `BufferedInputStream` + 自前パーサ or `FastScanner`
  - C++: `ios::sync_with_stdio(false); cin.tie(nullptr);`
  - Python: `sys.stdin.buffer.read()`