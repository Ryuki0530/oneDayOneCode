import java.io.*;
import java.util.Arrays;

public class Main {
    private static final class FastScanner {
        private final InputStream in;
        private final byte[] buffer = new byte[1 << 16];
        private int ptr = 0, len = 0;

        FastScanner(InputStream is) {
            this.in = is;
        }

        private int read() throws IOException {
            if (ptr >= len) {
                len = in.read(buffer);
                ptr = 0;
                if (len <= 0) {
                    return -1;
                }
            }
            return buffer[ptr++] & 0xff;
        }

        int nextInt() throws IOException {
            int c;
            while ((c = read()) <= ' ') {
                if (c == -1) {
                    return Integer.MIN_VALUE;
                }
            }
            int sign = 1;
            if (c == '-') {
                sign = -1;
                c = read();
            }
            int val = 0;
            while (c > ' ') {
                val = val * 10 + c - '0';
                c = read();
            }
            return val * sign;
        }
    }

    private static int lowerBound(long[] arr, int n, long target) {
        int l = 0, r = n;
        while (l < r) {
            int m = (l + r) >>> 1;
            if (arr[m] < target) {
                l = m + 1;
            } else {
                r = m;
            }
        }
        return l;
    }

    public static void main(String[] args) throws Exception {
        FastScanner fs = new FastScanner(System.in);
        int N = fs.nextInt();
        int M = fs.nextInt();

        int[] start = new int[N];
        int[] end = new int[N];
        int[] queries = new int[M];

        long[] coords = new long[2 * N + M];
        int pos = 0;

        for (int i = 0; i < N; i++) {
            start[i] = fs.nextInt();
            end[i] = fs.nextInt();
            coords[pos++] = start[i];
            coords[pos++] = end[i];
        }
        for (int i = 0; i < M; i++) {
            queries[i] = fs.nextInt();
            coords[pos++] = queries[i];
        }

        Arrays.sort(coords, 0, pos);

        long[] unique = new long[pos];
        int size = 0;
        for (int i = 0; i < pos; i++) {
            if (i == 0 || coords[i] != coords[i - 1]) {
                unique[size++] = coords[i];
            }
        }

        long[] diff = new long[size];
        for (int i = 0; i < N; i++) {
            diff[lowerBound(unique, size, start[i])] += 1;
            diff[lowerBound(unique, size, end[i])] -= 1;
        }

        long[] concurrency = new long[size];
        long curr = 0;
        long maxK = 0;
        long totalDur = 0;

        for (int i = 0; i < size; i++) {
            curr += diff[i];
            concurrency[i] = curr;

            if (i + 1 < size) {
                long lenSeg = unique[i + 1] - unique[i];
                if (lenSeg > 0) {
                    if (curr > maxK) {
                        maxK = curr;
                        totalDur = lenSeg;
                    } else if (curr == maxK) {
                        totalDur += lenSeg;
                    }
                }
            }
        }

        StringBuilder sb = new StringBuilder();
        sb.append(maxK).append('\n');
        sb.append(totalDur).append('\n');

        for (int i = 0; i < M; i++) {
            long q = queries[i];
            int ub = lowerBound(unique, size, q);

            long val;
            if (ub < size && unique[ub] == q) {
                val = concurrency[ub];
            } else if (ub == 0) {
                val = 0;
            } else {
                val = concurrency[ub - 1];
            }

            sb.append(val).append('\n');
        }

        System.out.print(sb);
    }
}
