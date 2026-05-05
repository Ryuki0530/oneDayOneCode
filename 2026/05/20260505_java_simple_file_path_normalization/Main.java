import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.Deque;

public class Main {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String path = br.readLine();

		Deque<String> dirs = new ArrayDeque<>();
		for (String part : path.split("/")) {
			if (part.isEmpty() || part.equals(".")) {
				continue;
			}
			if (part.equals("..")) {
				if (!dirs.isEmpty()) {
					dirs.removeLast();
				}
			} else {
				dirs.addLast(part);
			}
		}

		StringBuilder sb = new StringBuilder("/");
		boolean first = true;
		for (String dir : dirs) {
			if (!first) {
				sb.append('/');
			}
			sb.append(dir);
			first = false;
		}

		System.out.println(sb.toString());
	}
}
