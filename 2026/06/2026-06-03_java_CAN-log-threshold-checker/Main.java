import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Main {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int n = Integer.parseInt(br.readLine().trim());
		int warnCount = 0;

		StringBuilder out = new StringBuilder();
		for (int i = 0; i < n; i++) {
			String line = br.readLine();
			while (line != null && line.trim().isEmpty()) {
				line = br.readLine();
			}
			if (line == null) {
				break;
			}

			StringTokenizer st = new StringTokenizer(line);
			int id = Integer.parseInt(st.nextToken());
			int speed = Integer.parseInt(st.nextToken());

			if (id != 100) {
				continue;
			}

			if (speed >= 60) {
				out.append("WARN ").append(speed).append('\n');
				warnCount++;
			} else {
				out.append("INFO ").append(speed).append('\n');
			}
		}

		out.append("WARN_COUNT ").append(warnCount).append('\n');
		System.out.print(out);
	}
}
