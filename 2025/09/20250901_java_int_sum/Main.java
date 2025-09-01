public class Main {
    public static void main(String[] args) {
    java.util.Scanner sc = new java.util.Scanner(System.in);
    int sum = 0;
    while (sc.hasNextInt()) {
        sum += sc.nextInt();
    }
    System.out.println(sum);
}
}
