from collections import deque


def moving_average(values, window_size):
	"""Return the moving average for each value in ``values``."""
	if window_size <= 0:
		raise ValueError("window_size must be greater than 0")

	recent_values = deque()
	total = 0
	averages = []

	for value in values:
		if len(recent_values) == window_size:
			total -= recent_values.popleft()

		recent_values.append(value)
		total += value
		averages.append(total / len(recent_values))

	return averages

# test
def main():
	values = [10, 20, 30, 40, 50]
	window_size = 3

	print(moving_average(values, window_size))


if __name__ == "__main__":
	main()

