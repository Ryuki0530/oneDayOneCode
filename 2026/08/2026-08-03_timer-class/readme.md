# OneDayOneCode（C++）

## 今日のお題
「処理時間計測クラスを作る」

### 目的
- std::chronoの基本的な使い方を覚える
- RAIIと組み合わせた実用的なクラス設計を行う

---

## 課題

`ScopeTimer` クラスを作成してください。

### 要件

- コンストラクタで現在時刻を保存する
- デストラクタで経過時間を表示する
- 単位はミリ秒(ms)
- タイマー名を指定できるようにする

例

```cpp
{
    ScopeTimer timer("Task A");

    // 重い処理
}
```

出力例

```
[Task A] 152 ms
```

---

### main()の例

以下のようなコードで動作確認してください。

```cpp
int main()
{
    {
        ScopeTimer timer("Sleep");

        std::this_thread::sleep_for(
            std::chrono::milliseconds(500));
    }

    {
        ScopeTimer timer("Loop");

        volatile int sum = 0;
        for(int i = 0; i < 100000000; ++i)
            sum += i;
    }
}
```

---

## 使用してよいライブラリ

- <chrono>
- <iostream>
- <string>
- <thread>

---

## 発展課題

① μs(マイクロ秒)でも表示できるようにする

```
[Task] 352 us
```

② クラスに stop() を追加し、
デストラクタを待たずに計測終了できるようにする。

```
ScopeTimer timer("load");

...

timer.stop();
```

デストラクタでは二重に表示しないようにする。

---

## 学べること

- std::chrono::steady_clock
- time_point
- duration_cast
- RAIIによる自動後処理
- スコープ終了時の自動実行