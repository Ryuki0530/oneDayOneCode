# C言語課題（Linux環境・システムコール利用）

## 問題文

標準入力から文字列を読み込み、その文字列を標準出力に出力するプログラムを作成してください。  
ただし、**`stdio.h` の `printf` や `scanf` 等の標準ライブラリ関数は使わず**、  
Linux のシステムコール **`read`** と **`write`** を直接呼び出して実装してください。

### 要件

1. 入力バッファサイズは 1024 バイトとする。
2. 入力は 1 行（改行まで）とし、それをそのまま出力する。
3. エラーが発生した場合はエラーメッセージを標準エラー出力に出力して終了する。
4. ヘッダーファイル `<unistd.h>` を使用すること。
5. `gcc` でコンパイルできること。

### 実行例

```bash
$ ./a.out
Hello, system call world!
Hello, system call world!
（入力した文字列がそのまま出力される）
```
ヒント
read と write は以下のシグネチャを持ちます。


```c
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```
fd はファイルディスクリプタで、標準入力は 0、標準出力は 1、標準エラーは 2 です。

エラー処理には <errno.h> と <string.h> の strerror を使うと便利です。