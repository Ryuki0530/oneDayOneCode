import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Map;
import java.util.TreeMap;

public class Main {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		TreeMap<String, Integer> userCounts = new TreeMap<>();

		String line;
		while ((line = br.readLine()) != null) {
			line = line.trim();
			if (line.isEmpty()) {
				continue;
			}

			String[] parts = line.split("\\s+");
			String user = parts[0];
			userCounts.put(user, userCounts.getOrDefault(user, 0) + 1);
		}

		StringBuilder sb = new StringBuilder();
		for (Map.Entry<String, Integer> entry : userCounts.entrySet()) {
			sb.append(entry.getKey()).append(' ').append(entry.getValue()).append('\n');
		}

		if (sb.length() > 0) {
			System.out.print(sb);
		}
	}
}
