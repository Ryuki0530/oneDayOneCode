import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        
        int total = 0;
        Map<String, Integer> userCount = new TreeMap<>();
        Map<String, Integer> actionCount = new TreeMap<>();
        
        String line;
        while ((line = br.readLine()) != null) {
            line = line.trim();

            if (line.isEmpty()) {
                continue;
            }

            String[] parts = line.split("\\s+");

            if (parts.length < 3) {
                continue;
            }

            String userId = parts[1];
            String action = parts[2];
            
            total++;
            userCount.put(userId, userCount.getOrDefault(userId, 0) + 1);
            actionCount.put(action, actionCount.getOrDefault(action, 0) + 1);
        }
        
        System.out.println("TOTAL " + total);
        
        System.out.println("USERS");
        for (Map.Entry<String, Integer> entry : userCount.entrySet()) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
        
        System.out.println("ACTIONS");
        for (Map.Entry<String, Integer> entry : actionCount.entrySet()) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
    }
}