import sys


def main() -> None:
	data = list(map(int, sys.stdin.buffer.read().split()))
	if not data:
		return

	n, w, t = data[0], data[1], data[2]
	a = data[3:3 + n]

	window_sum = sum(a[:w])
	threshold_add = t * w
	ans = []

	for i in range(w, n):
		if a[i] * w >= window_sum + threshold_add:
			ans.append(str(i + 1))
		window_sum += a[i] - a[i - w]

	sys.stdout.write("\n".join(ans) if ans else "OK")


if __name__ == "__main__":
	main()
