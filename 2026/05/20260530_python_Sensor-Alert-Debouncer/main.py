import sys


def main() -> None:
	data = sys.stdin.buffer.read().split()
	if not data:
		return

	n = int(data[0])
	x = int(data[1])
	k = int(data[2])

	count = 0
	alerting = False
	out = []

	idx = 3
	for _ in range(n):
		t = int(data[idx])
		temp = int(data[idx + 1])
		idx += 2

		if temp >= x:
			count += 1
			if not alerting and count >= k:
				out.append(str(t))
				alerting = True
		else:
			count = 0
			alerting = False

	sys.stdout.write("\n".join(out))


if __name__ == "__main__":
	main()
