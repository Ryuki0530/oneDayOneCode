import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

public class Main {
    static class FastScanner {
        private final BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        private StringTokenizer st;

        String next() throws IOException {
            while (st == null || !st.hasMoreTokens()) {
                String line = br.readLine();
                if (line == null) {
                    return null;
                }
                st = new StringTokenizer(line);
            }
            return st.nextToken();
        }

        int nextInt() throws IOException {
            return Integer.parseInt(next());
        }
    }

    static class Event {
        int time;
        String user;

        Event(int time, String user) {
            this.time = time;
            this.user = user;
        }
    }

    public static void main(String[] args) throws IOException {
        FastScanner fs = new FastScanner();

        int N = fs.nextInt();
        int W = fs.nextInt();

        Deque<Event> queue = new ArrayDeque<>();
        Map<String, Integer> countMap = new HashMap<>();
        StringBuilder out = new StringBuilder();

        for (int i = 0; i < N; i++) {
            int time = fs.nextInt();
            String user = fs.next();

            queue.addLast(new Event(time, user));
            countMap.put(user, countMap.getOrDefault(user, 0) + 1);

            while (!queue.isEmpty() && queue.peekFirst().time <= time - W) {
                Event old = queue.pollFirst();
                int count = countMap.get(old.user) - 1;
                if (count == 0) {
                    countMap.remove(old.user);
                } else {
                    countMap.put(old.user, count);
                }
            }

            out.append(countMap.size()).append('\n');
        }

        System.out.print(out);
    }
}