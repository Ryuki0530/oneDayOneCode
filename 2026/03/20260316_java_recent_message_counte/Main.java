import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String[] nt = br.readLine().split(" ");
        int N = Integer.parseInt(nt[0]);
        int T = Integer.parseInt(nt[1]);

        Map<String, Deque<Integer>> userTimes = new HashMap<>();

        for (int i = 0; i < N; i++) {
            String[] line = br.readLine().split(" ");
            int time = Integer.parseInt(line[0]);
            String user = line[1];

            Deque<Integer> dq = userTimes.computeIfAbsent(user, k -> new ArrayDeque<>());
            while (!dq.isEmpty() && dq.peekFirst() < time - T) {
                dq.pollFirst();
            }
            dq.addLast(time);
            System.out.println(dq.size());
        }
    }
}