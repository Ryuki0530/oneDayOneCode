import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class Main {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int n = Integer.parseInt(br.readLine().trim());

		Map<String, String> latestStatus = new HashMap<>();
		for (int i = 0; i < n; i++) {
			String[] parts = br.readLine().split(" ");
			latestStatus.put(parts[0], parts[1]);
		}

		ArrayList<String> errorDevices = new ArrayList<>();
		for (Map.Entry<String, String> entry : latestStatus.entrySet()) {
			if ("ERROR".equals(entry.getValue())) {
				errorDevices.add(entry.getKey());
			}
		}

		if (errorDevices.isEmpty()) {
			System.out.println("ALL_OK");
			return;
		}

		Collections.sort(errorDevices);
		StringBuilder sb = new StringBuilder();
		for (String device : errorDevices) {
			sb.append(device).append('\n');
		}
		System.out.print(sb.toString());
	}
}
