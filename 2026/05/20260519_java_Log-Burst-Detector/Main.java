import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        FastScanner fs = new FastScanner(System.in);
        int N = fs.nextInt();
        long K = fs.nextLong();
        long[] t = new long[N];
        for (int i = 0; i < N; i++) t[i] = fs.nextLong();

        int r = 0;
        int best = 0;s
        for (int l = 0; l < N; l++) {
            while (r + 1 < N && t[r + 1] - t[l] <= K) r++;
            // ensure r is at least l
            if (r < l) r = l;
            best = Math.max(best, r - l + 1);
        }

        System.out.println(best);
    }

    // fast scanner
    static class FastScanner {
        private final InputStream in;
        private final byte[] buffer = new byte[1 << 16];
        private int ptr = 0, len = 0;

        FastScanner(InputStream is) { in = is; }

        private int read() throws IOException {
            if (ptr >= len) {
                len = in.read(buffer);
                ptr = 0;
                if (len <= 0) return -1;
            }
            return buffer[ptr++];
        }

        long nextLong() throws IOException {
            int c;
            while ((c = read()) <= ' ') {
                if (c == -1) return Long.MIN_VALUE;
            }
            int sign = 1;
            if (c == '-') { sign = -1; c = read(); }
            long val = 0;
            while (c > ' ') {
                val = val * 10 + (c - '0');
                c = read();
            }
            return val * sign;
        }

        int nextInt() throws IOException { return (int) nextLong(); }
    }
}
