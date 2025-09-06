import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter a string to invert: ");
        String original = sc.nextLine();
        String inverted = invertString(original);
        System.out.println("Original: " + original);
        System.out.println("Inverted: " + inverted);
    }

    public static String invertString(String str) {
        StringBuilder inverted = new StringBuilder();
        for (int i = str.length() - 1; i >= 0; i--) {
            inverted.append(str.charAt(i));
        }
        return inverted.toString();
    }
}