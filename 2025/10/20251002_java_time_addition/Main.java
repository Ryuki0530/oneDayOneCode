import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String time = sc.next();
        long K = sc.nextLong();

        String[] t = time.split(":");
        int h = Integer.parseInt(t[0]);
        int m = Integer.parseInt(t[1]);
        int s = Integer.parseInt(t[2]);

        long base = h * 3600 + m * 60 + s;
        long sum = base + K;

        long days = sum / 86400;
        int sec = (int)(sum % 86400);

        int nh = sec / 3600;
        int nm = (sec % 3600) / 60;
        int ns = sec % 60;

        System.out.println(String.format("%02d:%02d:%02d", nh, nm, ns));
        System.out.println(days);
    }
}