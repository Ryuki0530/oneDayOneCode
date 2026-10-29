def find_matches(words, pattern):
	"""Return unique words matching pattern in their original order."""
	matches = []
	seen = set()

	for word in words:
		if word in seen or len(word) != len(pattern):
			continue

		is_match = True
		for word_char, pattern_char in zip(word, pattern):
			if pattern_char != "?" and pattern_char != word_char:
				is_match = False
				break

		if is_match:
			matches.append(word)
			seen.add(word)

	return matches


def main():
	words = ["cat", "car", "cut", "cart", "cat", "Cat", "dog"]

	for pattern in ("c?t", "???", "z??"):
		print(find_matches(words, pattern))




if __name__ == "__main__":
	main()
