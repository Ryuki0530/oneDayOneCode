import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        FastScanner fs = new FastScanner(System.in);

        int n = Integer.parseInt(fs.next());
        Map<String, List<Integer>> userTimes = new HashMap<>();

        for (int i = 0; i < n; i++) {
            String time = fs.next();
            String user = fs.next();

            int minutes = toMinutes(time);
            userTimes.computeIfAbsent(user, k -> new ArrayList<>()).add(minutes);
        }

        List<String> users = new ArrayList<>(userTimes.keySet());
        Collections.sort(users);

        StringBuilder sb = new StringBuilder();
        for (String user : users) {
            List<Integer> times = userTimes.get(user);
            Collections.sort(times);

            int maxCount = maxIn10Minutes(times);
            sb.append(user).append(' ').append(maxCount).append('\n');
        }

        System.out.print(sb.toString());
    }

    private static int toMinutes(String hhmm) {
        int h = (hhmm.charAt(0) - '0') * 10 + (hhmm.charAt(1) - '0');
        int m = (hhmm.charAt(3) - '0') * 10 + (hhmm.charAt(4) - '0');
        return h * 60 + m;
    }

    private static int maxIn10Minutes(List<Integer> times) {
        int left = 0;
        int max = 0;

        for (int right = 0; right < times.size(); right++) {
            while (times.get(right) - times.get(left) >= 10) {
                left++;
            }
            max = Math.max(max, right - left + 1);
        }
        return max;
    }

    private static class FastScanner {
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
                if (len <= 0) return -1;
            }
            return buffer[ptr++];
        }

        String next() throws IOException {
            StringBuilder sb = new StringBuilder();
            int c;
            do {
                c = read();
            } while (c <= ' ' && c != -1);

            while (c > ' ') {
                sb.append((char) c);
                c = read();
            }
            return sb.toString();
        }
    }
}