import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        List<String> lines = new ArrayList<>();

        // 複数行の入力を受け取る
        while (true) {
            String line = scanner.nextLine();
            if (line.equals("END")) {
                break;
            }
            lines.add(line);
        }

        // 逆順に出力
        Collections.reverse(lines);
        for (String line : lines) {
            System.out.println(line);
        }
    }
}
