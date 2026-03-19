import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int Q = sc.nextInt();
        int W = sc.nextInt();
        
        Deque<Integer> events = new ArrayDeque<>();
        
        for (int i = 0; i < Q; i++) {
            String command = sc.next();
            int t = sc.nextInt();
            
            if (command.equals("ADD")) {
                events.addLast(t);
            } else if (command.equals("COUNT")) {

                while (!events.isEmpty() && events.peekFirst() <= t - W) {
                    events.removeFirst();
                }
                System.out.println(events.size());
            }
        }
        
        sc.close();
    }
}