from collections import Counter

words = [
	"apple",
	"banana",
	"apple",
	"orange",
	"banana",
	"apple",
]
word, count = Counter(words).most_common(1)[0]
print(f"{word}: {count}")
