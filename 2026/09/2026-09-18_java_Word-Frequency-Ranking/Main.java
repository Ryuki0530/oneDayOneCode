import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;

public class Main {
	public static void main(String[] args) {
		List<String> words = Arrays.asList(
				"apple",
				"banana",
				"apple",
				"orange",
				"banana",
				"apple",
				"grape",
				"orange"
		);

		HashMap<String, Integer> frequency = new HashMap<>();
		for (String word : words) {
			frequency.put(word, frequency.getOrDefault(word, 0) + 1);
		}

		List<String> rankedWords = new ArrayList<>(frequency.keySet());
		Collections.sort(rankedWords, new Comparator<String>() {
			@Override
			public int compare(String first, String second) {
				int countComparison = Integer.compare(
						frequency.get(second), frequency.get(first));
				return countComparison != 0
						? countComparison
						: first.compareTo(second);
			}
		});

		for (String word : rankedWords) {
			System.out.println(word + ": " + frequency.get(word));
		}
	}
}
