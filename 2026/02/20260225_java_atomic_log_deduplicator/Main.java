import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int K = sc.nextInt();
        
        HashMap<String, Integer> window = new HashMap<>();
        ArrayDeque<String> queue = new ArrayDeque<>();
        int duplicates = 0;
        
        for (int i = 0; i < N; i++) {
            String id = sc.next();
            
            if (window.getOrDefault(id, 0) > 0) {
                duplicates++;
            }
            
            queue.add(id);
            window.put(id, window.getOrDefault(id, 0) + 1);
            
            if (queue.size() > K) {
                String old = queue.removeFirst();
                int count = window.get(old) - 1;
                if (count == 0) {
                    window.remove(old);
                } else {
                    window.put(old, count);
                }
            }
        }
        
        System.out.println(duplicates);
    }
}
