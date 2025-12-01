import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        Map<String, Integer> freq = new HashMap<>();
        String line;
        
        while ((line = br.readLine()) != null) {
            String[] tokens = line.split("\\P{Alnum}+");
            for (String t : tokens) {
                if (t.isEmpty()) continue;
                String w = t.toLowerCase(Locale.ROOT);
                freq.merge(w, 1, Integer::sum);
            }
        }

        List<Map.Entry<String, Integer>> entries = new ArrayList<>(freq.entrySet());

        entries.sort((a, b) -> {
            int cmp = Integer.compare(b.getValue(), a.getValue());
            if (cmp != 0) return cmp;
            return a.getKey().compareTo(b.getKey());
        });

        PrintWriter out = new PrintWriter(new OutputStreamWriter(System.out));
        
        for (Map.Entry<String, Integer> e : entries) {
            out.println(e.getKey() + ": " + e.getValue());
        }
        out.flush();
    }
}