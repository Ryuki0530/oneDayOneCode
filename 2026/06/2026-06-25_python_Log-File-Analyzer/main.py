from collections import Counter
import sys


def read_log(path):
	with open(path, "r", encoding="utf-8") as f:
		return f.readlines()


def count_levels(lines):
	counter = Counter()
	error_messages = []

	for line in lines:
		line = line.rstrip("\n")
		if not line.strip():
			continue

		level, _, message = line.partition(" ")
		if level in {"INFO", "WARNING", "ERROR"}:
			counter[level] += 1
			if level == "ERROR" and message:
				error_messages.append(message)

	return counter, error_messages


def print_summary(counter):
	print("=== Log Summary ===")
	for level in ("INFO", "WARNING", "ERROR"):
		print(f"{level:<8}: {counter.get(level, 0)}")

	most_common = counter.most_common(1)
	if most_common:
		print()
		print(f"Most Frequent Level : {most_common[0][0]}")


def print_error_messages(error_messages):
	if not error_messages:
		return

	print()
	print("=== Error Messages ===")
	for message in error_messages:
		print(message)


def main():
	if len(sys.argv) < 2:
		return

	try:
		lines = read_log(sys.argv[1])
	except FileNotFoundError:
		print("File not found")
		return

	counter, error_messages = count_levels(lines)
	print_summary(counter)
	print_error_messages(error_messages)


if __name__ == "__main__":
	main()
