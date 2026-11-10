from bisect import bisect_left


def match_nearest_telemetry(frame_times, samples, max_gap_ms):
	sample_times = [recorded_time for recorded_time, _ in samples]
	matched = []

	for frame_time in frame_times:
		index = bisect_left(sample_times, frame_time)
		candidates = []

		if index < len(samples):
			candidates.append(samples[index])
		if index > 0:
			candidates.append(samples[index - 1])

		if not candidates:
			matched.append(None)
			continue

		nearest = min(
			candidates,
			key=lambda sample: (abs(sample[0] - frame_time), sample[0]),
		)
		if abs(nearest[0] - frame_time) <= max_gap_ms:
			matched.append(nearest)
		else:
			matched.append(None)

	return matched


if __name__ == "__main__":
	frame_times = [80, 20, 40, 60, 110]
	samples = [(20, 120), (60, 180), (90, 150)]
	max_gap_ms = 20

	print(match_nearest_telemetry(frame_times, samples, max_gap_ms))

