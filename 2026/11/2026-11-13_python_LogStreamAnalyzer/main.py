from collections import deque

class LogStreamAnalyzer:
    #constructor
    def __init__(self, window_size: int, threshold: float):
        self.window_size = window_size
        self.threshold = threshold
        self.log_stream = deque(maxlen=window_size)

    def add(self, value: float):
        self.log_stream.append(value)
        self._analyze()

    def _analyze(self):
        
