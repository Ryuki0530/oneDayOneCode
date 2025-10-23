import java.util.Scanner;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String line = sc.nextLine().toLowerCase();
        sc.close();

        String[] words = line.split("\\s+");
        Map<String, Integer> countMap = new HashMap<>();

        for (String w : words) {
            countMap.put(w, countMap.getOrDefault(w, 0) + 1);
        }

        List<String> keys = new ArrayList<>(countMap.keySet());
        Collections.sort(keys);

        for (String k : keys) {
            System.out.println(k + " " + countMap.get(k));
        }
    }
}