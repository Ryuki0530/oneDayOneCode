import sys
import re
from collections import Counter

WORD_RE = re.compile(rb'[A-Za-z0-9]+')  # bytes用にして高速化

def main():
    data = sys.stdin.buffer.read()
    if not data:
        return

    # 1行目のKだけはテキストとして読む必要があるので、最初の改行まで切り出す
    first_nl = data.find(b'\n')
    if first_nl == -1:
        # 1行しかない=Kしか無い → 何も数えない
        try:
            int(data.strip() or b'0')
        except ValueError:
            return
        return

    k_bytes = data[:first_nl].strip()
    try:
        K = int(k_bytes.decode('utf-8', 'ignore'))
    except ValueError:
        K = 0

    text_bytes = data[first_nl+1:]  # 2行目以降は全部バイト列のまま処理

    # バイト列のまま単語抽出 → 小文字化もバイトで
    # ASCII英数字のみなので bytes.lower() でOK
    words = (m.group(0).lower() for m in WORD_RE.finditer(text_bytes))

    counter = Counter()
    counter.update(w.decode('ascii') for w in words)  # 辞書順は str で統一

    if K <= 0:
        return

    # (-頻度, 単語) で安定ソート
    for word, cnt in sorted(counter.items(), key=lambda x: (-x[1], x[0]))[:K]:
        print(f"{word} {cnt}")

if __name__ == "__main__":
    main()

'''
buffer.read()＋バイト正規表現で速度UP（巨大入力でも快適）。
Counter.update(ジェネレータ)で中間リストを作らず低メモリ。
K の例外処理とゼロ/負値を自然に無視。
'''