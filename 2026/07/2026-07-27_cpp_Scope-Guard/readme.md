# OneDayOneCode

## タイトル
ScopeGuard（スコープガード）の実装

## 難易度
★★☆☆☆

## 使用言語
C++17

## 問題

RAIIを利用して、スコープを抜ける際に登録した処理を必ず実行する
`ScopeGuard`クラスを実装してください。

以下のようなコードが動作することを目標とします。

```cpp
#include <iostream>

int main() {
    std::cout << "Start\n";

    {
        ScopeGuard guard([]{
            std::cout << "Cleanup\n";
        });

        std::cout << "Working\n";
    }

    std::cout << "End\n";
}
```

### 実行例

```
Start
Working
Cleanup
End
```

---

## 要件

- コンストラクタで任意のCallableを受け取る
- デストラクタでそのCallableを1回だけ実行する
- コピーは禁止
- ムーブは許可
- `dismiss()` を呼ぶと実行されなくなる

---

## 発展課題

次のコードが期待通り動作すること。

```cpp
FILE* fp = fopen("sample.txt", "w");

ScopeGuard guard([&]{
    fclose(fp);
    std::cout << "File closed\n";
});

fprintf(fp, "Hello\n");
```

例外が発生しても必ず `fclose()` が呼ばれる設計になっていることを確認してください。