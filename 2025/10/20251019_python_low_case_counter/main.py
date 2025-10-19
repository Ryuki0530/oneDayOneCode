import sys

def main():
    text = sys.stdin.read()
    counts = [0] * 26
    for ch in text:
        if 'a' <= ch <= 'z':
            counts[ord(ch) - ord('a')] += 1
    for i in range(26):
        print(f"{chr(ord('a') + i)}: {counts[i]}")

if __name__ == "__main__":
    main()