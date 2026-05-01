# 今日の課題：Command Alias Expander

## 概要

簡単なコマンドエイリアス展開プログラムを作成してください。

ユーザーは、短い別名である「エイリアス」を登録できます。
その後、入力されたコマンド列に対して、先頭の単語が登録済みエイリアスなら、対応する文字列に置き換えて出力します。

## 入力形式

N
alias1 command1
alias2 command2
...
aliasN commandN
Q
query1
query2
...
queryQ

## 仕様

- N はエイリアス登録数
- alias は空白を含まない文字列
- command は空白を含む可能性がある文字列
- query は実行したいコマンド文字列
- query の先頭単語が alias と一致した場合、その部分だけを command に置き換える
- 一致しない場合は query をそのまま出力する
- query の先頭単語以外は判定しない

## 制約

- 1 <= N <= 100
- 1 <= Q <= 100
- 各行の長さは 200 文字以下

## 入力例

3
gs git status
gc git commit
ll ls -la
5
gs
gc -m "first commit"
ll src
git status
gc --amend

## 出力例

git status
git commit -m "first commit"
ls -la src
git status
git commit --amend

## 補足

例えば、

gc -m "first commit"

という query の先頭単語は gc です。
gc は git commit に登録されているので、

git commit -m "first commit"

と出力します。

## 実装条件

- Javaで実装すること
- Map<String, String> を使用すること
- 標準入力から読み込み、標準出力へ出力すること