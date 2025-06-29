# 20250628 C言語課題 - ディレクトリ内ファイルサイズ集計ツール

## 問題概要

指定されたディレクトリ内に存在するすべての通常ファイル（サブディレクトリは除く）について、  
それぞれのファイルサイズと合計サイズを出力するプログラムを作成してください。

## 入出力仕様

### 入力

プログラムのコマンドライン引数として、対象ディレクトリのパスを受け取ります。

```sh
$ ./file_size_sum ./sample_dir
```
出力
各ファイルの名前とサイズ（バイト単位）を1行ずつ出力

最後に合計サイズを出力

出力例（sample_dir内にa.txtとb.txtがある場合）
a.txt: 120 bytes<br>
b.txt: 300 bytes<br>
Total: 420 bytes<br><br>
条件
opendir, readdir, stat を使用してください。

ディレクトリは1階層のみ対象とし、サブディレクトリの中は再帰しません。

ファイルサイズの取得には stat() の st_size を使ってください。

Windows環境でWSLを使って実行してください（Linux系関数のため）。

ヒント
```
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
```
`readdir()` で . や .. を除外するのを忘れずに。

d_type で DT_REG かどうか確認（ただし環境によっては使えないため stat() の方が確実）。

