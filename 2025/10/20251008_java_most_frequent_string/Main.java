import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = Integer.parseInt(sc.nextLine());
        Map<String, Integer> freq = new HashMap<>();
        for (int i = 0; i < n; i++) {
            String s = sc.nextLine().toLowerCase();
            freq.put(s, freq.getOrDefault(s, 0) + 1);
        }
        String ans = null;
        int max = 0;
        for (Map.Entry<String, Integer> entry : freq.entrySet()) {
            String key = entry.getKey();
            int count = entry.getValue();
            if (count > max || (count == max && key.compareTo(ans) < 0)) {
                max = count;
                ans = key;
            }
        }
        System.out.println(ans);
    }
}