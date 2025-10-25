import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        int lineCount = 0;
        int wordCount = 0;
        int charCount = 0;
        Map<String, Integer> wordFrequency = new HashMap<>();
        
        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            lineCount++;
            charCount += line.length() + 1;

            String[] words = line.split("\\s+");
            for (String word : words) {
                String cleanWord = word.replaceAll("[^A-Za-z]", "");
                
                if (!cleanWord.isEmpty()) {
                    wordCount++;
                    String lowerWord = cleanWord.toLowerCase();
                    wordFrequency.put(lowerWord, wordFrequency.getOrDefault(lowerWord, 0) + 1);
                }
            }
        }
        
        if (charCount > 0) {
            charCount--;
        }
        
        String mostFrequentWord = "";
        int maxCount = 0;
        for (Map.Entry<String, Integer> entry : wordFrequency.entrySet()) {
            if (entry.getValue() > maxCount) {
                mostFrequentWord = entry.getKey();
                maxCount = entry.getValue();
            }
        }
        
        System.out.println("行数: " + lineCount);
        System.out.println("単語数: " + wordCount);
        System.out.println("文字数: " + charCount);
        if (!mostFrequentWord.isEmpty()) {
            System.out.println("最も多く出現した単語: " + mostFrequentWord + " (" + maxCount + "回)");
        }
        
        scanner.close();
    }
}