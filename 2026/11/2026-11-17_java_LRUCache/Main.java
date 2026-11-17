import java.util.LinkedHashMap;
import java.util.Map;

class LRUCache {
	private final int capacity;
	private final LinkedHashMap<Integer, Integer> cache;

	public LRUCache(int capacity) {
		if (capacity < 1) {
			throw new IllegalArgumentException("capacity must be at least 1");
		}

		this.capacity = capacity;
		this.cache = new LinkedHashMap<Integer, Integer>(capacity, 0.75f, true) {
			@Override
			protected boolean removeEldestEntry(Map.Entry<Integer, Integer> eldest) {
				return size() > LRUCache.this.capacity;
			}
		};
	}

	public void put(int key, int value) {
		cache.put(key, value);
	}

	public int get(int key) {
		Integer value = cache.get(key);
		return value == null ? -1 : value;
	}

	public int size() {
		return cache.size();
	}
}

public class Main {
	public static void main(String[] args) {
        LRUCache cache = new LRUCache(2);

        cache.put(1, 100);
        cache.put(2, 200);

        System.out.println(cache.get(1)); // 100

        cache.put(3, 300);
        // キー2が削除される

        System.out.println(cache.get(2)); // -1
        System.out.println(cache.get(3)); // 300

        cache.put(1, 150);

        System.out.println(cache.get(1)); // 150
        System.out.println(cache.size()); // 2
	}
}
