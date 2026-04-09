import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        int n = sc.nextInt();
        int maxStreak = 0;
        int currentStreak = 0;
        
        for (int i = 0; i < n; i++) {
            int login = sc.nextInt();
            
            if (login == 1) {
                currentStreak++;
                maxStreak = Math.max(maxStreak, currentStreak);
            } else {
                currentStreak = 0;
            }
        }
        
        System.out.println(maxStreak);
        sc.close();
    }
}