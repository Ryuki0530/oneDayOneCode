import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        int n = scanner.nextInt();
        long timeoutMs = scanner.nextLong();
        
        HashMap<String, Long> lastTimestamp = new HashMap<>();
        HashMap<String, Integer> ecuCount = new HashMap<>();
        List<String> timeoutResults = new ArrayList<>();
        
        for (int i = 0; i < n; i++) {
            long timestamp = scanner.nextLong();
            String ecuName = scanner.next();
            
            if (lastTimestamp.containsKey(ecuName)) {
                long previousTimestamp = lastTimestamp.get(ecuName);
                long interval = timestamp - previousTimestamp;
                
                if (interval > timeoutMs) {
                    timeoutResults.add(String.format("TIMEOUT %s %d %d %d", 
                        ecuName, previousTimestamp, timestamp, interval));
                }
            }
            
            lastTimestamp.put(ecuName, timestamp);
            ecuCount.put(ecuName, ecuCount.getOrDefault(ecuName, 0) + 1);
        }
        
        if (timeoutResults.isEmpty()) {
            System.out.println("OK");
        } else {
            for (String result : timeoutResults) {
                System.out.println(result);
            }
        }
        
        for (String ecuName : ecuCount.keySet()) {
            System.out.println(String.format("COUNT %s %d", ecuName, ecuCount.get(ecuName)));
        }
        
        scanner.close();
    }
}
