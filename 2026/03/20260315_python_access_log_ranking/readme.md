# 今日の課題: Access Log Ranking

## 問題概要
アクセスログが 1 行ずつ標準入力から与えられます。  
各行は次の形式です。

<user_id> <page>

例:
alice /home
bob /about
alice /home
alice /products

このログを集計し、以下を出力してください。

## やること
1. 総アクセス数を出力する
2. ユーザーごとのアクセス数を集計する
3. ページごとのアクセス数を集計する
4. 最もアクセス数の多いユーザーを出力する
5. 最もアクセス数の多いページを出力する

## 入力
標準入力で 0 行以上のログが与えられる。  
各行は以下の形式とする。

<user_id> <page>

- `user_id` は英数字からなる文字列
- `page` は `/` から始まる文字列
- 入力の終わりまで読み込むこと

## 出力
以下の形式で出力してください。

TOTAL <総アクセス数>
TOP_USER <ユーザーID> <アクセス数>
TOP_PAGE <ページ> <アクセス数>

USERS
<user_id1> <count1>
<user_id2> <count2>
...

PAGES
<page1> <count1>
<page2> <count2>
...

## 並び順
- `USERS` ではアクセス数の降順
- 同数の場合は `user_id` の辞書順昇順
- `PAGES` でもアクセス数の降順
- 同数の場合は `page` の辞書順昇順

## 特記事項
- 入力が 0 件の場合は以下のように出力すること

TOTAL 0
TOP_USER NONE 0
TOP_PAGE NONE 0

USERS
PAGES

## 入力例1
alice /home
bob /about
alice /home
alice /products
bob /home

## 出力例1
TOTAL 5
TOP_USER alice 3
TOP_PAGE /home 3

USERS
alice 3
bob 2

PAGES
/home 3
/about 1
/products 1

## 入力例2
<入力なし>

## 出力例2
TOTAL 0
TOP_USER NONE 0
TOP_PAGE NONE 0

USERS
PAGES

## ヒント
- `dict` で数える
- `sys.stdin` で複数行入力を読む
- 並び替えは `sorted(..., key=...)` を使う

## 目標
まずは正しく集計すること。  
余裕があれば関数に分けて、読みやすく整理してください。