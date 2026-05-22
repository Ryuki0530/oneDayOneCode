import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringTokenizer st = new StringTokenizer(br.readLine());
        int N = Integer.parseInt(st.nextToken());
        int K = Integer.parseInt(st.nextToken());
        int T = Integer.parseInt(st.nextToken());

        Map<String, ArrayDeque<Integer>> map = new HashMap<>();
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < N; i++) {
            st = new StringTokenizer(br.readLine());
            int time = Integer.parseInt(st.nextToken());
            String user = st.nextToken();

            ArrayDeque<Integer> q = map.computeIfAbsent(user, k -> new ArrayDeque<>());
            while (!q.isEmpty() && q.peekFirst() <= time - T) {
                q.pollFirst();
            }
            if (q.size() < K) {
                sb.append("ALLOW").append('\n');
                q.addLast(time);
            } else {
                sb.append("DENY").append('\n');
            }
        }

        System.out.print(sb.toString());
    }
}
