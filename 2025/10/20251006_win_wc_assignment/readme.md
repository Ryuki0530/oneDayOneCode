# OneDayOneCode / C（Windows）課題 — Win32 API だけで作る `win_wc`（行・単語・バイト数カウンタ）

## 目的
C言語で **標準ライブラリの `fopen/fread` 等を使わず**、Windows の **Win32 API**（いわゆる「Windowsのシステムコール」をラップしたユーザモードAPI）を直接呼び出して、ファイル／標準入力を読み取り、行数・単語数・バイト数を数えるコマンドラインツール `win_wc` を作成する。

学べる点：
- `CreateFileW` / `ReadFile` / `WriteFile` / `CloseHandle` / `GetFileSizeEx` / `GetStdHandle`
- wide 文字（UTF-16）引数の扱い（`wmain`）
- ハンドルと同期 I/O、バッファリング
- コマンドライン引数と標準入出力（`STD_INPUT_HANDLE` / `STD_OUTPUT_HANDLE`）

---

## 仕様

### 実行形式
```
win_wc.exe [PATH ...]
```
- 引数なし：標準入力から読み取る。
- 引数に `-` がある場合は、その位置で標準入力を読む（UNIX の `wc` 互換の雰囲気）。
- 1 個以上のパスを渡した場合：各ファイルを順に集計し、最後に **合計** を表示する。

### 出力形式（スペース区切り）
```
<lines> <words> <bytes> <name>
```
- `<name>` はファイル名。標準入力は `-` と表示。
- 複数ファイルのとき、最後に
  ```
  <total_lines> <total_words> <total_bytes> total
  ```
  を出力。

### 定義
- **行**：LF（`\n`、0x0A）で 1 行。CRLF（`\r\n`）は 1 行として数える（`\r` は行にカウントしない）。
- **単語**：**ASCII の空白類**（スペース、タブ、CR、LF、VT、FF）で区切られた非空白の連続列を 1 語とみなす。
- **バイト数**：ファイルは `GetFileSizeEx` の値。標準入力は `ReadFile` で読み取った合計バイト数。
  - テキストのエンコーディング（UTF-8/Shift-JIS 等）は問わない。**あくまでバイト単位**で数える。

### 制約（必読）
- 標準 C の入出力 API（例：`fopen`, `fread`, `fgets`, `fprintf` 等）は **禁止**。必ず **Win32 API** を用いること。
- パスは Unicode 対応（`CreateFileW` を使うこと）。エントリーポイントは `wmain`。
- 読み取りバッファは 64 KiB 程度（推奨）。
- 出力の生成も `WriteFile` で行う（`printf` 等は避ける練習）。
- エラーは標準エラー（`STD_ERROR_HANDLE`）に英語短文で出す（例：`open/read error: <path>`）。

---

## 使用する主な Win32 API（例）
- `HANDLE CreateFileW(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE)`
- `BOOL ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED)`
- `BOOL WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED)`
- `BOOL CloseHandle(HANDLE)`
- `BOOL GetFileSizeEx(HANDLE, PLARGE_INTEGER)`
- `HANDLE GetStdHandle(DWORD)`（`STD_INPUT_HANDLE`, `STD_OUTPUT_HANDLE`, `STD_ERROR_HANDLE`）

---

## 入出力例

### 例1：ファイル 1 つ
```powershell
PS> type sample.txt
hello world
こんにちは

PS> .\win_wc.exe sample.txt
2 3 <bytes> sample.txt
```
- 行 = 2（`hello world\n` と `こんにちは\n`）
- 単語 = 3（`hello`, `world`, `こんにちは`）
- バイト数 `<bytes>` はエンコーディングに依存（UTF-8 の日本語は複数バイト）。

### 例2：複数ファイル＋合計
```powershell
PS> .\win_wc.exe a.txt b.txt
12 98 6543 a.txt
 8 70 4210 b.txt
20 168 10753 total
```

### 例3：標準入力から
```powershell
PS> Get-Content .\a.txt | .\win_wc.exe
12 98 6543 -
```

---

## ビルド手順（VSCode ターミナルから）

### MSVC（Developer PowerShell / Developer CMD）
```bat
cl /EHsc /W4 /DUNICODE /D_UNICODE win_wc.c /link kernel32.lib
```

### MinGW-w64（PowerShell / CMD）
```bat
gcc -O2 -municode win_wc.c -o win_wc.exe
```

> 文字化けを避けたい場合は、コンソールを UTF-8（`chcp 65001`）にするか、エディタ側の表示を調整してください。出力は ASCII のみです。

---

## 実装のヒント（参考／コード提出不要）
- `wmain(int argc, wchar_t** argv)` を使う。
- 引数が 0 のとき・`-` のときは `GetStdHandle(STD_INPUT_HANDLE)` → `ReadFile`。
- ファイルは `CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL)` で開く。
- ループで `ReadFile` → バッファ走査：
  - `\n` で行数++
  - 直前が「非単語中」で、現在が「非空白」→ 単語開始（語数++）
  - 現在が空白→ 単語フラグを OFF
- CRLF 対応：`\r` は無視、`\n` でのみ行カウント。
- ファイルのバイト数は `GetFileSizeEx`、標準入力は読み取り合計。
- 出力は `WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), ...)` で行う。

擬似コード：
```
total = (0,0,0)
if argc == 1:
    c = count_from_handle(STD_INPUT_HANDLE)
    print(c, name="-")
else:
    for each arg:
        if arg == "-":
            c = count_from_handle(STD_INPUT_HANDLE)
            print(c, "-")
        else:
            c = count_from_file(arg)
            print(c, arg)
        total += c
    if processed_files >= 2:
        print(total, "total")
```

---

## 評価観点
- Win32 API を正しく使っている（禁止 API を使っていない）。
- 複数ファイル／標準入力を扱える。
- CRLF/LF の行数カウントが正しい。
- 大きなファイルでも正しく・効率よく処理できる（バッファループ）。
- エラー処理（開けないファイル等）の簡潔な表示。

---

## 発展課題（任意）
1. `-l` オプションで **最長行の長さ** を追加表示。
2. `--utf16` オプションで UTF-16LE テキストを **文字単位**で解析（BOM 検出、2バイト単位処理）。
3. `--bench` で `FILE_FLAG_SEQUENTIAL_SCAN` の有無やバッファサイズを変えてスループット比較。

---

## 提出物
- `win_wc.c`（単一ファイルで可）
- 実行例（スクリーンショットまたはログ）
- 任意：発展課題の所感（簡単な計測数値と気づき）

---

**Have fun.** コンピュータは素直、バイトは正直。境界を丁寧に扱えば、世界はだいたい優しく動く。