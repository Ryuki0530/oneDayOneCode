import sys
from collections import Counter

def main() -> None:
    text = sys.stdin.readline().rstrip("\n")
    counts = Counter(ch for ch in text if ch != " ")
    for ch in sorted(counts):
        print(f"{ch}:{counts[ch]}")

if __name__ == "__main__":
    main()