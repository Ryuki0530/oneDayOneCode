import java.io.*;
import java.util.*;

public class Main {
    static class FastScanner {
        private BufferedReader br;
        private StringTokenizer st;

        FastScanner(InputStream in) {
            br = new BufferedReader(new InputStreamReader(in));
        }

        String next() throws IOException {
            while (st == null || !st.hasMoreTokens()) {
                String line = br.readLine();
                if (line == null) return null;
                st = new StringTokenizer(line);
            }
            return st.nextToken();
        }

        int nextInt() throws IOException {
            return Integer.parseInt(next());
        }

        long nextLong() throws IOException {
            return Long.parseLong(next());
        }
    }

    static class Event {
        long time;
        String id;

        Event(long time, String id) {
            this.time = time;
            this.id = id;
        }
    }

    static void evictOld(Deque<Event> deque, Map<String, Integer> countMap, long threshold) {
        while (!deque.isEmpty() && deque.peekFirst().time <= threshold) {
            Event e = deque.removeFirst();
            int c = countMap.get(e.id) - 1;
            if (c == 0) countMap.remove(e.id);
            else countMap.put(e.id, c);
        }
    }

    public static void main(String[] args) throws IOException {
        FastScanner sc = new FastScanner(System.in);
        String first = sc.next();
        if (first == null) return;

        int Q = Integer.parseInt(first);
        long TTL = sc.nextLong();

        Deque<Event> deque = new ArrayDeque<>();
        Map<String, Integer> countMap = new HashMap<>();

        for (int i = 0; i < Q; i++) {
            String cmd = sc.next();

            if (cmd.equals("ADD")) {
                long t = sc.nextLong();
                String id = sc.next();

                long threshold = t - TTL;
                evictOld(deque, countMap, threshold);

                deque.addLast(new Event(t, id));
                countMap.put(id, countMap.getOrDefault(id, 0) + 1);

            } else {
                long t = sc.nextLong();
                long threshold = t - TTL;

                evictOld(deque, countMap, threshold);

                System.out.println(countMap.size());
            }
        }
    }
}