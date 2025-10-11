import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.math.RoundingMode;
import java.math.BigDecimal;
import java.util.Locale;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String line;
        int col = -1;
        int count = 0;
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;
        double sum = 0.0;

        // 1行目: 列番号
        while ((line = br.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty()) continue;
            col = Integer.parseInt(line);
            break;
        }
        if (col < 1) {
            System.out.println("NA NA NA");
            return;
        }
        // 2行目以降: CSVデータ
        while ((line = br.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty()) continue;
            String[] tokens = line.split(",", -1);
            if (tokens.length < col) continue;
            String target = tokens[col - 1].trim();
            if (target.isEmpty()) continue;
            double value;
            try {
                value = Double.parseDouble(target);
                if (Double.isNaN(value) || Double.isInfinite(value)) continue;
            } catch (NumberFormatException e) {
                continue;
            }
            min = Math.min(min, value);
            max = Math.max(max, value);
            sum += value;
            count++;
        }
        if (count == 0) {
            System.out.println("NA NA NA");
        } else {
            double avg = sum / count;
            System.out.printf(Locale.US, "%.3f %.3f %.3f%n", round3(min), round3(max), round3(avg));
        }
    }

    private static double round3(double value) {
        BigDecimal bd = new BigDecimal(Double.toString(value));
        bd = bd.setScale(3, RoundingMode.HALF_UP);
        return bd.doubleValue();
    }
}