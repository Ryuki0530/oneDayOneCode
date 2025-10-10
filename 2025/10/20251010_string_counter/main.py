import sys
import re
from collections import Counter

def main():
    text = sys.stdin.read()
    words = re.findall(r'[A-Za-z0-9]+', text)
    words = [w.lower() for w in words]
    if not words:
        return
    counter = Counter(words)
    max_count = max(counter.values())
    candidates = [w for w, c in counter.items() if c == max_count]
    result = min(candidates)
    print(f"{result} {max_count}")

if __name__ == "__main__":
    main()