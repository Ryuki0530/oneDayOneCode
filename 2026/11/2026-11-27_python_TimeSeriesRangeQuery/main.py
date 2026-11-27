

import bisect


class TimeSeriesRangeQuery:
    def __init__(self):
        self.data = {}
        self.timestamps = []

    def add(self, timestamp: int, value: float) -> None:
        index = bisect.bisect_left(self.timestamps, timestamp)
        if index < len(self.timestamps) and self.timestamps[index] == timestamp:
            self.data[timestamp] = value
        else:
            self.timestamps.insert(index, timestamp)
            self.data[timestamp] = value

    def query(self, start: int, end: int) -> list:
        if start > end:
            raise ValueError("start must be less than or equal to end")

        left = bisect.bisect_left(self.timestamps, start)
        right = bisect.bisect_right(self.timestamps, end)
        return [
            (timestamp, self.data[timestamp])
            for timestamp in self.timestamps[left:right]
        ]

    def latest(self, timestamp: int) -> tuple | None:
        index = bisect.bisect_right(self.timestamps, timestamp) - 1
        if index < 0:
            return None
        latest_timestamp = self.timestamps[index]
        return latest_timestamp, self.data[latest_timestamp]



def main():
    series = TimeSeriesRangeQuery()

    series.add(100, 12.5)
    series.add(300, 18.0)
    series.add(200, 15.2)
    series.add(500, 20.1)
    series.add(400, 17.8)

    print(series.query(200, 400))
    print(series.query(150, 350))
    print(series.query(600, 800))

    print(series.latest(350))
    print(series.latest(100))
    print(series.latest(50))

    series.add(300, 99.9)

    print(series.query(300, 300))


if __name__ == "__main__":
    main()