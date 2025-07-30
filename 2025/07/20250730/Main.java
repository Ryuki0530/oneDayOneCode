import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String line = sc.nextLine(); // 1行入力
        sc.close();

        int wordsQuantity = wordCounter(line);

        System.out.println(wordsQuantity);

    }

    public static int wordCounter(String input) {
        if (input == null || input.isEmpty()) {
            return 0;
        }
        String[] words = input.trim().split("\\s+");
        return words.length;
    }
}
