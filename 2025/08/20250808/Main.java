import java.util.Scanner;
import java.util.Map;

class stringEvaluator{
    private String inputString;
    public stringEvaluator(String string) {
        inputString = string;
    }

    public Map<Character, Integer> countWords() {
        Map<Character, Integer> countMap = new java.util.HashMap<>();

        for (char ch = 'a'; ch <= 'z'; ch++) {
            countMap.put(ch, 0);
        }

        for(char c : inputString.toCharArray()) {
            if (Character.isLetter(c)) {
                c = Character.toLowerCase(c);
                countMap.put(c, countMap.getOrDefault(c, 0) + 1);
            }
        }
        return countMap;
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Please enter a Lines of text (type 'exit' to quit):");
        String input;
        input = sc.nextLine();
        System.out.println("Exiting program.");
    }
}