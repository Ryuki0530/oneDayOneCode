import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        String csvFile = "employees.csv";
        String line = "";
        String csvSeparator = ",";
        int sum = 0;
        int itr = 0;
        String selected_department = "";
        Scanner sc = new Scanner(System.in);
        System.out.print("•”‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢: ");
        selected_department = sc.nextLine();
        System.out.println("•”u"+selected_department+"v‚ÌŽÐˆõˆê——:");
        try (BufferedReader br = new BufferedReader(new FileReader(csvFile))) {
            while ((line = br.readLine()) != null) {
              
                String[] currentRecord = line.split(csvSeparator);
                
                if (currentRecord.length >= 4 && selected_department.equals(currentRecord[2].trim())) {
                    String employeeId = currentRecord[0].trim();
                    String employeeName = currentRecord[1].trim();
                    int age = Integer.parseInt(currentRecord[3].trim());
                    System.out.println(employeeId + " " + employeeName +" " + age);
                    sum += age;
                    itr++;
                }
            }
            System.out.println("•½‹Ï”N—î: " + (sum > 0 ? (double) sum / itr : 0));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

