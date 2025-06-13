# 📅 20250613 - 最頻出単語のカウント（Python）

## 🎯 今日の問題

複数行の英単語から構成される文章を読み取り、その中で最も頻出する単語とその出現回数を求めてください。もし最頻出単語が複数ある場合は、辞書順で最も早いものを出力してください。

---

## 🧾 入力（標準入力）

複数行の文字列が入力されます（英小文字および `'` のみ）。  
各単語は空白で区切られています。  
入力は EOF（end-of-file）まで続きます。

> ※EOFの入力方法：  
> - Linux/macOS：`Ctrl + D`  
> - Windows（コマンドプロンプト）：`Ctrl + Z` → Enter

---

## 🖨️ 出力（標準出力）

最も頻出した単語とその出現回数を、以下の形式で1行で出力してください：

<単語> <回数>



---

## 🔒 制約

- 各行の長さは最大 1000 文字。
- 単語は英小文字およびアポストロフィ（`'`）のみで構成される。
- 入力には最低1つ以上の単語が含まれる。

---

## 💡 入出力例

### ✅ 入力

hello world
this is a test
hello again hello


### 🖨️ 出力

hello 3


---

## 🧠 ヒント

- Pythonでは `sys.stdin.read()` を使って標準入力からまとめて読み取れます。
- 単語の頻度をカウントするには `collections.Counter` を使うと便利です。
- 複数の最頻出単語がある場合、`min()` 関数で辞書順最小の単語を取得できます。

---

## 🧪 サンプルコード（Python）

```python
import sys
from collections import Counter

def main():
    text = sys.stdin.read()
    words = text.split()
    counter = Counter(words)
    max_count = max(counter.values())
    most_common_words = [word for word in counter if counter[word] == max_count]
    print(f"{min(most_common_words)} {max_count}")

if __name__ == "__main__":
    main()
Happy coding! 🎉