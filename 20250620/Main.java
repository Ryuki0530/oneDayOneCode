import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int inputEnglish = sc.nextInt();
        int inputMath = sc.nextInt();
        int inputScience = sc.nextInt();

        boolean isEnglishPass = inputEnglish >= 40;
        boolean isMathPass = inputMath >= 40;
        boolean isSciencePass = inputScience >= 40;
        boolean sumPass = (inputEnglish + inputMath + inputScience) >= 200;

        if(!isEnglishPass || !isMathPass || !isSciencePass) {
            System.out.println("不合格(科目不合格)");          
        } else if(!sumPass) {
            System.out.println("不合格(点数不足)");
        } else {
            System.out.println("合格");
        }


        // // debug
        // System.out.println("   Input English: " + inputEnglish);
        // System.out.println("   Input Math: " + inputMath);
        // System.out.println("   Input Science: " + inputScience);
        // System.out.println("   Is English Pass: " + isEnglishPass);
        // System.out.println("   Is Math Pass: " + isMathPass);
        // System.out.println("   Is Science Pass: " + isSciencePass);
        // System.out.println("   Sum Pass: " + sumPass);

        return;
    }
}
