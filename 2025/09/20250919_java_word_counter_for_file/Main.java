import java.util.ArrayList;
import java.io.FileReader;
import java.util.List;
import java.io.BufferedReader;
import java.io.IOException;

class WordRecord {
    String word;
    int count;

    public WordRecord(String word, int count) {
        this.word = word;
        this.count = count;
    }
    public String getWord() {
        return word;
    }
    public int getCount() {
        return count;
    }
    public void iterateCount() {
        count++;
    }
}

class FileManager {
    String filePath;

    public FileManager(String filePath) {
        this.filePath = filePath;
    }

    public String getText() {
        StringBuilder sb = new StringBuilder();
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = br.readLine()) != null) {
                sb.append(line).append(" ");
            }
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }
        return sb.toString();
    }
}

class WordCounter {
    public List<WordRecord> wordCount(String text) {
        List<WordRecord> wordList = new ArrayList<>();
        String[] words = text.split("[\\s\\.,!\\?]+");
        for (String word : words) {
            if (!word.isEmpty()) {
                boolean found = false;
                for (WordRecord record : wordList) {
                    if (record.getWord().equalsIgnoreCase(word)) {
                        record.iterateCount();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    wordList.add(new WordRecord(word, 1));
                }
            }
        }
        return wordList;
    }
}

public class Main {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Usage: java Main <file path>");
            return;
        }
        String filePath = args[0];
        FileManager fileManager = new FileManager(filePath);
        WordCounter wordCounter = new WordCounter();

        String inputText = fileManager.getText();
        List<WordRecord> result = wordCounter.wordCount(inputText);

        System.out.println("Word count results:");
        for (WordRecord record : result) {
            System.out.println(record.getWord() + ": " + record.getCount());
        }
    }
}
