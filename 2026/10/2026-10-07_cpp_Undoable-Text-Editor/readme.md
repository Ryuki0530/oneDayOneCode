# Undoable Text Editor

使用言語: C++17
目安: 15〜20分

## 目的
std::dequeを使い、最大3回まで操作を取り消せる簡易テキストエディタを実装してください。

## 実装要件
1. 編集中の文字列は空文字列から開始します。
2. 次のコマンドを処理してください。
   - APPEND word: 現在の文字列の末尾にwordを追加します。
   - UNDO: 直前の編集前の状態へ戻します。
   - PRINT: 現在の文字列を表示します。
3. APPENDを実行する直前の文字列をstd::deque<std::string>に保存してください。
   - 履歴は最大3件です。
   - 4件になった場合は最も古い履歴を削除します。
4. UNDOでは最新の履歴を取り出し、現在の文字列に設定してください。
   - 履歴が空なら何もしません。
   - UNDO自体は履歴へ保存しません。
5. PRINT時に文字列が空なら「(empty)」と表示してください。

入力はコード内に固定しても構いません。
wordは空白を含まない、1文字以上の文字列とします。
APPENDでは空白や改行を自動挿入しません。

## 入力例
APPEND A
APPEND B
APPEND C
APPEND D
PRINT
UNDO
PRINT
UNDO
PRINT
UNDO
PRINT
UNDO
PRINT

## 期待出力
ABCD
ABC
AB
A
A

## ヒント
- push_back(): 最新の履歴を追加
- pop_front(): 最も古い履歴を削除
- back(): 最新の履歴を参照
- pop_back(): 最新の履歴を削除
- empty(): 履歴が空か確認

## 完了条件
- 入力例と同じ出力になること。
- 最初にUNDOしても問題なく動作すること。
- 初期状態でPRINTすると「(empty)」と表示されること。