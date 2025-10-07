import sys
import re
from collections import Counter

def words_from_text(text):
    words = re.findall(r'[A-Za-z0-9]+', text)
    return [w.lower() for w in words]

def count_words(words):
    return Counter(words)


def main():
    input_lines = sys.stdin.read().splitlines()
    if not input_lines:
        return
    K = int(input_lines[0])
    text = '\n'.join(input_lines[1:])
    words = words_from_text(text)
    counter = count_words(words)
    for word, count in counter.items():
        print(f"{word} {count}")
    


if __name__ == "__main__":
    main()