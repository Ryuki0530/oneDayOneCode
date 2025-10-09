# C-087: 連続文字のランレングス圧縮（RLE）

## 説明
英大文字のみからなる文字列を **ランレングス圧縮**（Run-Length Encoding）してください。  
同じ文字が連続している区間ごとに「文字 + 連続回数」を並べた文字列に変換します。連続回数が **1 でも必ず数字を書きます**。

- 例: `AAABCC` → `A3B1C2`

## 入力
- 1 行の文字列 `S`
- 文字は `A`〜`Z` のみ
- 長さは `1 ≤ |S| ≤ 200000`
- 行末の改行（`LF` または `CRLF`）を含む

## 出力
- 圧縮後の文字列を 1 行で出力

## 例
**入力**
```
AAABCC
```
**出力**
```
A3B1C2
```

**入力**
```
Z
```
**出力**
```
Z1
```

**入力**
```
ABBBBBBBBBBBBBBBBBBBBB
```
**出力**
```
A1B20
```

## 実装上の注意（ヒント）
- 入力サイズが大きいので `fgets` で読み取り、末尾の改行は必要に応じて取り除く。
- 連続回数は `int` で足ります（この制約ではオーバーフローしない）。
- 連続回数は数値として出力（例: `printf("%d", count);`）。
- 空白は出現しない前提（`A`〜`Z` のみ）。

## 動作確認テンプレ（任意）
```c
// rle.c
#include <stdio.h>
#include <string.h>

int main(void) {
    static char s[200005]; // +α 余裕
    if (!fgets(s, sizeof(s), stdin)) return 0;

    // 改行除去（LF/CRLF 両対応）
    size_t n = strlen(s);
    if (n && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[--n] = '\0';
        if (n && s[n-1] == '\r') s[--n] = '\0';
    }

    if (n == 0) { puts(""); return 0; }

    char cur = s[0];
    int cnt = 1;
    for (size_t i = 1; i < n; ++i) {
        if (s[i] == cur) {
            ++cnt;
        } else {
            putchar(cur);
            printf("%d", cnt);
            cur = s[i];
            cnt = 1;
        }
    }
    // 最後の束
    putchar(cur);
    printf("%d\n", cnt);
    return 0;
}
```

### コンパイル・実行例（Windows / VSCode）
```bash
gcc -O2 -Wall rle.c -o rle.exe
type input.txt | rle.exe
```
