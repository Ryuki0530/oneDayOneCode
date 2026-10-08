import re

from unidecode import unidecode


def make_usernames(names):
	"""Convert names into unique, ASCII-friendly usernames."""
	used = set()
	usernames = []

	for name in names:
		base = unidecode(name).lower()
		base = re.sub(r"[^a-z0-9]+", "_", base).strip("_") or "user"

		username = base
		suffix = 2
		while username in used:
			username = f"{base}_{suffix}"
			suffix += 1

		used.add(username)
		usernames.append(username)

	return usernames


def main():
	names = [
		"José",
		"Jose",
		"JOSÉ!",
		"Müller",
		"Ａｌｉｃｅ",
		"Alice",
		"!!!",
		"",
		"Jose 2",
	]

	print(make_usernames(names))


if __name__ == "__main__":
	main()

