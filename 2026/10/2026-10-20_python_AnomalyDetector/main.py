def detect_anomalies(data, window_size, threshold):
	anomalies = []

	for index in range(window_size, len(data)):
		window = data[index - window_size:index]
		average = sum(window) / window_size

		if abs(data[index] - average) > threshold:
			anomalies.append((index, data[index]))

	return anomalies


def main():
    data = [10, 11, 10, 12, 11, 50, 12, 10, 9, 11]
    window_size = 3
    threshold = 15
	
    print(detect_anomalies(data, window_size, threshold))


if __name__ == "__main__":
	main()

