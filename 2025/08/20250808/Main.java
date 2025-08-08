import java.util.Scanner;
import java.util.Map;

class StringEvaluator{
    private String inputString;
    public StringEvaluator(String string) {
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

        StringEvaluator eval = new StringEvaluator(input);
        Map<Character, Integer> countMap= eval.countWords();
        
        for (char ch = 'a'; ch <= 'z'; ch++) {
            System.out.println(ch + ": " + countMap.get(ch));
        }
        sc.close();
    }
}