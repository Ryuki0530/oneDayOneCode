import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        
        Map<String, Integer> errorCountMap = new HashMap<>();
        for (int i = 0; i < n; i++) {
            String userName = sc.next();
            String errorCode = sc.next();
            errorCountMap.put(errorCode, errorCountMap.getOrDefault(errorCode, 0) + 1);
        }
        
        List<Map.Entry<String, Integer>> entryList = new ArrayList<>(errorCountMap.entrySet());
        
        entryList.sort((a, b) -> {
            if (a.getValue() != b.getValue()) {
                return b.getValue() - a.getValue();
            }
            return a.getKey().compareTo(b.getKey());
        });
        
        for (Map.Entry<String, Integer> entry : entryList) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
    }
}
