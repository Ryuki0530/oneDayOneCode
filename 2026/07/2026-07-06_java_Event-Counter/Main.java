import java.util.Map;
import java.util.Scanner;
import java.util.TreeMap;

public class Main {
	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		Map<String, Integer> counts = new TreeMap<>();

		while (scanner.hasNextLine()) {
			String event = scanner.nextLine().trim();
			if (event.isEmpty()) {
				continue;
			}
			counts.put(event, counts.getOrDefault(event, 0) + 1);
		}

		for (Map.Entry<String, Integer> entry : counts.entrySet()) {
			System.out.println(entry.getKey() + " : " + entry.getValue());
		}
	}
}
