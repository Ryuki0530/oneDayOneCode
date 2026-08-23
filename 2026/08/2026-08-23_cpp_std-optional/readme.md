# 今日のC++課題：`std::optional`で「値がない」を表現する

## 難易度
★★☆☆☆

## 目的
- `std::optional` の基本的な使い方を理解する
- 「値が存在しない可能性」を戻り値として表現する
- `-1` などの特殊値に頼らない設計を体験する

## 課題

次のような設定情報を持つ `Config` クラスを作成してください。

    Config config;

    config.set("port", 8080);
    config.set("timeout", 30);

    auto port = config.get("port");
    auto retry = config.get("retry");

`Config` クラスは内部に

    std::unordered_map<std::string, int>

を持つものとします。

## 実装するメソッド

    void set(const std::string& key, int value);

    std::optional<int> get(const std::string& key) const;

`get()` は、

- キーが存在する → その値を返す
- キーが存在しない → `std::nullopt` を返す

ようにしてください。

## 出力例

    port = 8080
    timeout = 30
    retry = not found

## 条件

- C++17を使用する
- `get()` の戻り値は `std::optional<int>` とする
- キーが存在しない場合に `-1` などの特殊値を使用しない
- `main()` 側では `std::optional` に値が入っているか確認してから表示する

## 使用するもの

    #include <iostream>
    #include <optional>
    #include <string>
    #include <unordered_map>

## コンパイル

    g++ -std=c++17 -Wall -Wextra main.cpp -o main

## おまけ

余裕があれば、次のメソッドも追加してください。

    int get_or(const std::string& key, int default_value) const;

例えば、

    std::cout << config.get_or("retry", 3) << '\n';

なら、`retry` が存在しないので

    3

を返すようにしてください。