import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String input = scanner.nextLine();
        
        if (input.isEmpty()) {
            System.out.println("•¶Žš‚ª“ü—Í‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
            return;
        }
        
        Map<Character, Integer> charCount = new HashMap<>();
        for (char c : input.toCharArray()) {
            charCount.put(c, charCount.getOrDefault(c, 0) + 1);
        }
        
        List<Map.Entry<Character, Integer>> entries = new ArrayList<>(charCount.entrySet());
        entries.sort((a, b) -> {
            int countCompare = b.getValue().compareTo(a.getValue());
            if (countCompare != 0) {
                return countCompare;
            }
            return a.getKey().compareTo(b.getKey());
        });
        
        for (Map.Entry<Character, Integer> entry : entries) {
            char c = entry.getKey();
            int count = entry.getValue();
            
            if (c == ' ') {
                System.out.println("[space]: " + count);
            } else {
                System.out.println(c + ": " + count);
            }
        }
    }
}