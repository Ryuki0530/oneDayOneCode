from difflib import get_close_matches


REGISTERED_WORDS = ["start", "stop", "status", "restart", "reset"]


def suggest_command(word):
	normalized_word = word.strip().lower()

	if not normalized_word:
		return "EMPTY"
	if normalized_word in REGISTERED_WORDS:
		return f"OK: {normalized_word}"

	candidates = get_close_matches(
		normalized_word, REGISTERED_WORDS, n=2, cutoff=0.6
	)
	if candidates:
		return f"候補: {', '.join(candidates)}"
	return "候補なし"


if __name__ == "__main__":
	for value in (" START ", "stats", "xyz", "   "):
		print(suggest_command(value))
