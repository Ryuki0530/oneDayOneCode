import java.math.BigInteger;
import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) throws Exception {
        FastScanner fs = new FastScanner(System.in);
        int n = fs.nextInt();
        BigInteger x = new BigInteger(fs.next());
        BigInteger[] a = new BigInteger[n + 1];
        for (int i = 0; i <= n; i++) a[i] = new BigInteger(fs.next());

        // Horner: r = P(x), d = P'(x)
        BigInteger r = a[n];
        BigInteger d = BigInteger.ZERO;
        for (int i = n - 1; i >= 0; i--) {
            d = d.multiply(x).add(r);
            r = r.multiply(x).add(a[i]);
        }
        System.out.println(r.toString());
        System.out.println(d.toString());
    }

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
            while ((c = read()) <= ' ' && c != -1) {}
            for (; c > ' '; c = read()) sb.append((char) c);
            return sb.toString();
        }

        int nextInt() throws IOException {
            return Integer.parseInt(next());
        }
    }
}

