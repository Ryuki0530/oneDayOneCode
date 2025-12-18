import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        String s = reader.readLine();
        StringBuilder result = new StringBuilder();
        char prev = s.charAt(0);
        int count = 1;
        for (int i = 1; i < s.length(); i++) {
            char current = s.charAt(i);
            if (current == prev) {
                count++;
            } else {
                result.append(prev).append(count);
                prev = current;
                count = 1;
            }
        }
        result.append(prev).append(count);
        System.out.print(result.toString());
    }
}