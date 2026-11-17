# LRUCache

## 問題概要

LRU（Least Recently Used）方式でデータを管理する
キャッシュクラス `LRUCache` を実装してください。

LRU方式では、キャッシュ容量を超えた場合、
最も長い間使用されていないデータを削除します。

## 要件

以下のメソッドを持つクラスを実装してください。

### コンストラクタ
`LRUCache(int capacity)`

- キャッシュの最大容量を指定する。
- capacityは1以上とする。

### put(int key, int value)
- 指定したキーと値をキャッシュへ保存する。
- 既存キーの場合は値を更新する。
- 保存または更新したキーは最新使用扱いとする。
- 容量を超えた場合、最も古く使用されたキーを削除する。

### get(int key)
- 指定キーに対応する値を返す。
- キーが存在しない場合は -1 を返す。
- 取得に成功したキーは最新使用扱いとする。

### size()
- 現在保存されている要素数を返す。

## 実行例

LRUCache cache = new LRUCache(2);

cache.put(1, 100);
cache.put(2, 200);

System.out.println(cache.get(1)); // 100

cache.put(3, 300);
// キー2が削除される

System.out.println(cache.get(2)); // -1
System.out.println(cache.get(3)); // 300

cache.put(1, 150);

System.out.println(cache.get(1)); // 150
System.out.println(cache.size()); // 2

## 制約

- キーと値はint型とする。
- キャッシュ容量は1以上。
- Java標準ライブラリを使用してよい。
- getとputは平均O(1)で動作すること。

## 発展課題（任意）

1. LinkedHashMapを使わず、
   HashMapと双方向連結リストでLRUを実装する。

2. remove(int key)を追加し、
   指定キーをキャッシュから削除できるようにする。