# 今日の課題（Java）: 「TTL付きメモ + スレッドセーフ」

## 概要
**一定時間だけ有効なメモ（キー→文字列）** を管理する小さなライブラリを書いてください。  
保存した値は **TTL（生存時間: Time To Live）** を過ぎると自動的に「消えた扱い」になります。

さらに今回は、`put/get/cleanup` が **複数スレッドから同時に呼ばれても壊れない**（スレッドセーフ）ようにします。

---

## 仕様

### コマンド入力
標準入力からコマンドを読み、標準出力に結果を出してください。

最初の1行:
- `Q`（コマンド数）

続くQ行（いずれか）:
- `PUT key ttl_seconds value`
  - `key` は空白なし文字列
  - `ttl_seconds` は 0以上の整数
  - `value` は空白なし文字列
  - そのキーの値と期限を更新する（上書き）
- `GET key`
  - 有効なら `value` を出力
  - 無効（存在しない or 期限切れ）なら `-1` を出力
- `SLEEP ms`
  - `ms` ミリ秒スリープ（テスト用）。出力なし
- `CLEANUP`
  - 期限切れキーを物理削除する（出力なし）

### 時刻の扱い
- 「今」は `System.currentTimeMillis()` を使う
- `ttl_seconds` は秒。期限 = `now + ttl_seconds*1000`
- `ttl_seconds=0` は **即期限切れ**（PUTしてもGETでは `-1`）

---

## 要件（重要）
- `TTLStore` クラスを作る
  - `void put(String key, String value, long ttlSeconds)`
  - `String get(String key)`（無効なら `null`）
  - `void cleanup()`
- **スレッドセーフ**にすること  
  例: `ConcurrentHashMap` + 期限情報の保持 / `ReentrantLock` / `synchronized` など、どれでもOK  
  ただし「たまたま動く」じゃなく、設計として破綻しない形にする。

---

## 入出力例

入力:
```
8
PUT a 1 hello
GET a
SLEEP 1100
GET a
PUT a 2 world
GET a
CLEANUP
GET a
```


出力:
```
hello
-1
world
world
```

## 関連モジュールのヒント（使い方ざっくり）
- `ConcurrentHashMap<K,V>`: 複数スレッドから安全に `get/put/remove` できるMap
- `System.currentTimeMillis()`: 現在時刻（ミリ秒）
- `Thread.sleep(ms)`: 指定ミリ秒停止（例外処理必要）
- 期限情報は `record Entry(String value, long expiresAtMillis)` みたいにまとめると楽

---

## ゴール
**入力コマンドを処理して、GETの結果だけを出力**できればクリアです。

