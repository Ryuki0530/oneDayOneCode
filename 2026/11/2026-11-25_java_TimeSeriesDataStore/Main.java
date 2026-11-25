import java.util.Map;
import java.util.NavigableMap;
import java.util.TreeMap;

class TimeSeriesDataStore {
	private final TreeMap<Long, Double> data = new TreeMap<>();

	public void add(long timestamp, double value) {
		data.put(timestamp, value);
	}

	public Double getLatest(long timestamp) {
		Map.Entry<Long, Double> entry = data.floorEntry(timestamp);
		return entry == null ? null : entry.getValue();
	}

	public Double getAverage(long start, long end) {
		if (start > end) {
			throw new IllegalArgumentException("start must not be greater than end");
		}

		NavigableMap<Long, Double> range = data.subMap(start, true, end, true);
		if (range.isEmpty()) {
			return null;
		}

		double sum = 0.0;
		for (double value : range.values()) {
			sum += value;
		}
		return sum / range.size();
	}

	public int removeBefore(long timestamp) {
		NavigableMap<Long, Double> olderEntries = data.headMap(timestamp, false);
		int removed = olderEntries.size();
		olderEntries.clear();
		return removed;
	}

	public int size() {
		return data.size();
	}

	public Double getNearest(long timestamp) {
		Map.Entry<Long, Double> before = data.floorEntry(timestamp);
		Map.Entry<Long, Double> after = data.ceilingEntry(timestamp);

		if (before == null) {
			return after == null ? null : after.getValue();
		}
		if (after == null || before.getKey().equals(after.getKey())) {
			return before.getValue();
		}

		long distanceBefore = timestamp - before.getKey();
		long distanceAfter = after.getKey() - timestamp;
		return Long.compareUnsigned(distanceBefore, distanceAfter) <= 0
				? before.getValue()
				: after.getValue();
	}
}



public class Main {
	public static void main(String[] args) {
		TimeSeriesDataStore store = new TimeSeriesDataStore();

		store.add(1000, 10.0);
		store.add(3000, 30.0);
		store.add(2000, 20.0);
		store.add(4000, 40.0);

		System.out.println(store.getLatest(2500));
		System.out.println(store.getLatest(500));
		System.out.println(store.getAverage(1000, 3000));

		store.add(2000, 25.0);

		System.out.println(store.getLatest(2500));
		System.out.println(store.removeBefore(3000));
		System.out.println(store.size());
		System.out.println(store.getAverage(3000, 4000));
	}
}


