import java.util.ArrayList;
import java.io.FileReader;
import java.util.List;

class wordRecord{
    String word;
    int count;

    public wordRecord(String word, int count) {
        this.word = word;
        this.count = count;
    }
    public String getWord() {
        return word;
    }
    public void iterateCount() {
        count++;
    }
}
class FileManager {
    // Methods for file operations
    FileReader fileReader;

    public FileManager(String filePath) {

        // Initialize file operations
        System.out.println("FileManager initialized with file: " + filePath);
        try {
            fileReader = new FileReader(filePath);
            System.out.println("File opened successfully.");
            fileReader.close();
        } catch (java.io.IOException e) {
            System.out.println("Failed to open file: " + e.getMessage());
        }
    }

    public String getText() {
        // Placeholder for file reading logic
        return "Sample text from file.";
    }
}
class WordCounter {
    // Methods for word counting
    public WordCounter() {
        // Initialize word counting operations
        System.out.println("WordCounter initialized.");
    }

    public List<wordRecord> wordCount( String text ) {
        List<wordRecord> wordList = new ArrayList<>();
        String[] words = text.split("[\\s\\.,!\\?]+");
        for (String word : words) {
            if (!word.isEmpty()) {
                boolean found = false;
                for (wordRecord record : wordList) {
                    if (record.getWord().equalsIgnoreCase(word)) {
                        record.iterateCount();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    wordList.add(new wordRecord(word, 1));
                }
            }
        }
    
        return wordList;
    }
}
public class Main {
    public static void main(String[] args) {
        FileManager fileManager = new FileManager("path/to/file.txt");
        WordCounter wordCounter = new WordCounter();

        String inputText = fileManager.getText();

    }
}
