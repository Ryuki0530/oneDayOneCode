import java.nio.file.*;
import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws IOException {
        String input = new String(Files.readAllBytes(Paths.get("input.txt")), "UTF-8");

        String[] words = input.split("[\\s\\p{Punct}]+");

        Map<String, Integer> wordCount = new HashMap<>();
        for (String word : words) {
            if (word.isEmpty()) continue;
            String lower = word.toLowerCase();
            wordCount.put(lower, wordCount.getOrDefault(lower, 0) + 1);
        }

        List<String> sortedKeys = new ArrayList<>(wordCount.keySet());
        Collections.sort(sortedKeys);

        StringBuilder sb = new StringBuilder();
        for (String key : sortedKeys) {
            sb.append(key).append(": ").append(wordCount.get(key)).append(" ");
        }

        Files.write(Paths.get("output.txt"), sb.toString().trim().getBytes("UTF-8"));
    }
}