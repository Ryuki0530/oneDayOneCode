import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    public static void main(String[] args) {
	List<String> words = Arrays.asList(
		"apple", "cat", "banana", "dog", "pen", "orange"
	);
	Map<Integer, List<String>> wordsByLength = new HashMap<>();

	for (String word : words) {
	    wordsByLength
		    .computeIfAbsent(word.length(), key -> new ArrayList<>())
		    .add(word);
	}

	wordsByLength.keySet().stream()
		.sorted()
		.forEach(length ->
			System.out.println(length + ": " + wordsByLength.get(length)));
    }
}
