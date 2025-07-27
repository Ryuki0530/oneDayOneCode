import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String line = sc.nextLine(); // 1行入力
        sc.close();

        String[] tokens = line.split(" ");
        int sum = 0;

        for (String token : tokens) {
            sum += Integer.parseInt(token);
        }

        double average = (double) sum / tokens.length;
        System.out.println("合計: " + sum);
        System.out.println("平均: " + average);
    }
}
