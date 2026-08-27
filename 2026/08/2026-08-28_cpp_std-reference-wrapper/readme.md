# 今日のC++課題：`std::reference_wrapper` でオブジェクトを参照管理する

## 概要

複数の `Sensor` オブジェクトを管理する `SensorManager` を作成してください。

ただし、`SensorManager` は `Sensor` をコピーして所有するのではなく、
`main()` 側で生成された `Sensor` を「参照」として保持します。

C++では次のようなコードは書けません。

```cpp
std::vector<Sensor&> sensors;
```

そこで今回は、

```cpp
std::reference_wrapper<Sensor>
```

を使用します。

---

## 使用するもの

- `std::vector`
- `std::reference_wrapper`
- `std::ref`

必要なヘッダ：

```cpp
#include <functional>
#include <iostream>
#include <string>
#include <vector>
```

---

## Sensor クラス

以下のメンバを持たせてください。

```cpp
std::string name_;
double value_;
```

以下のメソッドを実装してください。

```cpp
Sensor(std::string name, double value);

void setValue(double value);

double getValue() const;

void print() const;
```

`print()` の出力例：

```text
temperature: 25.3
```

---

## SensorManager クラス

内部に以下を持たせてください。

```cpp
std::vector<std::reference_wrapper<Sensor>> sensors_;
```

次のメソッドを実装してください。

### add()

```cpp
void add(Sensor& sensor);
```

渡された `Sensor` の参照を `sensors_` に追加します。

ヒント：

```cpp
std::ref(sensor)
```

を使用できます。

### printAll()

```cpp
void printAll() const;
```

登録されているすべての `Sensor` について `print()` を呼び出してください。

---

## main()

次の3つの `Sensor` を作成してください。

```cpp
Sensor temperature("temperature", 25.3);
Sensor pressure("pressure", 101.3);
Sensor humidity("humidity", 45.0);
```

`SensorManager` に3つとも登録してください。

その後、

```cpp
temperature.setValue(30.0);
```

としてから `manager.printAll()` を呼び出してください。

---

## 期待する出力

```text
temperature: 30
pressure: 101.3
humidity: 45
```

---

## ポイント

`SensorManager` 内に `Sensor` のコピーを保存しているわけではありません。

そのため、

```cpp
temperature.setValue(30.0);
```

による変更が、`SensorManager` からアクセスした場合にも反映されます。

今回確認したいのは、

> オブジェクトを所有せず、既存オブジェクトへの参照だけをコンテナに保持する

という設計です。

---

## コンパイル

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
```