import java.util.HashMap;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class Main {
	public static void main(String[] args) {
		List<String> words = Arrays.asList(
				"apple",
				"banana",
				"apple",
				"orange",
				"banana",
				"apple"
		);

		HashMap<String, Integer> frequencies = new HashMap<>();

		for (String word : words) {
			frequencies.put(word, frequencies.getOrDefault(word, 0) + 1);
		}

		for (Map.Entry<String, Integer> entry : frequencies.entrySet()) {
			System.out.println(entry.getKey() + ": " + entry.getValue());
		}

		String mostFrequent = null;
		int highestCount = 0;
		for (Map.Entry<String, Integer> entry : frequencies.entrySet()) {
			if (entry.getValue() > highestCount) {
				mostFrequent = entry.getKey();
				highestCount = entry.getValue();
			}
		}

		System.out.println("Most frequent: " + mostFrequent);
	}
}
