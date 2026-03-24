import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.Map;
import java.util.TreeMap;

class OneLogEntry {
    String userName;
    String action;
    
    public OneLogEntry(String userName, String action) {
        this.userName = userName;
        this.action = action;
    }
}

class LogSummary {
    Map<String, Integer> users;
    Map<String, Integer> actions;
    int totalCount;
    
    public LogSummary() {
        users = new TreeMap<>();
        actions = new TreeMap<>();
        totalCount = 0;
    }
    
    public void addLogEntry(OneLogEntry entry) {
        users.put(entry.userName, users.getOrDefault(entry.userName, 0) + 1);
        actions.put(entry.action, actions.getOrDefault(entry.action, 0) + 1);
        totalCount++;
    }
    
    public void printSummary() {
        System.out.println("TOTAL " + totalCount);
        System.out.println("USERS");
        for (Map.Entry<String, Integer> entry : users.entrySet()) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
        System.out.println("ACTIONS");
        for (Map.Entry<String, Integer> entry : actions.entrySet()) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
    }
}

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        LogSummary summary = new LogSummary();
        
        String line;
        while ((line = reader.readLine()) != null && !line.isEmpty()) {
            String[] parts = line.split(" ");
            if (parts.length == 2) {
                OneLogEntry entry = new OneLogEntry(parts[0], parts[1]);
                summary.addLogEntry(entry);
            }
        }
        
        summary.printSummary();
    }
}