import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int Q = sc.nextInt();
        sc.nextLine();
        
        StringBuilder text = new StringBuilder();
        Stack<Object[]> history = new Stack<>();
        
        for (int i = 0; i < Q; i++) {
            String line = sc.nextLine().trim();
            String[] parts = line.split(" ", 2);
            String command = parts[0];
            
            if (command.equals("TYPE")) {
                String x = parts[1];
                history.push(new Object[]{"TYPE", x.length()});
                text.append(x);
            } else if (command.equals("BACK")) {
                int k = Integer.parseInt(parts[1]);
                String removed = text.substring(text.length() - k);
                history.push(new Object[]{"BACK", removed});
                text.delete(text.length() - k, text.length());
            } else if (command.equals("UNDO")) {
                Object[] prev = history.pop();
                if (prev[0].equals("TYPE")) {
                    int len = (int) prev[1];
                    text.delete(text.length() - len, text.length());
                } else { // "BACK"
                    String restored = (String) prev[1];
                    text.append(restored);
                }
            }
        }
        
        System.out.println(text.toString());
        sc.close();
    }
}