# DependencyResolver

## 問題概要

複数のタスクと、それらの依存関係を管理する
DependencyResolver クラスを実装してください。

あるタスクが別のタスクに依存している場合、
依存先のタスクを先に実行する必要があります。

例えば、以下の依存関係があるとします。

A → B → C

これは、Aを実行するにはBが完了している必要があり、
Bを実行するにはCが完了している必要があることを意味します。

したがって、実行順序は C, B, A となります。

## 実装するクラス

class DependencyResolver:

    def add_task(self, name: str) -> None:
        ...

    def add_dependency(self, task: str, depends_on: str) -> None:
        ...

    def resolve(self) -> list[str]:
        ...

    def has_cycle(self) -> bool:
        ...

## 各メソッドの仕様

### 1. add_task(name)

タスクを登録します。

- 同名タスクが存在する場合は何もしません。
- タスク名は文字列です。

### 2. add_dependency(task, depends_on)

タスク間の依存関係を追加します。

- task は depends_on の完了後に実行できます。
- 未登録のタスクが指定された場合は自動登録します。
- 同じ依存関係を複数回追加しても重複しません。
- 自分自身への依存も登録可能ですが、循環依存として扱います。

### 3. resolve()

依存関係を満たす実行順序をリストで返します。

- すべてのタスクを1回ずつ含めます。
- 依存先のタスクは必ず先に配置します。
- 実行可能なタスクが複数ある場合、
  タスク名の辞書順で優先します。
- 循環依存がある場合は ValueError を送出します。
- タスクが存在しない場合は空リストを返します。

### 4. has_cycle()

循環依存が存在するか判定します。

- 循環依存が存在する場合は True
- 存在しない場合は False
- 自己依存も循環依存として扱います。

## 実行例

resolver = DependencyResolver()

resolver.add_dependency("Build", "Compile")
resolver.add_dependency("Compile", "Generate")
resolver.add_dependency("Test", "Build")
resolver.add_task("Lint")

print(resolver.resolve())
# ['Generate', 'Compile', 'Build', 'Lint', 'Test']

print(resolver.has_cycle())
# False

resolver.add_dependency("Generate", "Test")

print(resolver.has_cycle())
# True

try:
    print(resolver.resolve())
except ValueError:
    print("Circular dependency detected")

# Circular dependency detected

## 制約

- タスク数は最大10,000件
- 依存関係は最大50,000件
- 標準ライブラリのみ使用可能
- resolve() は繰り返し呼び出しても同じ結果を返すこと
- resolve() によって登録済みの依存関係を変更しないこと

## 発展課題（任意）

1. remove_task(name) を実装し、
   タスクと関連する依存関係をすべて削除する。

2. get_dependencies(name) を実装し、
   指定タスクの直接的な依存先を辞書順で返す。

3. resolve() の計算量を考察し、
   タスク数 V、依存関係数 E を用いて表現する。