import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringTokenizer st = new StringTokenizer(br.readLine());
        
        int N = Integer.parseInt(st.nextToken());
        int K = Integer.parseInt(st.nextToken());
        
        st = new StringTokenizer(br.readLine());
        int[] A = new int[N];
        for (int i = 0; i < N; i++) {
            A[i] = Integer.parseInt(st.nextToken());
        }
        
        Deque<Integer> dq = new ArrayDeque<>();
        StringBuilder sb = new StringBuilder();
        
        for (int i = 0; i < N; i++) {
            if (!dq.isEmpty() && dq.peekFirst() < i - K + 1) {
                dq.pollFirst();
            }
            
            while (!dq.isEmpty() && A[dq.peekLast()] <= A[i]) {
                dq.pollLast();
            }
            
            dq.addLast(i);
            
            if (i >= K - 1) {
                if (sb.length() > 0) sb.append(" ");
                sb.append(A[dq.peekFirst()]);
            }
        }
        
        System.out.println(sb.toString());
    }
}