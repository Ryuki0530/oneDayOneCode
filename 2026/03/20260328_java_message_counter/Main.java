import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        sc.nextLine();
        
        Map<String, String> lastMessage = new HashMap<>();
        Map<String, Integer> messageCount = new HashMap<>();
        
        for (int i = 0; i < n; i++) {
            String line = sc.nextLine();
            String[] parts = line.split(" ", 2);
            String user = parts[0];
            String message = parts[1];
            
            String previous = lastMessage.get(user);
            
            if (previous == null || !previous.equals(message)) {
                messageCount.put(user, messageCount.getOrDefault(user, 0) + 1);
                lastMessage.put(user, message);
            }
        }
        
        List<Map.Entry<String, Integer>> entries = new ArrayList<>(messageCount.entrySet());
        entries.sort((a, b) -> {
            int countCmp = Integer.compare(b.getValue(), a.getValue());
            if (countCmp != 0) return countCmp;
            return a.getKey().compareTo(b.getKey());
        });
        
        for (Map.Entry<String, Integer> entry : entries) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
    }
}