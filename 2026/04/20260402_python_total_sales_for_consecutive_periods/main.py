import sys


def main() -> None:
	data = list(map(int, sys.stdin.buffer.read().split()))
	if not data:
		return

	n, k = data[0], data[1]
	a = data[2:2 + n]

	window_sum = sum(a[:k])
	max_sum = window_sum

	for i in range(k, n):
		window_sum += a[i] - a[i - k]
		if window_sum > max_sum:
			max_sum = window_sum

	print(max_sum)


if __name__ == "__main__":
	main()
