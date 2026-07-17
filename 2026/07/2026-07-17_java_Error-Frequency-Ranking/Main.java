import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    public static void main(String[] args) {
        List<String> logs = List.of(
                "INFO",
                "ERROR",
                "WARN",
                "ERROR",
                "INFO",
                "ERROR",
                "WARN",
                "DEBUG",
                "INFO",
                "ERROR"
        );

        Map<String, Integer> frequencyMap = new HashMap<>();
        for (String log : logs) {
            frequencyMap.put(log, frequencyMap.getOrDefault(log, 0) + 1);
        }

        List<Map.Entry<String, Integer>> entries = new ArrayList<>(frequencyMap.entrySet());
        entries.sort((a, b) -> {
            int compareByCount = Integer.compare(b.getValue(), a.getValue());
            if (compareByCount != 0) {
                return compareByCount;
            }
            return a.getKey().compareTo(b.getKey());
        });

        for (Map.Entry<String, Integer> entry : entries) {
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }
    }
}
