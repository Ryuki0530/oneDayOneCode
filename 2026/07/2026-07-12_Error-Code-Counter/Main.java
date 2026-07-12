import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
	public static void main(String[] args) {
		List<String> logs = Arrays.asList(
				"INFO",
				"ERROR",
				"WARN",
				"ERROR",
				"INFO",
				"ERROR",
				"WARN",
				"ERROR",
				"ERROR"
		);

		Map<String, Integer> counter = new HashMap<>();

		for (String level : logs) {
			counter.put(level, counter.getOrDefault(level, 0) + 1);
		}

		String[] order = {"INFO", "WARN", "ERROR"};
		for (String level : order) {
			System.out.println(level + " : " + counter.getOrDefault(level, 0));
		}

		String mostFrequentLevel = "";
		int maxCount = 0;
		for (String level : counter.keySet()) {
			int count = counter.get(level);
			if (count > maxCount) {
				maxCount = count;
				mostFrequentLevel = level;
			}
		}

		System.out.println("Most frequent : " + mostFrequentLevel + " (" + maxCount + ")");
	}
}
