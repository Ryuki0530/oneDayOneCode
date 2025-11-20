import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(reader.readLine());
        List<String> lines = new ArrayList<>(n);
        for (int i = 0; i < n; i++) {
            lines.add(reader.readLine());
        }
        String keyword = reader.readLine();

        StringBuilder output = new StringBuilder();
        for (String line : lines) {
            if (line.contains(keyword)) {
                output.append(line).append(System.lineSeparator());
            }
        }
        System.out.print(output.toString());
    }
}