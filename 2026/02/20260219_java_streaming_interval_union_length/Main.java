import java.io.*;
import java.util.*;

public class Main {
    static class FastScanner {
        BufferedReader br;
        StringTokenizer st;

        FastScanner(InputStream in) {
            br = new BufferedReader(new InputStreamReader(in));
        }

        String next() throws IOException {
            while (st == null || !st.hasMoreTokens()) {
                st = new StringTokenizer(br.readLine());
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

    public static void main(String[] args) throws IOException {
        FastScanner sc = new FastScanner(System.in);
        int Q = sc.nextInt();
        
        TreeMap<Long, Long> intervals = new TreeMap<>();
        long total = 0;
        
        for (int q = 0; q < Q; q++) {
            String cmd = sc.next();
            
            if (cmd.equals("ADD")) {
                long l = sc.nextLong();
                long r = sc.nextLong();
                
                long newStart = l;
                long newEnd = r;
                
                // Find overlapping intervals
                Map.Entry<Long, Long> entry = intervals.floorEntry(l);
                
                // Check if there's an interval that overlaps or touches
                if (entry != null && entry.getValue() >= l) {
                    newStart = entry.getKey();
                    newEnd = Math.max(newEnd, entry.getValue());
                    total -= (entry.getValue() - entry.getKey());
                    intervals.remove(entry.getKey());
                }
                
                // Merge all overlapping intervals to the right
                while (true) {
                    entry = intervals.ceilingEntry(newStart);
                    if (entry == null || entry.getKey() > newEnd) {
                        break;
                    }
                    newEnd = Math.max(newEnd, entry.getValue());
                    total -= (entry.getValue() - entry.getKey());
                    intervals.remove(entry.getKey());
                }
                
                intervals.put(newStart, newEnd);
                total += (newEnd - newStart);
                
            } else if (cmd.equals("LEN")) {
                System.out.println(total);
            }
        }
    }
}