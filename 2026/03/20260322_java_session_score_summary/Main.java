import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        scanner.nextLine();
        
        Map<String, Integer> scoreMap = new HashMap<>();
        
        for (int i = 0; i < n; i++) {
            String line = scanner.nextLine();
            String[] parts = line.split(" ");
            String userName = parts[0];
            int score = Integer.parseInt(parts[1]);
            
            scoreMap.put(userName, scoreMap.getOrDefault(userName, 0) + score);
        }
        
        List<Map.Entry<String, Integer>> entryList = new ArrayList<>(scoreMap.entrySet());
        
        entryList.sort((a, b) -> {
            if (!a.getValue().equals(b.getValue())) {
                return b.getValue() - a.getValue();
            }
            return a.getKey().compareTo(b.getKey());
        });
        
        for (Map.Entry<String, Integer> entry : entryList) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
        
        scanner.close();
    }
}