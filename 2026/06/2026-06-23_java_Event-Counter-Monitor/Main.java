import java.util.LinkedHashMap;
import java.util.Map;

public class Main {
	public static void main(String[] args) {
		String[] events = {
			"INFO",
			"WARN",
			"INFO",
			"ERROR",
			"WARN",
			"INFO"
		};

		Map<String, Integer> counts = new LinkedHashMap<>();

		for (String event : events) {
			counts.put(event, counts.getOrDefault(event, 0) + 1);
		}

		String mostFrequentEvent = null;
		int maxCount = 0;

		for (Map.Entry<String, Integer> entry : counts.entrySet()) {
			System.out.println(entry.getKey() + " : " + entry.getValue());
			if (entry.getValue() > maxCount) {
				maxCount = entry.getValue();
				mostFrequentEvent = entry.getKey();
			}
		}

		System.out.println();
		System.out.println("Most Frequent Event : " + mostFrequentEvent);
	}
}
