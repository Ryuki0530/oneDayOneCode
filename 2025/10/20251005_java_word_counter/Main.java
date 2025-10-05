import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        if (!sc.hasNextLine()) {
            sc.close();
            return;
        }
        String line = sc.nextLine().toLowerCase();
        sc.close();
        if (line.trim().isEmpty()) return;

        String[] words = line.split(" +");
        Map<String, Integer> countMap = new TreeMap<>();
        for (String word : words) {
            if (word.isEmpty()) continue;
            countMap.put(word, countMap.getOrDefault(word, 0) + 1);
        }
        for (Map.Entry<String, Integer> entry : countMap.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }
    }
}