from collections import Counter

def main():
    text = input().strip()
    words = text.split()
    counter = Counter(words)
    sorted_items = sorted(counter.items(), key=lambda x: (-x[1], x[0]))
    for word, count in sorted_items:
        print(f"{word}: {count}")
if __name__ == "__main__":
    main()