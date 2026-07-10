# OneDayOneCode

## タイトル
Variant Sensor Logger

## 問題

3種類のセンサデータを `std::variant` を使って表現してください。

扱うデータは以下の3種類です。

- int       : 車速[km/h]
- double    : バッテリー電圧[V]
- std::string : ステータスメッセージ

以下の順番でデータを `std::vector<std::variant<...>>` に格納してください。

80
13.8
"READY"
100
12.5
"ERROR"

`std::visit` を用いて取り出し、型ごとに以下の形式で表示してください。

Speed : 80 km/h
Voltage : 13.8 V
Status : READY
Speed : 100 km/h
Voltage : 12.5 V
Status : ERROR

## 条件

- C++17以上
- dynamic_castは禁止
- typeidは禁止
- std::variant
- std::visit
- if constexpr と std::is_same_v を利用すること

## 学べること

- 型安全なUnionの考え方
- std::variant
- std::visit
- コンパイル時型判定(if constexpr)# OneDayOneCode

## タイトル
Variant Sensor Logger

## 問題

3種類のセンサデータを `std::variant` を使って表現してください。

扱うデータは以下の3種類です。

- int       : 車速[km/h]
- double    : バッテリー電圧[V]
- std::string : ステータスメッセージ

以下の順番でデータを `std::vector<std::variant<...>>` に格納してください。

80
13.8
"READY"
100
12.5
"ERROR"

`std::visit` を用いて取り出し、型ごとに以下の形式で表示してください。

Speed : 80 km/h
Voltage : 13.8 V
Status : READY
Speed : 100 km/h
Voltage : 12.5 V
Status : ERROR

## 条件

- C++17以上
- dynamic_castは禁止
- typeidは禁止
- std::variant
- std::visit
- if constexpr と std::is_same_v を利用すること

## 学べること

- 型安全なUnionの考え方
- std::variant
- std::visit
- コンパイル時型判定(if constexpr)