import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
	public static void main(String[] args) {
		List<String> logs = Arrays.asList(
				"INFO: Application started",
				"WARN: Low memory",
				"INFO: User login",
				"ERROR: Database connection failed",
				"INFO: File loaded",
				"ERROR: Timeout",
				"WARN: Disk usage high",
				"INFO: Shutdown"
		);

		Map<String, Integer> counts = new LinkedHashMap<>();
		counts.put("INFO", 0);
		counts.put("WARN", 0);
		counts.put("ERROR", 0);

		for (String log : logs) {
			String level = log.substring(0, log.indexOf(':'));
			counts.put(level, counts.get(level) + 1);
		}

		for (Map.Entry<String, Integer> entry : counts.entrySet()) {
			System.out.printf("%-5s: %d%n", entry.getKey(), entry.getValue());
		}
	}
}
