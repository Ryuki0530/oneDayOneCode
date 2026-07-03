import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
	public static void main(String[] args) {
		List<String> logs = List.of(
			"E101",
			"E404",
			"E101",
			"E500",
			"E404",
			"E404",
			"E101",
			"E301",
			"E500",
			"E404"
		);

		printTopErrors(logs);
	}

	private static void printTopErrors(List<String> logs) {
		Map<String, Integer> counts = new HashMap<>();
		for (String log : logs) {
			counts.put(log, counts.getOrDefault(log, 0) + 1);
		}

		int total = logs.size();
		List<Map.Entry<String, Integer>> entries = new ArrayList<>(counts.entrySet());
		entries.sort(
			Comparator.<Map.Entry<String, Integer>>comparingInt(Map.Entry::getValue)
				.reversed()
				.thenComparing(Map.Entry::getKey)
		);

		for (Map.Entry<String, Integer> entry : entries) {
			double rate = total == 0 ? 0.0 : entry.getValue() * 100.0 / total;
			System.out.printf("%s : %d (%.1f%%)%n", entry.getKey(), entry.getValue(), rate);
		}
	}
}
