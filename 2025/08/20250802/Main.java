import java.util.Map;
import java.util.Scanner;
import java.util.HashMap;

public class Main {
    public static void main(String[] args) {
        boolean debug = false;
        if (args.length > 0 && args[0].equals("-debug")) {
            debug = true;
        }
        Map<String, Integer> wordCount = new HashMap<>();
        Scanner sc = new Scanner(System.in);
        StringBuilder sb = new StringBuilder();
        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            sb.append(line).append(" ");
        }
        sc.close();
        String[] words = extractWords(sb.toString());
        
        if (debug) {
            System.out.println("Debug mode is ON. \nExtracted words:");
            for (String word : words) {
                System.out.println("[" + word + "]");
            }
        }
        countWords(words, wordCount);
        if (debug) {
            System.out.println("Word count(before sorting):");
            for (Map.Entry<String, Integer> entry : wordCount.entrySet()) {
                System.out.println(entry.getKey() + ": " + entry.getValue());
            }
        }
        wordCount = sortWordCount(wordCount);
        for (Map.Entry<String, Integer> entry : wordCount.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }

    public static String replaceSymbolsWithSpaces(String input) {
        return input.replaceAll("[^a-zA-Z0-9']", " ")
                .replaceAll("'", "");
    }

    public static String[] extractWords(String line) {
        line = replaceSymbolsWithSpaces(line);
        line = line.toLowerCase();
        String[] words = line.split("\\s+");
        return words;
    }

    public static void countWords(String[] words, Map<String, Integer> wordCount) {
        for (String word : words) {
            if (wordCount.containsKey(word)) {
                wordCount.put(word, wordCount.get(word) + 1);
            } else {
                wordCount.put(word, 1);
            }
        }
    }

    public static Map<String, Integer> sortWordCount(Map<String, Integer> wordCount) {
        return wordCount.entrySet()
            .stream()
            .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
            .collect(java.util.stream.Collectors.toMap(
                Map.Entry::getKey,
                Map.Entry::getValue,
                (e1, e2) -> e1,
                java.util.LinkedHashMap::new));
    }
}
