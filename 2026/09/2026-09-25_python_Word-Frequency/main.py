from collections import Counter
import re


text = """
Python is easy to learn.
Python is powerful and Python is useful.
Learning Python is fun.
"""

words = re.findall(r"[a-z]+", text.lower())
word_counts = Counter(words)

for word, count in word_counts.most_common():
	print(f"{word}: {count}")

word, count = word_counts.most_common(1)[0]
print(f"Most common word: {word} ({count})")
