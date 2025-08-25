def main():
    text = input()
    word_count = {}
    for word in text.lower().split():
        word_count[word] = word_count.get(word, 0) + 1
    for word, count in sorted(word_count.items(), key=lambda x: (-x[1], x[0])):
        print(f"{word} {count}")

if __name__ == "__main__":
    main()