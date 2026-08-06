```markdown
# 今日のC++課題「簡易タスクスケジューラ」

## 目的

- `std::function`
- ラムダ式
- `std::vector`
- クラス設計

を練習する。

---

## 問題

`TaskScheduler`クラスを作成してください。

### メンバ関数

```cpp
void addTask(const std::string& name,
             std::function<void()> task);
```

タスクを登録する。

```cpp
void run();
```

登録された順番に

```text
Running: タスク名
```

を表示してからタスクを実行する。

---

## 実行例

```text
Running: Hello
Hello World!

Running: Count
1
2
3

Running: Finish
Done!
```

---

## 条件

- タスクは `std::vector` に保存する。
- `TaskScheduler` の外ではラムダ式を使って登録する。
- `run()` を2回呼んだ場合、同じタスクが再度実行される仕様でよい。

---

## 発展課題（余裕があれば）

以下の機能を追加してみる。

```cpp
void removeTask(const std::string& name);
```

指定した名前のタスクを削除する。

さらに、

```cpp
void clear();
```

ですべてのタスクを削除できるようにする。
```