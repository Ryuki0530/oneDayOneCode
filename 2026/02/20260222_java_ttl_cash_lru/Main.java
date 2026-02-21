import java.util.*;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.function.LongSupplier;

class TTLCache<K, V> {
    private static class Entry<V> {
        V value;
        long expiresAtMillis;
        
        Entry(V value, long expiresAtMillis) {
            this.value = value;
            this.expiresAtMillis = expiresAtMillis;
        }
        
        boolean isExpired(long nowMillis) {
            return nowMillis >= expiresAtMillis;
        }
    }
    
    private final int capacity;
    private final long defaultTtlMillis;
    private final LongSupplier nowMillis;
    private final ReentrantReadWriteLock lock = new ReentrantReadWriteLock();
    
    private final LinkedHashMap<K, Entry<V>> cache;
    
    public TTLCache(int capacity, long defaultTtlMillis) {
        this(capacity, defaultTtlMillis, System::currentTimeMillis);
    }
    
    public TTLCache(int capacity, long defaultTtlMillis, LongSupplier nowMillis) {
        this.capacity = capacity;
        this.defaultTtlMillis = defaultTtlMillis;
        this.nowMillis = nowMillis;
        this.cache = new LinkedHashMap<K, Entry<V>>(capacity + 1, 0.75f, true) {
            @Override
            protected boolean removeEldestEntry(Map.Entry<K, Entry<V>> eldest) {
                return size() > TTLCache.this.capacity;
            }
        };
    }
    
    public void put(K key, V value) {
        put(key, value, defaultTtlMillis);
    }
    
    public void put(K key, V value, long ttlMillis) {
        if (ttlMillis <= 0) {
            return;
        }
        
        lock.writeLock().lock();
        try {
            long expiresAt = nowMillis.getAsLong() + ttlMillis;
            cache.put(key, new Entry<>(value, expiresAt));
        } finally {
            lock.writeLock().unlock();
        }
    }
    
    public V get(K key) {
        lock.writeLock().lock();
        try {
            Entry<V> entry = cache.get(key);
            if (entry == null) {
                return null;
            }
            
            long now = nowMillis.getAsLong();
            if (entry.isExpired(now)) {
                cache.remove(key);
                return null;
            }
            
            return entry.value;
        } finally {
            lock.writeLock().unlock();
        }
    }
    
    public int size() {
        lock.readLock().lock();
        try {
            long now = nowMillis.getAsLong();
            return (int) cache.entrySet().stream()
                .filter(e -> !e.getValue().isExpired(now))
                .count();
        } finally {
            lock.readLock().unlock();
        }
    }
    
    public int purgeExpired() {
        lock.writeLock().lock();
        try {
            long now = nowMillis.getAsLong();
            int removed = 0;
            Iterator<Entry<V>> iter = cache.values().iterator();
            while (iter.hasNext()) {
                if (iter.next().isExpired(now)) {
                    iter.remove();
                    removed++;
                }
            }
            return removed;
        } finally {
            lock.writeLock().unlock();
        }
    }
}

class Main {
    public static void main(String[] args) {
        TTLCache<String, String> cache = new TTLCache<>(100, 5000);
        Scanner scanner = new Scanner(System.in);
        
        while (scanner.hasNextLine()) {
            String line = scanner.nextLine().trim();
            if (line.isEmpty()) continue;
            
            String[] parts = line.split("\\s+", 3);
            String cmd = parts[0];
            
            switch (cmd) {
                case "PUT":
                    if (parts.length >= 3) {
                        String key = parts[1];
                        String value = parts[2].split("\\s+")[0];
                        if (parts.length >= 4) {
                            long ttl = Long.parseLong(line.split("\\s+")[3]);
                            cache.put(key, value, ttl);
                        } else {
                            cache.put(key, value);
                        }
                    }
                    break;
                case "GET":
                    if (parts.length >= 2) {
                        String key = parts[1];
                        String result = cache.get(key);
                        System.out.println(result != null ? result : "NULL");
                    }
                    break;
                case "SIZE":
                    System.out.println(cache.size());
                    break;
                case "PURGE":
                    System.out.println(cache.purgeExpired());
                    break;
            }
        }
        scanner.close();
    }
}