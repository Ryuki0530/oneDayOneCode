# 今日のJava課題：**TTL付きLRUキャッシュ（スレッド安全）**

## 概要
「最大容量つきのキャッシュ」に **TTL（有効期限）** を足して、さらに **LRU（最近使われたものを残す）** で追い出し制御するクラスを Java で実装してください。  
加えて、複数スレッドから `get/put` が呼ばれても破綻しないように **スレッド安全** にします。

---

## 仕様

### 1) キャッシュの基本
- `TTLCache<K, V>` を作る
- コンストラクタで以下を受け取る
  - `capacity`（最大保持数。超えたらLRUで削除）
  - `defaultTtlMillis`（putでTTLを省略した時のTTL）

### 2) put / get
- `put(K key, V value)`  
  - `defaultTtlMillis` で保存
- `put(K key, V value, long ttlMillis)`  
  - `ttlMillis <= 0` のときは「即期限切れ扱い」で保存しない（または保存して即消えるでも可だが、挙動を明記）
- `V get(K key)`  
  - 存在しない or 期限切れなら `null`
  - 期限切れなら内部から削除する
  - 取得できた場合はLRU的に「最近使った」扱いに更新する

### 3) サイズとメンテ
- `int size()`  
  - 期限切れを除いた「実質サイズ」を返す（呼び出し時に期限切れ掃除してOK）
- `int purgeExpired()`  
  - 期限切れを掃除して、削除した件数を返す

### 4) スレッド安全
- `get/put/purgeExpired/size` の同時実行で
  - 例外が飛ばない
  - 内部状態が壊れない
  - LRU順序が変なことにならない  
  を満たすこと

---

## 実装上のヒント（推奨構成）
- LRUは `LinkedHashMap` の `accessOrder=true` が便利
- TTLは `Entry` に `expiresAtMillis` を持たせる
- スレッド安全はまずは **1つのロック（synchronized か ReentrantLock）** でOK  
  （ReadWriteLockで最適化したくなったらボーナス）

---

## 入出力（動作テスト用の簡易CLI）
標準入力で以下コマンドを処理する `Main` を用意してください。

- `PUT key value [ttlMillis]`
- `GET key`
- `SIZE`
- `PURGE`

出力ルール：
- `GET` は値があれば値、なければ `NULL`
- `SIZE` は数値
- `PURGE` は削除件数

### サンプル
入力:
```
PUT a hello 200
GET a
PUT b world 1000
SIZE    
```
出力例:
```

hello
2
```

（※時間経過が絡むので、サンプルは参考程度）

---

## 追加条件（ボーナス）
1. `put` で同じキーが来た場合、値と期限を更新し、LRU上も「最近」になることを確認せよ
2. `purgeExpired()` を定期実行する用途を想定し、`O(n)` でも良いが無駄に重くしない
3. `System.currentTimeMillis()` の依存を減らすために `LongSupplier nowMillis` を注入できる設計にしても良い（テストが楽になる）

---

## 提出物
- `TTLCache.java`
- `Main.java`（CLIテスト）
- （任意）`README.md`：設計の意図、スレッド安全の方針、TTLの扱い

---

## ねらい（何が鍛えられるか）
- `LinkedHashMap` による LRU 実装
- 有効期限（TTL）という「時間の状態」をデータ構造に混ぜる設計
- 競合が起きるコードを壊さずにまとめるロック設計
