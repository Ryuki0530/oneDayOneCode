import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.Deque;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String s = br.readLine();
        if (s == null) return;

        Deque<StringBuilder> strStack = new ArrayDeque<>();
        Deque<Integer> countStack = new ArrayDeque<>();

        StringBuilder current = new StringBuilder();
        int number = 0;

        for (int i = 0; i < s.length(); i++) {
            char ch = s.charAt(i);

            if (Character.isDigit(ch)) {
                number = number * 10 + (ch - '0');
            } else if (ch == '(') {
                strStack.push(current);
                countStack.push(number);
                current = new StringBuilder();
                number = 0;
            } else if (ch == ')') {
                int repeat = countStack.pop();
                StringBuilder prev = strStack.pop();
                for (int r = 0; r < repeat; r++) {
                    prev.append(current);
                }
                current = prev;
            } else {
                current.append(ch);
            }
        }

        System.out.println(current.toString());
    }
}