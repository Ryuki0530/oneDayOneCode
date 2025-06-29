import java.util.Scanner;
public class main {

    // Constants for options
    public static final String OPTION_1 = "A";
    public static final String OPTION_2 = "B";
    public static final String OPTION_3 = "C";
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int participants_num = sc.nextInt();
        System.out.println("Number of participants: " + participants_num);
        int optionPoints[] = new int[3];
        for (int i = 0; i < participants_num; i++) {
            String option = sc.next().toUpperCase();
            
            switch (option) {
                case OPTION_1:
                    optionPoints[0]++;
                    break;
                case OPTION_2:
                    optionPoints[1]++;
                    break;
                case OPTION_3:
                    optionPoints[2]++;
                    break;
                default:
                    // System.out.println(participant_name + " made an invalid choice.");
            }
            
        }
        System.out.println("A: " + optionPoints[0]);
        System.out.println("B: " + optionPoints[1]);
        System.out.println("C: " + optionPoints[2]);
        String Winner;
        int maxPoints = optionPoints[0];
        Winner = OPTION_1;
        if (optionPoints[1] > maxPoints) {
            maxPoints = optionPoints[1];
            Winner = OPTION_2;
        }
        if (optionPoints[2] > maxPoints) {
            Winner = OPTION_3;
        }
        System.out.println("Winner:" + Winner);
        return;
    }
}