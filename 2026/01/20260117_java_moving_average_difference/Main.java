import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int K = sc.nextInt();
        
        long[] A = new long[N];
        for (int i = 0; i < N; i++) {
            A[i] = sc.nextLong();
        }
        
        long sum = 0;
        List<Long> results = new ArrayList<>();
        
        for (int i = 0; i < N; i++) {
            sum += A[i];
            
            int start = Math.max(0, i - K + 1);
            int windowSize = i - start + 1;
            long avg = sum / windowSize;
            
            results.add(A[i] - avg);
            
            if (i >= K - 1) {
                sum -= A[i - K + 1];
            }
        }
        
        for (int i = 0; i < results.size(); i++) {
            if (i > 0) System.out.print(" ");
            System.out.print(results.get(i));
        }
        System.out.println();
    }
}