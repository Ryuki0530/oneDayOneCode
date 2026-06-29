import java.util.HashMap;
import java.util.Map;

public class Main {
	public static void main(String[] args) {
		String[] logs = {
				"INFO:Start",
				"WARN:Low memory",
				"INFO:Login",
				"ERROR:File not found",
				"INFO:Logout",
				"WARN:High CPU"
		};

		Map<String, Integer> counts = new HashMap<>();

		for (String log : logs) {
			String level = log.split(":", 2)[0];
			counts.put(level, counts.getOrDefault(level, 0) + 1);
		}

		String[] levels = {"INFO", "WARN", "ERROR"};
		for (String level : levels) {
			System.out.println(level + " : " + counts.getOrDefault(level, 0));
		}
	}
}
