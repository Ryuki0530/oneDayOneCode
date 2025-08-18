# 課題: `cat` の簡易版を作ろう  

UNIX にある `cat` コマンド（ファイルの中身を標準出力に表示するツール）を **UNIXシステムコールのみ** を用いて実装してください。  

## 要件
1. **使用する関数**  
   - `open`, `read`, `write`, `close` を用いること。  
   - 標準Cライブラリ関数（`fopen`, `fprintf` など）は使用禁止。  

2. **引数でファイルを指定すること**  
   - プログラムの実行時にファイル名を1つ引数として受け取る。  
   - 例:  
     ```bash
     ./mycat sample.txt
     ```

3. **動作仕様**  
   - 指定したファイルを開き、その内容を標準出力に表示する。  
   - 読み込みは1回で終わらせず、バッファを用いて複数回 `read` を行うこと。  

4. **エラー処理**  
   - 引数が指定されていない場合はエラーメッセージを表示して終了すること。  
   - ファイルが存在しない場合や開けない場合もエラーメッセージを表示すること。  

## 実行例
```bash
$ ./mycat hello.txt
Hello, World!
This is a test file.
```

```bash
$ ./mycat
Usage: ./mycat <filename>
```

```bash
$ ./mycat notfound.txt
Error: cannot open file
```
