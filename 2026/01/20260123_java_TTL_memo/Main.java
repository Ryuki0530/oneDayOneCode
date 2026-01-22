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
        long now = System.currentTimeMillis();
        long expiresAt = now + ttlSeconds * 1000L;
        map.put(key, new Entry(value, expiresAt));
    }

    public String get(String key) {
        Entry entry = map.get(key);
        if (entry == null) return null;

        long now = System.currentTimeMillis();
        if (now >= entry.expiresAtMillis) {
            map.remove(key, entry);
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
            if (line.isEmpty()) {
                i--;
                continue;
            }

            StringTokenizer st = new StringTokenizer(line);
            String cmd = st.nextToken();

            if ("PUT".equals(cmd)) {
                String key = st.nextToken();
                long ttl = Long.parseLong(st.nextToken());
                String value = st.nextToken();
                store.put(key, value, ttl);
            }
            else if ("GET".equals(cmd)) {
                String key = st.nextToken();
                String result = store.get(key);
                System.out.println(result != null ? result : "-1");

            }
            else if ("SLEEP".equals(cmd)) {
                long ms = Long.parseLong(st.nextToken());
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
