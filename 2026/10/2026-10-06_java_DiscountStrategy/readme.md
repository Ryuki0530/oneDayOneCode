# DiscountStrategy

使用言語: Java
目安: 20〜25分
提出ファイル: Main.java
外部ライブラリ: 不要

## 目的
Strategyパターンを使い、料金計算と割引ルールを分離してください。

Strategyパターンとは、交換可能な処理を共通のインターフェースで
定義し、利用側のオブジェクトに渡す設計です。
今回は「割引方法」を交換可能にします。

## 課題
通常料金・会員割引・定額割引を切り替えられる、
簡単な会計プログラムを作成してください。

## 実装要件
1. DiscountStrategyインターフェースを定義してください。
   メソッド: int apply(int price)

2. 以下の3クラスでDiscountStrategyを実装してください。
   - NoDiscount: 元の金額を返す。
   - MemberDiscount: 10%割引する。
     計算は price * 90 / 100 とし、端数は切り捨てる。
   - FixedDiscount: コンストラクタで指定した金額を引く。
     計算結果が負なら0を返す。

3. Checkoutクラスを作成してください。
   - コンストラクタでDiscountStrategyを受け取る。
   - setStrategyで割引方法を変更できる。
   - calculate(int price)で、保持しているStrategyのapplyを呼び出す。
   - 割引の種類を判定するif・switch・instanceofは使用しない。

4. mainで同じCheckoutオブジェクトの割引方法を順に変更し、
   以下の例を実行してください。
   金額・定額割引額は0以上とし、不正入力への対応は不要です。
   Main以外の型はpublicにせず、1ファイルにまとめてください。

## 入力例
標準入力は不要です。mainに以下の処理を記述してください。

元の金額: 1200円

1. NoDiscountで計算する。
2. MemberDiscountに変更して計算する。
3. FixedDiscount(300)に変更して計算する。
4. FixedDiscount(1500)に変更して計算する。

## 期待出力
Normal: 1200
Member: 1080
Fixed300: 900
Fixed1500: 0

## 設計のヒント
Checkoutは具体的な割引クラスではなく、
DiscountStrategy型のフィールドを持ちます。

「どの割引を使うか」はmainで決め、
「その割引で計算すること」はCheckoutに任せてください。

## 完了条件
- 期待出力と一致する。
- Checkout内に具体的な割引計算や種類判定がない。
- 新しい割引クラスを追加するとき、
  Checkoutのコードを変更せずに利用できる。