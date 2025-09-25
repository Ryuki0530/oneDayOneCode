import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        FastScanner fs = new FastScanner(System.in);
        int N = fs.nextInt();
        if (N <= 0) {
            return;
        }
        int prev = fs.nextInt();
        long cnt = 1; // 回数はlongにしておくと安全
        StringBuilder sb = new StringBuilder();

        for (int i = 1; i < N; i++) {
            int x = fs.nextInt();
            if (x == prev) {
                cnt++;
            } else {
                if (sb.length() > 0) sb.append(' ');
                sb.append(prev).append(':').append(cnt);
                prev = x;
                cnt = 1;
            }
        }
        // 最後のブロック
        if (sb.length() > 0) sb.append(' ');
        sb.append(prev).append(':').append(cnt);

        System.out.println(sb.toString());
    }

    // 高速入力（空白区切り）
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
        String next() throws IOException {
            StringBuilder sb = new StringBuilder();
            int c;
            // skip spaces
            while ((c = read()) != -1 && c <= ' ') {}
            if (c == -1) return null;
            do {
                sb.append((char)c);
                c = read();
            } while (c != -1 && c > ' ');
            return sb.toString();
        }
        int nextInt() throws IOException { return Integer.parseInt(next()); }
    }
}
