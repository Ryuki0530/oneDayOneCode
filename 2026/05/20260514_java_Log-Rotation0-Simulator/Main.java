import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String first = br.readLine();   
        if (first == null || first.isEmpty()) return;
        String[] parts = first.trim().split("\\s+");
        int K = Integer.parseInt(parts[0]);
        int Q = Integer.parseInt(parts[1]);

        List<List<String>> files = new ArrayList<>();
        files.add(new ArrayList<>());

        String line;
        for (int i = 0; i < Q; ) {
            line = br.readLine();
            if (line == null) break;
            line = line.trim();
            if (line.isEmpty()) continue;
            i++;
            if (line.startsWith("ADD")) {
                String[] sp = line.split("\\s+", 2);
                String msg = sp.length > 1 ? sp[1] : "";
                List<String> cur = files.get(files.size() - 1);
                if (cur.size() >= K) {
                    cur = new ArrayList<>();
                    files.add(cur);
                }
                cur.add(msg);
            } else if (line.equals("PRINT")) {
                StringBuilder sb = new StringBuilder();
                for (int idx = 0; idx < files.size(); idx++) {
                    List<String> f = files.get(idx);
                    for (String m : f) {
                        sb.append('[').append(idx + 1).append("] ").append(m).append('\n');
                    }
                }
                System.out.print(sb.toString());
            }
        }
    }
}
