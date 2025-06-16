import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        int [] numCount = new int[10];
        Scanner sc = new Scanner(System.in);
        String input = sc.nextLine();
        boolean isNumeric = input.matches("\\d+");
        if (!isNumeric) {
            System.out.println("Invalid input");
            System.exit(1);
        }
        for (int i = 0; i < input.length(); i++) {
            numCount[input.charAt(i) - '0']++;
        }
        for (int i = 0; i < numCount.length; i++) {
            System.out.println(i + " " + numCount[i]);
        }
    }
}