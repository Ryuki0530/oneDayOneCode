import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

class TTLStore {
    private static class Entry {
        String value;
        long expiresAtMillis;
        
        Entry(String value, long expiresAtMillis) {
            this.value = value;
            this.expiresAtMillis = expiresAtMillis;
        }
    }
    
    private final ConcurrentHashMap<String, Entry> map = new ConcurrentHashMap<>();
    
    public void put(String key, String value, long ttlSeconds) {
        long expiresAt = System.currentTimeMillis() + ttlSeconds * 1000;
        map.put(key, new Entry(value, expiresAt));
    }
    
    public String get(String key) {
        Entry entry = map.get(key);
        if (entry == null) {
            return null;
        }
        if (System.currentTimeMillis() >= entry.expiresAtMillis) {
            return null;
        }
        return entry.value;
    }
    
    public void cleanup() {
        long now = System.currentTimeMillis();
        map.entrySet().removeIf(e -> now >= e.getValue().expiresAtMillis);
    }
}

public class Main {
    public static void main(String[] args) {
        TTLStore store = new TTLStore();
        Scanner sc = new Scanner(System.in);
        
        int Q = sc.nextInt();
        sc.nextLine();
        
        for (int i = 0; i < Q; i++) {
            String line = sc.nextLine().trim();
            String[] parts = line.split(" ", 3);
            String cmd = parts[0];
            
            if ("PUT".equals(cmd)) {
                String key = parts[1];
                long ttl = Long.parseLong(parts[2]);
                String value = parts.length > 3 ? parts[3] : "";
                store.put(key, value, ttl);
            }
            else if ("GET".equals(cmd)) {
                String key = parts[1];
                String result = store.get(key);
                System.out.println(result != null ? result : "-1");
            
            }
            else if ("SLEEP".equals(cmd)) {
                long ms = Long.parseLong(parts[1]);
                try {
                    Thread.sleep(ms);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }

            }
            else if ("CLEANUP".equals(cmd)) {
                store.cleanup();
            }
        }
        
        sc.close();
    }
}