import sys
from collections import Counter

def main():
    text = sys.stdin.read()
    words = text.split()
    counter = Counter(words)
    most_common = counter.most_common(1)
    print(most_common[0][0], most_common[0][1])

if __name__ == "__main__":
    main()
