import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Main {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String s = br.readLine();
		if (s == null) {
			return;
		}

		int current = 0;
		int max = 0;
		for (int i = 0; i < s.length(); i++) {
			if (s.charAt(i) == 'E') {
				current++;
				if (current > max) {
					max = current;
				}
			} else {
				current = 0;
			}
		}

		if (max >= 4) {
			System.out.println("CRITICAL");
		} else if (max >= 3) {
			System.out.println("ALERT");
		} else {
			System.out.println("OK");
		}
	}
}
