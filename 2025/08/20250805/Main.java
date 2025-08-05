import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("日付を入力してください (YYYY/MM/DD):");
        String input = sc.nextLine();
        String output = changeDateFormat(input);
        System.out.println(output);
    }

    public static String changeDateFormat(String date) {
        if (date.matches("\\d{4}/\\d{2}/\\d{2}")) {
            String[] parts = date.split("/");
            int year = Integer.parseInt(parts[0]);
            int month = Integer.parseInt(parts[1]);
            int day = Integer.parseInt(parts[2]);
            String output;

            if (month < 1 || month > 12) {
                output = "不正な日付です";
                return output;
            }

            int[] daysInMonth = {31, (isLeapYear(year) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (day < 1 || day > daysInMonth[month - 1]) {
                output = "不正な日付です";
            } else {
                output = "%04d年%02d月%02d日".formatted(year, month, day);
            }

            return output;
        } else {
            return "不正な日付です";
        }
    }

    public static boolean isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
}