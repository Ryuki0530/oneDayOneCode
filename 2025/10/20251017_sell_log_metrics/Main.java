import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8));
        TreeMap<String, Long> dailyTotals = new TreeMap<>();
        HashMap<String, Long> categoryTotals = new HashMap<>();

        String line;
        while ((line = reader.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty()) {
                continue;
            }

            String[] parts = line.split(",", 3);
            if (parts.length != 3) {
                continue;
            }

            String date = parts[0].trim();
            String category = parts[1].trim();
            String amountStr = parts[2].trim();

            if (!isValidDate(date) || !isValidCategory(category)) {
                continue;
            }

            long amount;
            try {
                amount = Long.parseLong(amountStr);
            } catch (NumberFormatException e) {
                continue;
            }

            dailyTotals.merge(date, amount, Long::sum);
            categoryTotals.merge(category, amount, Long::sum);
        }

        StringBuilder output = new StringBuilder();
        output.append("# daily\n");
        for (Map.Entry<String, Long> entry : dailyTotals.entrySet()) {
            output.append(entry.getKey()).append(',').append(entry.getValue()).append('\n');
        }

        output.append("# category\n");
        List<Map.Entry<String, Long>> categoryList = new ArrayList<>(categoryTotals.entrySet());
        categoryList.sort(new Comparator<Map.Entry<String, Long>>() {
            @Override
            public int compare(Map.Entry<String, Long> a, Map.Entry<String, Long> b) {
                int cmp = Long.compare(b.getValue(), a.getValue());
                if (cmp != 0) {
                    return cmp;
                }
                return a.getKey().compareTo(b.getKey());
            }
        });
        for (Map.Entry<String, Long> entry : categoryList) {
            output.append(entry.getKey()).append(',').append(entry.getValue()).append('\n');
        }

        System.out.print(output.toString());
    }

    private static boolean isValidDate(String date) {
        if (date.length() != 10) {
            return false;
        }
        for (int i = 0; i < 10; i++) {
            char ch = date.charAt(i);
            if (i == 4 || i == 7) {
                if (ch != '-') {
                    return false;
                }
            } else if (!Character.isDigit(ch)) {
                return false;
            }
        }
        return true;
    }

    private static boolean isValidCategory(String category) {
        if (category.isEmpty()) {
            return false;
        }
        for (int i = 0; i < category.length(); i++) {
            char ch = category.charAt(i);
            if (!(Character.isLetterOrDigit(ch) || ch == '-' || ch == '_')) {
                return false;
            }
        }
        return true;
    }
}