import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        long w = sc.nextLong();
        
        Map<String, Queue<Long>> userQueues = new HashMap<>();
        Map<String, Integer> maxAccessCount = new HashMap<>();
        
        for (int i = 0; i < n; i++) {
            long time = sc.nextLong();
            String userId = sc.next();
            
            userQueues.putIfAbsent(userId, new LinkedList<>());
            Queue<Long> queue = userQueues.get(userId);
            
            queue.add(time);
            
            long threshold = time - w + 1;
            while (!queue.isEmpty() && queue.peek() < threshold) {
                queue.poll();
            }
            
            int currentCount = queue.size();
            System.out.println(currentCount);
            
            maxAccessCount.put(userId, Math.max(
                maxAccessCount.getOrDefault(userId, 0),
                currentCount
            ));
        }
        
        int maxCount = maxAccessCount.values().stream().max(Integer::compareTo).orElse(0);
        
        String maxUser = maxAccessCount.entrySet().stream()
            .filter(e -> e.getValue() == maxCount)
            .map(Map.Entry::getKey)
            .min(String::compareTo)
            .orElse("");
        
        System.out.println(maxCount);
        System.out.println(maxUser);
        
        sc.close();
    }
}
