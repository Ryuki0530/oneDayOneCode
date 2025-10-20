import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8));
        String line;
        // read K (skip possible empty lines before K)
        int K = 0;
        while ((line = br.readLine()) != null) {
            line = line.trim();
            if (!line.isEmpty()) {
                try {
                    K = Integer.parseInt(line);
                } catch (NumberFormatException e) {
                    // invalid first line -> treat as 0 and continue (no further processing)
                    K = 0;
                }
                break;
            }
        }

        Aggregator agg = new Aggregator();
        while ((line = br.readLine()) != null) {
            Optional<LogEntry> opt = LogEntry.parse(line);
            if (opt.isPresent()) {
                agg.add(opt.get());
            }
        }

        agg.printResult(K);
    }
}

class LogEntry {
    final String path;
    final int status;
    final long bytes;

    private LogEntry(String path, int status, long bytes) {
        this.path = path;
        this.status = status;
        this.bytes = bytes;
    }

    // Parse a CLF-like line. Return empty if mandatory pieces cannot be extracted.
    public static Optional<LogEntry> parse(String line) {
        if (line == null) return Optional.empty();
        try {
            // Split by quotation marks to extract the request section "METHOD PATH PROTO"
            // This is robust for typical CLF lines.
            String[] qparts = line.split("\"");
            if (qparts.length < 3) return Optional.empty();
            String request = qparts[1].trim();
            String afterReq = qparts[2].trim(); // contains status and bytes (and maybe extra)
            String[] reqTokens = request.split("\\s+");
            if (reqTokens.length < 2) return Optional.empty();
            String path = reqTokens[1];

            String[] tail = afterReq.split("\\s+");
            if (tail.length < 2) return Optional.empty();
            int status = Integer.parseInt(tail[0]);
            long bytes = 0;
            if (!tail[1].equals("-")) {
                try {
                    bytes = Long.parseLong(tail[1]);
                } catch (NumberFormatException e) {
                    bytes = 0;
                }
            }
            return Optional.of(new LogEntry(path, status, bytes));
        } catch (Exception e) {
            return Optional.empty();
        }
    }
}

class Aggregator {
    private long totalRequests = 0;
    private long totalBytes = 0;
    private long c2 = 0, c3 = 0, c4 = 0, c5 = 0;
    private final Map<String, Integer> pathCounts = new HashMap<>();

    public void add(LogEntry e) {
        totalRequests++;
        totalBytes += e.bytes;
        int sc = e.status / 100;
        if (sc == 2) c2++;
        else if (sc == 3) c3++;
        else if (sc == 4) c4++;
        else if (sc == 5) c5++;
        pathCounts.merge(e.path, 1, Integer::sum);
    }

    public void printResult(int K) {
        System.out.println("Requests " + totalRequests);
        System.out.println("Bytes " + totalBytes);
        System.out.println("2xx " + c2);
        System.out.println("3xx " + c3);
        System.out.println("4xx " + c4);
        System.out.println("5xx " + c5);
        System.out.println("Top " + K);

        List<Map.Entry<String, Integer>> list = new ArrayList<>(pathCounts.entrySet());
        list.sort((a, b) -> {
            int cmp = Integer.compare(b.getValue(), a.getValue()); // desc by count
            if (cmp != 0) return cmp;
            return a.getKey().compareTo(b.getKey()); // asc lex for ties
        });

        int printed = 0;
        for (Map.Entry<String, Integer> e : list) {
            if (printed >= K) break;
            System.out.println(e.getKey() + " " + e.getValue());
            printed++;
        }
    }
}