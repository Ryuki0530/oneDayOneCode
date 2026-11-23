from bisect import bisect_left, bisect_right


class SensorDataInterpolator:
	"""Store sensor readings and linearly interpolate between them."""

	def __init__(self):
		self._data: dict[str, list[tuple[float, float]]] = {}

	def add_data(self, sensor_id: str, timestamp: float, value: float) -> None:
		readings = self._data.setdefault(sensor_id, [])
		timestamps = [reading[0] for reading in readings]
		index = bisect_left(timestamps, timestamp)

		if index < len(readings) and readings[index][0] == timestamp:
			readings[index] = (timestamp, value)
		else:
			readings.insert(index, (timestamp, value))

	def get_value(self, sensor_id: str, timestamp: float) -> float | None:
		readings = self._data.get(sensor_id)
		if not readings:
			return None

		timestamps = [reading[0] for reading in readings]
		index = bisect_left(timestamps, timestamp)

		if index < len(readings) and readings[index][0] == timestamp:
			return readings[index][1]
		if index == 0 or index == len(readings):
			return None

		t0, y0 = readings[index - 1]
		t1, y1 = readings[index]
		return y0 + (y1 - y0) * (timestamp - t0) / (t1 - t0)

	def get_range(
		self, sensor_id: str, start: float, end: float
	) -> list[tuple[float, float]]:
		if start > end:
			return []

		readings = self._data.get(sensor_id)
		if not readings:
			return []

		timestamps = [reading[0] for reading in readings]
		first = bisect_left(timestamps, start)
		last = bisect_right(timestamps, end)
		return readings[first:last].copy()

interpolator = SensorDataInterpolator()

interpolator.add_data("motor1", 0.0, 10.0)
interpolator.add_data("motor1", 2.0, 30.0)
interpolator.add_data("motor1", 5.0, 60.0)

print(interpolator.get_value("motor1", 1.0))
# 20.0

print(interpolator.get_value("motor1", 3.0))
# 40.0

print(interpolator.get_value("motor1", 6.0))
# None

print(interpolator.get_range("motor1", 1.0, 5.0))
# [(2.0, 30.0), (5.0, 60.0)]

interpolator.add_data("motor1", 2.0, 50.0)

print(interpolator.get_value("motor1", 2.0))
# 50.0