# OneDayOneCode

## 2026-06-27
### C++ : Event Dispatcher

## 問題

イベントディスパッチャを実装してください。

イベントには種類があり、イベント名に応じて登録済みの処理を呼び出します。

以下のイベントを用意してください。

- START
- STOP
- ERROR

---

## 入力例

START
ERROR
STOP
UNKNOWN
START

---

## 出力例

System started
Error occurred
System stopped
Unknown event: UNKNOWN
System started

---

## 要件

- EventDispatcher クラスを作成する
- std::unordered_map を利用してイベント名と処理を管理する
- 処理本体は std::function<void()> で保持する
- registerEvent() を実装する
- dispatch() を実装する
- 未登録イベントは
  Unknown event: <イベント名>
  と表示する

---

## 発展課題

std::function<void()> ではなく

std::function<void(const std::string&)>

へ変更し、

ERROR Disk Full

のような入力では

Error occurred: Disk Full

と表示できるようにしてください。

---

## 学べる内容

- std::unordered_map
- std::function
- ラムダ式
- 関数オブジェクト
- 動的ディスパッチ
- オブジェクト指向での責務分離

---

## ディレクトリ例

2026-06-27_cpp_Event-Dispatcher/

├── CMakeLists.txt
├── include/
│   └── EventDispatcher.hpp
├── src/
│   ├── EventDispatcher.cpp
│   └── main.cpp
└── README.md

---

## コンパイル

mkdir build
cd build

cmake ..
make

./EventDispatcher