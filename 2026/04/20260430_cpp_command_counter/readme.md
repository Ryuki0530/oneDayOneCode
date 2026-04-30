# Today's C++ Challenge: Command Counter

## 問題

複数行の操作ログが与えられます。
各行には、次のいずれかのコマンドが書かれています。

- ADD
- DELETE
- UPDATE
- READ

それぞれのコマンドが何回出現したかを数えてください。

## 入力

```text
N
command_1
command_2
...
command_N
```

## 条件

- 1 <= N <= 1000
- command_i は ADD, DELETE, UPDATE, READ のいずれか

## 出力

次の順番で、各コマンドの出現回数を出力してください。

```text
ADD count
DELETE count
UPDATE count
READ count
```

## 入力例

```text
7
ADD
READ
ADD
DELETE
UPDATE
READ
ADD
```

## 出力例

```text
ADD 3
DELETE 1
UPDATE 1
READ 2
```

## 実装条件

- C++で実装すること
- map または unordered_map を使うこと
- 入力は標準入力から受け取ること