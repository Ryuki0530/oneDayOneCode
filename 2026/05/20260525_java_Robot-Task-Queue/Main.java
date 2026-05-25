import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.HashSet;
import java.util.StringTokenizer;

public class Main {
	static class Task {
		int id;
		String name;
		int time;

		Task(int id, String name, int time) {
			this.id = id;
			this.name = name;
			this.time = time;
		}
	}

	public static void main(String[] args) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int n = Integer.parseInt(br.readLine().trim());

		ArrayDeque<Task> queue = new ArrayDeque<>();
		HashSet<Integer> ids = new HashSet<>();
		long totalTime = 0;

		StringBuilder out = new StringBuilder();
		for (int i = 0; i < n; i++) {
			String line = br.readLine();
			if (line == null || line.isEmpty()) {
				i--;
				continue;
			}

			StringTokenizer st = new StringTokenizer(line);
			String cmd = st.nextToken();

			if ("ADD".equals(cmd)) {
				int id = Integer.parseInt(st.nextToken());
				String name = st.nextToken();
				int time = Integer.parseInt(st.nextToken());

				if (ids.contains(id)) {
					out.append("DUPLICATE ").append(id).append('\n');
				} else {
					queue.addLast(new Task(id, name, time));
					ids.add(id);
					totalTime += time;
				}
			} else if ("RUN".equals(cmd)) {
				if (queue.isEmpty()) {
					out.append("EMPTY\n");
				} else {
					Task task = queue.removeFirst();
					ids.remove(task.id);
					totalTime -= task.time;
					out.append("RUN ").append(task.id).append(' ')
					   .append(task.name).append(' ').append(task.time).append('\n');
				}
			} else if ("STATUS".equals(cmd)) {
				out.append("STATUS ").append(queue.size()).append(' ').append(totalTime).append('\n');
			}
		}

		System.out.print(out.toString());
	}
}
