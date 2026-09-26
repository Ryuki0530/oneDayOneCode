import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    public static void main(String[] args) {
        String[] words = {"apple", "cat", "banana", "dog", "grape", "orange", "ant"};
        Map<Integer, List<String>> wordsByLength = new TreeMap<>();

        for (String word : words) {
            int length = word.length();
            wordsByLength
                    .computeIfAbsent(length, key -> new ArrayList<>())
                    .add(word);
        }

        for (Map.Entry<Integer, List<String>> entry : wordsByLength.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }
}