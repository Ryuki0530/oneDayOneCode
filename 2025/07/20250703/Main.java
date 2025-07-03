//2025-07-03
import java.util.Scanner;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.nio.file.*;

public class Main {
    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String filePath;
        String targetName;
        if (args.length < 2) {
            filePath = "./phonebook.txt";
        }else{
            filePath = args[1];
        }
        System.out.print("Enter the name to search for\n >");
        targetName = sc.nextLine();
        String result = searchNumber(filePath, targetName);
        System.out.println("Result: " + result);
    }

    public static String searchNumber(String filePath, String targetName){
        Path path = Paths.get(filePath);
        try (BufferedReader file = new BufferedReader(new FileReader(path.toFile()))){
            String line;
            while ((line = file.readLine()) != null){
                String[] currentPerson = line.split(" ");
                if (currentPerson[0].equalsIgnoreCase(targetName)){
                    String number = currentPerson[1].toString();
                    return number;
                }
            }
        }catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }
        return "not found";
    }
}