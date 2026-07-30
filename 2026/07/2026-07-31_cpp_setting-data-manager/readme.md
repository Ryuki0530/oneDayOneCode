```markdown
# 今日の課題：RAIIで一時ファイルを管理するクラス

## 概要

LinuxのシステムコールとC++のクラス設計を組み合わせて、  
**一時ファイルを自動的に削除するクラス**を作成してください。

今回はWSL上でC++を使用します。

---

## 要件

`TemporaryFile` クラスを実装してください。

### コンストラクタ

```cpp
TemporaryFile(const std::string& path);
```

以下の処理を行います。

1. `open()` を使ってファイルを作成する
2. 読み書き可能な状態で開く
3. 既存ファイルがある場合は内容を空にする
4. `open()` に失敗した場合は例外を送出する

使用するフラグは以下を参考にしてください。

```cpp
O_CREAT | O_RDWR | O_TRUNC
```

---

### `writeText`

```cpp
void writeText(const std::string& text);
```

`write()` システムコールを使って、ファイルへ文字列を書き込みます。

`write()` が途中までしか書き込まない場合も考慮し、  
文字列全体が書き込まれるまで繰り返してください。

---

### `readText`

```cpp
std::string readText();
```

以下の処理を行います。

1. `lseek()` を使って読み取り位置をファイル先頭へ戻す
2. `read()` を使ってファイル内容を読み取る
3. 読み取った内容を `std::string` として返す

バッファサイズは `128` バイト程度で構いません。

---

### デストラクタ

```cpp
~TemporaryFile();
```

以下の後処理を行います。

1. `close()` でファイルディスクリプタを閉じる
2. `unlink()` でファイルを削除する

これにより、オブジェクトがスコープを抜けたときに、一時ファイルが自動削除されるようにしてください。

---

## クラスの制約

コピーによって同じファイルディスクリプタが複数のオブジェクトから管理されることを防ぐため、コピーを禁止してください。

```cpp
TemporaryFile(const TemporaryFile&) = delete;
TemporaryFile& operator=(const TemporaryFile&) = delete;
```

余裕があれば、ムーブコンストラクタとムーブ代入演算子を実装してください。

---

## `main` 関数で行うこと

以下の流れを確認してください。

```cpp
int main() {
    {
        TemporaryFile file("/tmp/one_day_one_code.txt");

        file.writeText("Hello system call!\n");
        file.writeText("Managed by RAII.\n");

        std::cout << file.readText();
    }

    // この時点でファイルが削除されていることを確認する
}
```

`access()` システムコールを使い、スコープ終了後にファイルが存在しないことも確認してください。

期待する出力例：

```text
Hello system call!
Managed by RAII.
Temporary file was deleted.
```

---

## 使用する主なヘッダ

```cpp
#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
```

---

## コンパイル・実行

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
./main
```

---

## 学習ポイント

- `open()`、`read()`、`write()`、`lseek()`、`close()`、`unlink()` の使い方
- ファイルディスクリプタの管理
- システムコールが失敗した場合の処理
- RAIIによるリソース管理
- コピー禁止による二重解放の防止
- デストラクタを使った自動後処理

今回の中心は、**ファイルディスクリプタを単なる整数ではなく、クラスが所有するリソースとして扱うこと**です。

`write()` の部分書き込み対応までできれば十分です。
```