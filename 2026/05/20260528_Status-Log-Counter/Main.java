import java.util.Scanner;

public class Main {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);

		int n = sc.nextInt();
		int ok = 0;
		int warn = 0;
		int error = 0;

		for (int i = 0; i < n; i++) {
			sc.next();
			String status = sc.next();

			if ("OK".equals(status)) {
				ok++;
			} else if ("WARN".equals(status)) {
				warn++;
			} else if ("ERROR".equals(status)) {
				error++;
			}
		}

		System.out.println(ok + " " + warn + " " + error);
		sc.close();
	}
}
