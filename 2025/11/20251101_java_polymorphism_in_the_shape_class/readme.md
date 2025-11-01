# 🧩 今日の課題：図形クラスの継承と多態性

## 問題文

次の仕様に従って、いくつかの図形クラスを作成しなさい。  
すべてのクラスは、標準入力から与えられたデータをもとに面積を計算し、標準出力に結果を出すプログラムの一部として動作します。

---

## 仕様

1. **抽象クラス `Shape` を定義**し、次のメソッドを持たせること。  
```java
   abstract class Shape {
       abstract double getArea();
   }
```

2. **Shape を継承する次の3つのクラスを作ること。**

Circle … 半径 r をもつ円

Rectangle … 幅 w、高さ h をもつ長方形

Triangle … 底辺 b、高さ h をもつ三角形

各クラスはコンストラクタで値を受け取り、getArea() メソッドで面積を返すこと。

メインクラス Main では、以下の形式の入力を受け取る。

```
N
type a b
type a b
...


N は図形の個数（整数）

type は "CIRCLE", "RECT", "TRI" のいずれか

a, b はその図形のパラメータ（円は a=r のみで b は無視してよい）

各図形の面積を計算し、次の形式で出力せよ。
```
CIRCLE: 12.56
RECT: 20.00
TRI: 7.50
...
小数点以下2桁まで表示すること。

入力例
```
3
CIRCLE 2 0
RECT 4 5
TRI 3 5
出力例
```
CIRCLE: 12.57
RECT: 20.00
TRI: 7.50
3
CIRCLE 2 0
RECT 4 5
TRI 3 5
出力例
```
CIRCLE: 12.57
RECT: 20.00
TRI: 7.50
```
ヒント
Math.PI を使うと円周率が得られる。

System.out.printf("%.2f", value); で小数点以下2桁のフォーマットが可能。

クラスの多態性（Shape 型の配列やリストに Circle, Rectangle, Triangle を格納）を意識すると良い。

発展課題（余裕があれば）
Shape に「名前」を持たせて出力を自動化してみる。

Comparator を使って面積の大きい順にソートして出力してみる。