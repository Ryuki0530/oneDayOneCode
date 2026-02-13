import java.util.*;
public class Main {
    static class LogEntry implements Comparable<LogEntry> {
        int timestamp;
        int sourceId;
        int indexInSource;
        String message;

        LogEntry(int timestamp, int sourceId, int indexInSource, String message) {
            this.timestamp = timestamp;
            this.sourceId = sourceId;
            this.indexInSource = indexInSource;
            this.message = message;
        }

        @Override
        public int compareTo(LogEntry other) {
            if (this.timestamp != other.timestamp) {
                return Integer.compare(this.timestamp, other.timestamp);
            }
            if (this.sourceId != other.sourceId) {
                return Integer.compare(this.sourceId, other.sourceId);
            }
            return Integer.compare(this.indexInSource, other.indexInSource);
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int K = sc.nextInt();

        List<List<LogEntry>> logs = new ArrayList<>();
        for (int i = 0; i < K; i++) {
            logs.add(new ArrayList<>());
        }

        for (int sourceId = 0; sourceId < K; sourceId++) {
            int N = sc.nextInt();
            for (int idx = 0; idx < N; idx++) {
                int t = sc.nextInt();
                String message = sc.next();
                logs.get(sourceId).add(new LogEntry(t, sourceId, idx, message));
            }
        }

        PriorityQueue<LogEntry> pq = new PriorityQueue<>();
        int[] pointers = new int[K];

        for (int i = 0; i < K; i++) {
            if (!logs.get(i).isEmpty()) {
                pq.offer(logs.get(i).get(0));
            }
        }

        while (!pq.isEmpty()) {
            LogEntry current = pq.poll();
            System.out.println(current.sourceId + " " + current.timestamp + " " + current.message);

            int sourceId = current.sourceId;
            int nextIndex = current.indexInSource + 1;

            if (nextIndex < logs.get(sourceId).size()) {
                pq.offer(logs.get(sourceId).get(nextIndex));
            }
        }

        sc.close();
    }
}