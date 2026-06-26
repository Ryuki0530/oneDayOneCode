import java.util.Map;
import java.util.HashMap;

public class Main {
    public static void main(String[] args) {
        String[] logs = {
            "[INFO] Start",
            "[WARN] Low Memory",
            "[INFO] Connect",
            "[ERROR] File Not Found",
            "[WARN] Retry",
            "[ERROR] Timeout",
            "[INFO] End"
        };
        
        System.out.println("Log Severity Count.");
        countLogSeverity(logs);
        
        String[] logsWithTime = {
            "[2026-06-26 20:10:05][INFO] Start",
            "[2026-06-26 20:10:06][WARN] Low Memory",
            "[2026-06-26 20:10:07][INFO] Connect",
            "[2026-06-26 20:10:08][ERROR] File Not Found",
            "[2026-06-26 20:10:09][WARN] Retry",
            "[2026-06-26 20:10:10][ERROR] Timeout",
            "[2026-06-26 20:10:11][INFO] End"
        };
        System.out.println("\nLog Severity Count with Time.");
        countLogSeverity(logsWithTime);
    }
    
    static void countLogSeverity(String[] logs) {
        Map<String, Integer> map = new HashMap<>();
        
        for (String log : logs) {
            String severity = extractSeverity(log);
            
            if (severity != null) {
                int count = map.getOrDefault(severity, 0);
                map.put(severity, count + 1);
            }
        }
        
        System.out.println("INFO : " + map.getOrDefault("INFO", 0));
        System.out.println("WARN : " + map.getOrDefault("WARN", 0));
        System.out.println("ERROR: " + map.getOrDefault("ERROR", 0));
    }
    
    static String extractSeverity(String log) {
        int lastBracketIndex = log.lastIndexOf(']');
        int secondLastBracketIndex = log.lastIndexOf(']', lastBracketIndex - 1);        
        String level = null;
        
        if (secondLastBracketIndex != -1) {
            level = log.substring(secondLastBracketIndex + 2, lastBracketIndex);
        } else {
            int startIndex = log.indexOf('[');
            int endIndex = log.indexOf(']');
            if (startIndex != -1 && endIndex != -1) {
                level = log.substring(startIndex + 1, endIndex);
            }
        }
        
        if ("INFO".equals(level) || "WARN".equals(level) || "ERROR".equals(level)) {
            return level;
        }
        return null;
    }
}

