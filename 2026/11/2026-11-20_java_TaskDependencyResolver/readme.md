# TaskDependencyResolver

## 概要

複数のタスクと、それぞれのタスクの依存関係を管理し、
依存関係を満たす実行順序を生成するプログラムを作成してください。

例えば、ソフトウェアの起動時に以下の依存関係があるとします。

- NetworkはDriverに依存する
- ApplicationはNetworkに依存する
- ApplicationはLoggerに依存する

この場合、依存先のタスクが先に実行されるように、
実行順序を決定する必要があります。

また、循環依存が存在する場合は、
実行順序を生成できないことを検出してください。

## 実装要件

TaskDependencyResolverクラスを作成し、
以下のメソッドを実装してください。

### 1. addTask

```java
public void addTask(String task)
```

- タスクを登録する。
- 登録済みのタスクが指定された場合は何もしない。

### 2. addDependency

```java
public void addDependency(String task, String dependency)
```

- taskがdependencyに依存することを登録する。
- dependencyはtaskより先に実行される必要がある。
- 未登録のタスクが指定された場合は自動的に登録する。
- 同一の依存関係を重複して登録しない。

### 3. resolve

```java
public List<String> resolve()
```

- 依存関係を満たすタスクの実行順序を返す。
- 実行可能なタスクが複数存在する場合は、
  辞書順で最も小さいタスクを優先する。
- 循環依存が存在する場合はIllegalStateExceptionを送出する。
- タスクが存在しない場合は空のリストを返す。
- このメソッドを実行しても登録済みの情報を変更しない。

## 実行例

```java
public class Main {
    public static void main(String[] args) {
        TaskDependencyResolver resolver =
            new TaskDependencyResolver();

        resolver.addDependency("Network", "Driver");
        resolver.addDependency("Application", "Network");
        resolver.addDependency("Application", "Logger");

        resolver.addTask("Monitor");

        System.out.println(resolver.resolve());
    }
}
```

### 期待される出力

```
[Driver, Logger, Monitor, Network, Application]
```

## 循環依存のテスト

```java
TaskDependencyResolver resolver =
    new TaskDependencyResolver();

resolver.addDependency("A", "B");
resolver.addDependency("B", "C");
resolver.addDependency("C", "A");

System.out.println(resolver.resolve());
```

IllegalStateExceptionが送出されること。

## 制約

- Java標準ライブラリのみ使用可能。
- タスク名は空でない文字列とする。
- タスク名にnullは渡されないものとする。
- タスク数は最大10,000件とする。
- 依存関係は最大100,000件とする。
- 依存関係の全件走査を繰り返す実装は避けること。

