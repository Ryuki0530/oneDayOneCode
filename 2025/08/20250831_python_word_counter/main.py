from collections import Counter

def count_words(text):
    words = text.split()
    return Counter(words)

def main():
    input_text = input("Enter text: ")
    word_counts = count_words(input_text)
    for word, count in word_counts.items():
        print(f"{word}: {count}")

if __name__ == "__main__":
    main()