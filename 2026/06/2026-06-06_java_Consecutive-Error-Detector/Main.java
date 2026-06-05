import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        int N = scanner.nextInt();
        int K = scanner.nextInt();
        
        int consecutiveErrors = 0;
        boolean alert = false;
        
        for (int i = 0; i < N; i++) {
            String status = scanner.next();
            
            if (status.equals("ERROR")) {
                consecutiveErrors++;
                if (consecutiveErrors >= K) {
                    alert = true;
                }
            } else {
                consecutiveErrors = 0;
            }
        }
        
        if (alert) {
            System.out.println("ALERT");
        } else {
            System.out.println("SAFE");
        }
        
        scanner.close();
    }
}
