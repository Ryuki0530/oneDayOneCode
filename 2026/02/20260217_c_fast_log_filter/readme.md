# Today’s C Challenge: **Fast Log Filter (Substring + Count)**

## 問題
N行のログと、Q個のクエリが与えられます。各クエリは文字列 `pattern` です。  
各クエリごとに「ログのうち `pattern` を**部分文字列として含む**行の数」を出力してください。

ただし、`N` と `Q` が大きいので **愚直な全探索（全行に対して毎回 strstr）だとTLE** します。  
Cで“それなりに速い”実装を目指してください。

---

## 入力形式
- 1行目: `N Q`
- 続くN行: ログ文字列（ASCII、空白を含まない、長さ <= 200）
- 続くQ行: クエリ文字列 `pattern`（ASCII、空白を含まない、長さ <= 50）

## 出力形式
- Q行出力。i行目は i番目のクエリの答え。

---

## 制約
- 1 <= N <= 20000
- 1 <= Q <= 20000
- ログ長 <= 200
- クエリ長 <= 50

---

## 要求
- C（標準ライブラリOK）で実装
- 入出力は高速化する（`fgets` / バッファリング推奨）
- **工夫**：たとえば
  - クエリをまとめて処理する
  - 文字列探索を高速化する（KMP / Z / rolling hash など）
  - ログ側の前処理（n-gramのインデックス等）

---

## サンプル
### 入力
5 4
ERROR_disk_full
INFO_start
WARN_disk_slow
ERROR_network
INFO_disk_check
disk
ERROR
INFO
slow

### 出力
3
2
2
1

---

## ヒント（手短）
- “短いpatternが大量”なので、ログから **n-gram（例: 3-gram）→行ID集合** の辞書を作り、
  patternの3-gramの共通部分から候補行を絞って最後に `strstr` で確定、が現実的。
- pattern長<3 のときは別処理（1文字・2文字のインデックス or 全探索でもOKにする工夫）。

---

## ゴール
TLEしないこと（目安：2秒級想定）。