import java.util.Scanner;
import java.util.HashMap;
import java.util.Map;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Map <String, String> aliasMap = new HashMap<>();

        int N = sc.nextInt();
        sc.nextLine(); // Consume the newline

        for (int i = 0; i < N; i++) {
            String inputLine = sc.nextLine().trim();   
            String[] parts = inputLine.split(" ", 2);
            String alias = parts[0];
            String command = parts[1];
            aliasMap.put(alias, command);
        }

        int Q = sc.nextInt();
        sc.nextLine(); // Consume the newline

        for (int i = 0; i < Q; i++) {
            String InputLine = sc.nextLine().trim();
            String expandedCommand = expandedCommandBuilder(InputLine, aliasMap);
            System.out.println(expandedCommand);
        }
        sc.close();
    }

    private static String expandedCommandBuilder(String inputString, Map<String, String> aliasMap) {
        String[] parts = inputString.split(" ", 2);
            String alias = parts[0];
            String expandedCommand;
            if (!aliasMap.containsKey(alias)) {
                return inputString;
            }
            if (parts.length > 1) {
                expandedCommand = aliasMap.get(alias) + " " + parts[1];
                return expandedCommand;
            } else {
                expandedCommand = aliasMap.get(alias);
            }
        return expandedCommand;
    }
}