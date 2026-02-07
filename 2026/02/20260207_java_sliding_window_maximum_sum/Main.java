import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int k = sc.nextInt();
        long[] a = new long[n];
        
        for (int i = 0; i < n; i++) {
            a[i] = sc.nextLong();
        }
        
        long windowSum = 0;
        for (int i = 0; i < k; i++) {
            windowSum += a[i];
        }
        
        long maxSum = windowSum;
        
        for (int i = k; i < n; i++) {
            windowSum = windowSum - a[i - k] + a[i];
            maxSum = Math.max(maxSum, windowSum);
        }
        
        System.out.println(maxSum);
        sc.close();
    }
}