import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayDeque;

public class Main {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int n = Integer.parseInt(br.readLine().trim());
		ArrayDeque<String> stack = new ArrayDeque<>();
		StringBuilder sb = new StringBuilder();

		for (int i = 0; i < n; i++) {
			String line = br.readLine();
			String[] parts = line.split(" ", 2);

			switch (parts[0]) {
				case "PUSH":
					stack.push(parts[1]);
					break;
				case "POP":
					if (!stack.isEmpty()) {
						stack.pop();
					}
					break;
				case "TOP":
					sb.append(stack.isEmpty() ? "EMPTY" : stack.peek()).append('\n');
					break;
				default:
					break;
			}
		}

		System.out.print(sb.toString());
	}
}
