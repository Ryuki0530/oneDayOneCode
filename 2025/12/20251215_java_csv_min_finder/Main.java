import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class Main {

    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        int N;
        int col;
        String mode;

        try {
            String lineN = br.readLine();
            if (lineN == null) { System.out.println("NA"); return; }
            N = Integer.parseInt(lineN.trim());

            String lineCol = br.readLine();
            if (lineCol == null) { System.out.println("NA"); return; }
            col = Integer.parseInt(lineCol.trim());

            mode = br.readLine();
            if (mode == null) { System.out.println("NA"); return; }
            mode = mode.trim();
        } catch (Exception e) {
            System.out.println("NA");
            return;
        }

        long sum = 0L;
        long min = Long.MAX_VALUE;
        long max = Long.MIN_VALUE;
        long count = 0L;

        for (int i = 0; i < N; i++) {
            String row = br.readLine();
            if (row == null) break; 

            String[] parts = row.split(",", -1);

            if (col < 0 || col >= parts.length) continue;
            String cell = parts[col].trim();
            if (cell.isEmpty()) continue;

            long v;
            try {
                v = Long.parseLong(cell);
            } catch (NumberFormatException e) {
                continue;
            }

            sum += v;
            if (v < min) min = v;
            if (v > max) max = v;
            count++;
        }

        if (!(mode.equals("sum") || mode.equals("avg") || mode.equals("min") || mode.equals("max"))) {
            System.out.println("NA");
            return;
        }

        if (count == 0) {
            System.out.println("NA");
            return;
        }

        switch (mode) {
            case "sum":
                System.out.println(sum);
                break;
            case "min":
                System.out.println(min);
                break;
            case "max":
                System.out.println(max);
                break;
            case "avg":
                double avg = (double) sum / (double) count;
                System.out.printf("%.2f%n", avg);
                break;
            default:
                System.out.println("NA");
        }
    }
}
