from collections import deque

class LogStreamAnalyzer:
    #constructor
    def __init__(self, window_size: int, threshold: float):
        if window_size < 1:
            raise ValueError("window_size must be >= 1")
        if threshold < 0:
            raise ValueError("threshold must be >= 0")

        self.window_size = window_size
        self.threshold = threshold
        self.log_stream = deque(maxlen=window_size)

        self.total = 0.0

    def add(self, value: float) -> bool:
        is_anomaly = False

        if len(self.log_stream) == self.window_size:
            average = self.total / self.window_size
            is_anomaly = abs(value - average) > self.threshold
            self.total -= self.log_stream[0]

        self.log_stream.append(value)
        self.total += value

        return is_anomaly

    def get_average(self) -> float:
        if not self.log_stream:
            return 0.0
        return self.total / len(self.log_stream)
    
def main():
    analyzer = LogStreamAnalyzer(window_size=3, threshold=10)
    print(analyzer.add(10))  # False
    print(analyzer.add(12))  # False
    print(analyzer.add(11))  # False
    print(analyzer.add(50))  # True
    print(analyzer.add(13))  # False
    print(analyzer.get_average())  # 約24.67

if __name__== "__main__":
    main()