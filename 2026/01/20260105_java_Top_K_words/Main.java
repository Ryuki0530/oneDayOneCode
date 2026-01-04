import java.util.*;
import java.util.regex.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int K = sc.nextInt();
        sc.nextLine();
        
        Map<String, Integer> countMap = new HashMap<>();
        Map<String, Integer> firstIndexMap = new HashMap<>();
        int wordIndex = 0;
        
        Pattern pattern = Pattern.compile("[A-Za-z]+");
        
        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            Matcher matcher = pattern.matcher(line);
            
            while (matcher.find()) {
                String word = matcher.group().toLowerCase();
                countMap.put(word, countMap.getOrDefault(word, 0) + 1);
                firstIndexMap.putIfAbsent(word, wordIndex++);
            }
        }
        
        List<String> words = new ArrayList<>(countMap.keySet());
        words.sort((w1, w2) -> {
            int cmp = countMap.get(w2).compareTo(countMap.get(w1));
            if (cmp != 0) return cmp;
            return firstIndexMap.get(w1).compareTo(firstIndexMap.get(w2));
        });
        
        for (int i = 0; i < Math.min(K, words.size()); i++) {
            String word = words.get(i);
            System.out.println(word + " " + countMap.get(word));
        }
    }
}