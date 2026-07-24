# C++ Task: Subprocess Output Capture

## 概要

Linuxのシステムコールを使用し、子プロセスが実行したコマンドの標準出力を、親プロセスが取得するプログラムを作成してください。

## 使用する主な機能

- `pipe()`
- `fork()`
- `dup2()`
- `execvp()`
- `read()`
- `waitpid()`

## 要件

1. 親プロセスでパイプを作成する。
2. `fork()`で子プロセスを生成する。
3. 子プロセスでは、標準出力をパイプの書き込み側へ接続する。
4. 子プロセスから次のコマンドを実行する。

    uname -a

5. 親プロセスはパイプから出力を読み取り、以下の形式で表示する。

    Child output:
    Linux ...

6. 親プロセスは`waitpid()`を使用して子プロセスの終了を待つ。
7. `pipe()`、`fork()`、`dup2()`、`execvp()`、`read()`の失敗を適切に処理する。
8. 不要になったファイルディスクリプタは必ず`close()`する。

## 制約

- C++17以上
- `std::system()`および`popen()`は禁止
- WSL上で実行すること

## ビルド・実行例

    g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o subprocess
    ./subprocess

## 発展課題

実行するコマンドと引数を、プログラムのコマンドライン引数から指定できるようにしてください。

実行例:

    ./subprocess ls -la