# Daily Python Challenge: Simple Command Filter

## 問題

標準入力から `N` 個のコマンド文字列が与えられる。

各コマンドは次の形式である。

- `ADD 名前`
- `DEL 名前`
- `CHECK 名前`

空の集合を用意し、次の処理を行え。

- `ADD 名前`  
  名前を集合に追加する

- `DEL 名前`  
  名前が集合に存在すれば削除する  
  存在しない場合は何もしない

- `CHECK 名前`  
  名前が集合に存在すれば `YES`、存在しなければ `NO` を出力する

## 入力形式

```text
N
command_1
command_2
...
command_N
```

## 出力形式

`CHECK` コマンドごとに判定結果を1行で出力する。

## 制約

- 1 <= N <= 100
- 名前は英小文字のみ
- 名前の長さは 1 以上 20 以下

## 入力例

```text
6
ADD apple
CHECK apple
CHECK banana
ADD banana
DEL apple
CHECK apple
```

## 出力例

```text
YES
NO
NO
```

## 実装条件

- Pythonで実装すること
- `set` を使うこと
- ファイル入出力は不要