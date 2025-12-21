import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws IOException {
        FastScanner sc = new FastScanner(System.in);
        int n = sc.nextInt();
        Event[] events = new Event[n * 2];
        for (int i = 0; i < n; i++) {
            int start = sc.nextInt();
            int end = sc.nextInt();
            events[i * 2] = new Event(start, 1);
            events[i * 2 + 1] = new Event(end, -1);
        }
        Arrays.sort(events);
        int max = 0, cur = 0;
        for (Event e : events) {
            cur += e.type;
            max = Math.max(max, cur);
        }
        System.out.println(max);
    }

    static class Event implements Comparable<Event> {
        int time, type;
        Event(int time, int type) {
            this.time = time;
            this.type = type;
        }
        public int compareTo(Event o) {
            if (this.time != o.time) return Integer.compare(this.time, o.time);
            return Integer.compare(this.type, o.type);
        }
    }
    static class FastScanner {
        private final InputStream in;
        private final byte[] buf = new byte[1 << 16];
        private int ptr = 0, buflen = 0;
        FastScanner(InputStream in) { this.in = in; }
        private boolean hasNextByte() throws IOException {
            if (ptr < buflen) return true;
            ptr = 0;
            buflen = in.read(buf);
            return buflen > 0;
        }
        private int readByte() throws IOException {
            return hasNextByte() ? buf[ptr++] : -1;
        }
        public int nextInt() throws IOException {
            int c = readByte();
            while (c <= 32) c = readByte();
            int sign = 1;
            if (c == '-') { sign = -1; c = readByte(); }
            int n = 0;
            while ('0' <= c && c <= '9') {
                n = n * 10 + (c - '0');
                c = readByte();
            }
            return n * sign;
        }
    }
}