import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String line = br.readLine();
        if (line == null || line.trim().isEmpty()) return;
        int N = Integer.parseInt(line.trim());

        String prev = null;
        int count = 0;

        for (int i = 0; i < N; i++) {
            String cmd = br.readLine();
            if (cmd == null) break;
            cmd = cmd.trim();
            if (prev == null) {
                prev = cmd;
                count = 1;
            } else if (prev.equals(cmd)) {
                count++;
            } else {
                System.out.println(prev + " " + count);
                prev = cmd;
                count = 1;
            }
        }

        if (prev != null) {
            System.out.println(prev + " " + count);
        }
    }
}
