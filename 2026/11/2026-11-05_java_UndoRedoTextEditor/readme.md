# Undo・Redo付きの文字列エディタ

使用言語: Java
目安: 15〜25分

## 目的
文字列の編集履歴を管理し、Undo（取り消し）とRedo（やり直し）を実装してください。

## 実装要件
1. 初期状態の文字列は空文字列とします。
2. 次のコマンドを順番に処理してください。
   - APPEND text: 文字列の末尾にtextを追加する
   - UNDO: 直前の編集を取り消す
   - REDO: 取り消した編集をやり直す
3. UNDO・REDOできる履歴がない場合は、何もしません。
4. APPENDを実行すると、それまでのREDO用の履歴をすべて破棄します。
5. 各コマンドの処理後に、現在の文字列を角括弧で囲んで表示してください。

## 入力条件
- コマンドはmain内のString配列に直接記述して構いません。
- APPENDのtextは1文字以上で、空白を含まないものとします。
- 不正なコマンドは与えられません。

## 入力例
APPEND Hello
APPEND World
UNDO
REDO
UNDO
APPEND Java
REDO

## 期待出力
[Hello]
[HelloWorld]
[Hello]
[HelloWorld]
[Hello]
[HelloJava]
[HelloJava]

## ヒント
- Deque<String>とArrayDeque<String>を使えます。
- 編集前の文字列を保存すると、状態を戻しやすくなります。
- push、pop、isEmpty、clearの使い方を確認してください。

## 完了条件
- 入力例どおりに表示されること。
- 初期状態でUNDO・REDOを実行しても例外が出ないこと。
- UNDOを繰り返すと空文字列まで戻せること。
- UNDO後にAPPENDすると、以前の編集をREDOできなくなること。