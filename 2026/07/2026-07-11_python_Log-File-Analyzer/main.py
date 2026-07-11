from collections import Counter
import sys


def analyze_log(path: str) -> Counter:
	counts = Counter()
	error_lines = []

	with open(path, encoding="utf-8") as f:
		for line in f:
			line = line.strip()
			if not line:
				continue

			level = line.split(":", 1)[0].strip()
			counts[level] += 1

			if level == "ERROR":
				error_lines.append(line)

	if error_lines:
		with open("errors.txt", "w", encoding="utf-8") as f:
			f.write("\n".join(error_lines) + "\n")

	return counts


def main() -> None:
	path = sys.argv[1] if len(sys.argv) > 1 else "log.txt"
	counts = analyze_log(path)

	for level, count in sorted(counts.items(), key=lambda item: (-item[1], item[0])):
		print(f"{level} : {count}")


if __name__ == "__main__":
	main()
