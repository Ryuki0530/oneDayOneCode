import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Main {    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in, "UTF-8"));
        String line = br.readLine();
        int count = 0;
        if (line != null) {
            for (int i = 0; i < line.length(); i++) {
                char c = line.charAt(i);
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    count++;
                }
            }
        }
        System.out.println("‰pŽš‚ÌŒÂ”: " + count);
    }
}