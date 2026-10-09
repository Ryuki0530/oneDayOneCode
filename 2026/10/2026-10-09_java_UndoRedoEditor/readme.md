# UndoRedoEditor

- 使用言語: Java
- 所要時間: 15〜25分
- 学習テーマ: ArrayDequeを使った履歴管理

## 課題
文字列を編集する簡単なエディタを作成してください。
文字の追加に加え、UNDO（元に戻す）とREDO（やり直す）を実装します。

## 実装要件
1. Editorクラスを作り、次のメソッドを実装してください。
   - append(String text): 現在の文字列の末尾にtextを追加する
   - undo(): 直前の編集前の状態へ戻す
   - redo(): undoで取り消した編集をやり直す
   - getText(): 現在の文字列を返す

2. 初期状態は空文字列とします。
   undo用とredo用の履歴は、それぞれArrayDeque<String>で管理してください。

3. appendでは、編集前の文字列をundo用の履歴に保存します。
   新しい編集を行ったら、redo用の履歴をすべて消してください。
   空文字列の追加は何もせず、履歴も変更しないでください。

4. undoとredoでは、移動前の状態を反対側の履歴に保存してください。
   対象の履歴が空の場合は、何もしないでください。

5. Main.javaのmainメソッドで、以下の操作を順に実行してください。
   各操作後の文字列を、角括弧で囲んで表示します。

## 操作例
append("Java")
append(" is")
append(" fun")
undo()
undo()
redo()
append(" great")
redo()

## 期待出力
[Java]
[Java is]
[Java is fun]
[Java is]
[Java]
[Java is]
[Java is great]
[Java is great]

## ヒント
- push(): 履歴の先頭に状態を追加する
- pop(): 履歴の先頭から状態を取り出す
- isEmpty(): 履歴が空か確認する
- clear(): 履歴をすべて消す

## 完了条件
- 操作例の出力が一致すること。
- 初期状態でundoやredoを呼んでも例外が発生しないこと。
- undo後に新しく編集すると、以前の編集をredoできなくなること。