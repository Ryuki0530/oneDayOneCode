import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        long S = sc.nextLong();
        int[] A = new int[N];
        for (int i = 0; i < N; i++) {
            A[i] = sc.nextInt();
        }
        
        int minLength = Integer.MAX_VALUE;
        long sum = 0;
        int left = 0;
        
        for (int right = 0; right < N; right++) {
            sum += A[right];
            
            while (sum >= S) {
                minLength = Math.min(minLength, right - left + 1);
                sum -= A[left];
                left++;
            }
        }
        
        System.out.println(minLength == Integer.MAX_VALUE ? 0 : minLength);
    }
}