# Task: Sensor Interface Practice

## 目的

C++で「インターフェイス」を使い、異なる種類のセンサーを同じ扱いで処理できるようにする。

C++では、Javaのような `interface` キーワードはない。
代わりに、純粋仮想関数を持つ抽象クラスを使ってインターフェイスを表現する。

## 問題

以下のインターフェイスを作成せよ。

```cpp
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual std::string name() const = 0;
    virtual double readValue() const = 0;
};
```

この `ISensor` を実装する以下の3つのクラスを作成せよ。

- TemperatureSensor
- SpeedSensor
- BatterySensor

それぞれ以下の値を返すものとする。

| クラス | name() | readValue() |
|---|---|---|
| TemperatureSensor | "temperature" | 36.5 |
| SpeedSensor | "speed" | 72.0 |
| BatterySensor | "battery" | 84.0 |

## 入力

なし

## 出力

`std::vector<std::unique_ptr<ISensor>>` に3種類のセンサーを格納し、順番に名前と値を出力せよ。

## 出力例

```text
temperature: 36.5
speed: 72
battery: 84
```

## 条件

- `ISensor` は抽象クラスにすること
- `readValue()` と `name()` は純粋仮想関数にすること
- センサーの処理は、具象クラス名ではなく `ISensor` 型を通して呼び出すこと
- `new` / `delete` を直接使わず、`std::unique_ptr` を使うこと

## 発展

時間があれば、以下を追加せよ。

- `printSensor(const ISensor& sensor)` 関数を作る
- `WarningSensor` を追加し、値が一定以上なら `"warning"` と表示する