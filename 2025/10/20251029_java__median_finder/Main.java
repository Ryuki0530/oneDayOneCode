import java.util.Arrays;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        int n = sc.nextInt();
        
        int[] numbers = new int[n];
        for (int i = 0; i < n; i++) {
            numbers[i] = sc.nextInt();
        }
        
        Arrays.sort(numbers);
        if (n % 2 == 1) {
            System.out.println(numbers[n / 2]);
        } else {
            double median = (numbers[n / 2 - 1] + numbers[n / 2]) / 2.0;
            System.out.println(median);
        }
        
        sc.close();
    }
}