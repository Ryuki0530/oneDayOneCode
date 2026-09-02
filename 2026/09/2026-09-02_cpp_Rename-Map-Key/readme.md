# RenameMapKey

## 課題

C++17の `std::map::extract()` を使い、マップのキーを変更する関数を実装してください。

次の関数を完成させてください。

```cpp
bool renameKey(
    std::map<std::string, int>& scores,
    const std::string& oldKey,
    const std::string& newKey);
```

## 要件

- `oldKey` が存在しない場合は、何も変更せず `false` を返す
- `newKey` がすでに存在する場合は、何も変更せず `false` を返す
- 変更に成功した場合は `true` を返す
- 値をコピーして新しい要素を作るのではなく、`std::map::extract()` で取り出したノードのキーを変更する
- C++17を使用する

## 実行例

変更前:

```text
Alice: 80
Bob: 95
Carol: 70
```

`Bob` を `Robert` に変更した後:

```text
Alice: 80
Carol: 70
Robert: 95
```

## ひな形

```cpp
#include <iostream>
#include <map>
#include <string>

bool renameKey(
    std::map<std::string, int>& scores,
    const std::string& oldKey,
    const std::string& newKey)
{
    // ここを実装する
    return false;
}

void printScores(const std::map<std::string, int>& scores)
{
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << '\n';
    }
}

int main()
{
    std::map<std::string, int> scores{
        {"Alice", 80},
        {"Bob", 95},
        {"Carol", 70}
    };

    if (renameKey(scores, "Bob", "Robert")) {
        std::cout << "Renamed successfully\n";
    } else {
        std::cout << "Rename failed\n";
    }

    printScores(scores);

    return 0;
}
```

## ビルド例

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
.\main.exe
```

## ヒント

`extract()` が返すノードハンドルでは、次のようにキーを変更できます。

```cpp
node.key() = newKey;
```

変更後は `insert()` を使ってマップへ戻します。