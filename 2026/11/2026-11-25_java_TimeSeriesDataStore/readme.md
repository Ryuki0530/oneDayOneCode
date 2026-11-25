# TimeSeriesDataStore

## 概要

タイムスタンプとセンサーの計測値を管理する
時系列データストアを実装してください。

データは必ずしも時系列順に登録されるとは限りません。

登録されたデータに対して、指定時刻以前の最新データの取得や、
指定期間内の平均値の算出を効率的に行えるようにしてください。

## 実装要件

TimeSeriesDataStore クラスを実装してください。

### 1. add

```java
public void add(long timestamp, double value)
```

指定されたタイムスタンプと計測値を登録する。

同一タイムスタンプのデータが既に存在する場合は上書きする。

### 2. getLatest

```java
public Double getLatest(long timestamp)
```

指定されたタイムスタンプ以下のデータのうち、
最も新しいデータの計測値を返す。

該当するデータが存在しない場合は null を返す。

### 3. getAverage

```java
public Double getAverage(long start, long end)
```

指定期間内の計測値の平均を返す。

開始時刻と終了時刻は検索範囲に含める。

該当するデータが存在しない場合は null を返す。

start が end より大きい場合は
IllegalArgumentException を送出する。

### 4. removeBefore

```java
public int removeBefore(long timestamp)
```

指定されたタイムスタンプより古いデータをすべて削除する。

削除したデータの件数を返す。

指定されたタイムスタンプと一致するデータは削除しない。

### 5. size

```java
public int size()
```

現在登録されているデータの件数を返す。

## 実行例

```java
public class Main {
    public static void main(String[] args) {
        TimeSeriesDataStore store = new TimeSeriesDataStore();

        store.add(1000, 10.0);
        store.add(3000, 30.0);
        store.add(2000, 20.0);
        store.add(4000, 40.0);

        System.out.println(store.getLatest(2500));
        System.out.println(store.getLatest(500));
        System.out.println(store.getAverage(1000, 3000));

        store.add(2000, 25.0);

        System.out.println(store.getLatest(2500));
        System.out.println(store.removeBefore(3000));
        System.out.println(store.size());
        System.out.println(store.getAverage(3000, 4000));
    }
}
```

## 期待される出力

```text
20.0
null
20.0
25.0
2
2
35.0
```

## 制約

- Java標準ライブラリのみ使用可能とする。
- TreeMapを使用すること。
- データ登録のたびにソートを行わないこと。
- getLatestの計算量は O(log N) とすること。
- getAverageでは検索範囲外のデータを走査しないこと。
- データが存在しない場合も正常に処理すること。
