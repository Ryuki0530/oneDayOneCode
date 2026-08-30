# 今日のC++課題：バイナリパケットの解析

## 目的

`std::vector<std::uint8_t>` に格納されたバイナリデータから、各フィールドを取り出す処理を実装してください。

## パケット形式

データは次の順番で格納されています。

| オフセット | サイズ | 内容 |
|---:|---:|---|
| 0 | 1バイト | メッセージID |
| 1 | 2バイト | センサ値（符号なし16ビット整数） |
| 3 | 1バイト | ステータス |
| 4 | 1バイト | チェックサム |

センサ値はビッグエンディアンです。

```text
センサ値 = (上位バイト << 8) | 下位バイト
```

チェックサムは、チェックサム自身を除く先頭4バイトの合計値の下位8ビットです。

```text
checksum = (byte[0] + byte[1] + byte[2] + byte[3]) & 0xFF
```

## 実装するもの

次の構造体と関数を実装してください。

```cpp
struct Packet {
    std::uint8_t messageId;
    std::uint16_t sensorValue;
    std::uint8_t status;
};

std::optional<Packet> parsePacket(
    const std::vector<std::uint8_t>& data
);
```

`parsePacket()` は次の場合に `std::nullopt` を返してください。

- データサイズが5バイトではない
- チェックサムが一致しない

正常な場合は、解析した `Packet` を返してください。

## 動作確認用コード

以下のデータで確認してください。

```cpp
int main() {
    std::vector<std::uint8_t> validData{
        0x10, 0x12, 0x34, 0x01, 0x57
    };

    std::vector<std::uint8_t> invalidData{
        0x10, 0x12, 0x34, 0x01, 0x00
    };

    // parsePacket()を呼び出し、
    // 成功した場合は各フィールドを表示する
    // 失敗した場合は "Invalid packet" と表示する
}
```

## 期待される出力

```text
Message ID: 16
Sensor value: 4660
Status: 1
Invalid packet
```

## 条件

- C++17を使用する
- `std::optional` を使用する
- C形式のキャストは使用しない
- `std::uint8_t` を直接 `std::cout` に渡すと文字として扱われるため、整数型へ変換して表示すること

## コンパイル例

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
```