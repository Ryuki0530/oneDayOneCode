import java.util.Scanner;

public class Main {

    static final String symmetricChars = "AHIMOTUVWXY";
    public static void main(String[] args) {

        boolean isSymmetricChars = true;
        boolean isSymmetricString = false;
        Scanner sc = new Scanner(System.in);
        
        String input = sc.nextLine();
        String reversedInput = new StringBuilder(input).reverse().toString();

        for (char c : input.toCharArray()) {
            if (symmetricChars.indexOf(c) == -1) {
                isSymmetricChars = false;
                break;
            }
        }

        if(input.equals(reversedInput))isSymmetricString = true;  
        
        if (isSymmetricChars && isSymmetricString) {
            System.out.println("YES");
        } else {
            System.out.println("NO");
        }

        //Debug
        // System.out.println("Input: " + input);
        // System.out.println("Reversed Input: " + reversedInput); 
        // System.out.println("Is Symmetric Chars: " + isSymmetricChars);
        // System.out.println("Is Symmetric String: " + isSymmetricString);
    }

}