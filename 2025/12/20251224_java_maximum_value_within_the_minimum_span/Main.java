import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String[] nk = br.readLine().split(" ");
        int N = Integer.parseInt(nk[0]);
        int K = Integer.parseInt(nk[1]);
        int[] A = new int[N];
        String[] aStr = br.readLine().split(" ");
        for (int i = 0; i < N; i++) {
            A[i] = Integer.parseInt(aStr[i]);
        }

        Deque<Integer> dq = new ArrayDeque<>();
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < N; i++) {
            while (!dq.isEmpty() && A[dq.peekLast()] <= A[i]) {
                dq.pollLast();
            }
            dq.addLast(i);

            if (dq.peekFirst() <= i - K) {
                dq.pollFirst();
            }
            if (i >= K - 1) {
                sb.append(A[dq.peekFirst()]);
                if (i != N - 1) sb.append(" ");
            }
        }
        System.out.println(sb.toString().trim());
    }
}