def detect_changes(loads, threshold):
	"""Return (index, absolute change) for sudden adjacent load changes."""
	return [
		(index, abs(loads[index] - loads[index - 1]))
		for index in range(1, len(loads))
		if abs(loads[index] - loads[index - 1]) >= threshold
	]


loads = [12, 13, 12, 14, 15, 29, 31, 30, 16, 15]
threshold = 10

for index, change in detect_changes(loads, threshold):
	print(f"index={index}, change={change}")
