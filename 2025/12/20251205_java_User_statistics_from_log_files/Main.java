import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        Map<String, Integer> userCount = new LinkedHashMap<>();
        Map<String, Integer> statusCount = new LinkedHashMap<>();
        Set<String> erroredUsers = new LinkedHashSet<>();

        String line;
        while ((line = reader.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty()) {
                continue;
            }
            String[] parts = line.split("\\s+");
            if (parts.length < 4) {
                continue;
            }
            String userId = parts[1];
            String status = parts[3];

            userCount.merge(userId, 1, Integer::sum);
            statusCount.merge(status, 1, Integer::sum);

            try {
                int statusCode = Integer.parseInt(status);
                if (statusCode >= 500 && statusCode < 600) {
                    erroredUsers.add(userId);
                }
            } catch (NumberFormatException ignored) {
            }
        }

        int maxCount = 0;
        for (int count : userCount.values()) {
            if (count > maxCount) {
                maxCount = count;
            }
        }
        List<String> mostActive = new ArrayList<>();
        if (maxCount > 0) {
            for (Map.Entry<String, Integer> entry : userCount.entrySet()) {
                if (entry.getValue() == maxCount) {
                    mostActive.add(entry.getKey());
                }
            }
        }

        System.out.println("# User Access Count");
        userCount.forEach((user, count) -> System.out.println(user + " " + count));

        System.out.println();
        System.out.println("# Status Count");
        statusCount.forEach((status, count) -> System.out.println(status + " " + count));

        System.out.println();
        System.out.println("# Most Active");
        System.out.print("Most Active:");
        for (String user : mostActive) {
            System.out.print(" " + user);
        }
        System.out.println();

        System.out.println();
        System.out.println("# Errored Users");
        System.out.print("Errored Users:");
        for (String user : erroredUsers) {
            System.out.print(" " + user);
        }
        System.out.println();
    }
}
