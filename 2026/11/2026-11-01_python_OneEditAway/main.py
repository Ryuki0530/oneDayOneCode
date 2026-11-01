def is_one_edit_away(a: str, b: str) -> bool:
	"""Return whether a and b differ by at most one edit."""
	if abs(len(a) - len(b)) > 1:
		return False

	# Keep a as the shorter string for the insertion/deletion case.
	if len(a) > len(b):
		a, b = b, a

	i = j = differences = 0
	while i < len(a) and j < len(b):
		if a[i] != b[j]:
			differences += 1
			if differences > 1:
				return False
			if len(a) == len(b):
				i += 1
		else:
			i += 1
		j += 1

	return True


def main() -> None:
	examples = [
		("pale", "ple", True),
		("pale", "pales", True),
		("pale", "bale", True),
		("pale", "bake", False),
		("same", "same", True),
		("", "a", True),
		("", "ab", False),
		("ab", "ba", False),
		("", "", True),
	]

	for a, b, expected in examples:
		result = is_one_edit_away(a, b)
		print(f'{a!r}, {b!r}: {result} (expected {expected})')
		assert result == expected


if __name__ == "__main__":
	main()
