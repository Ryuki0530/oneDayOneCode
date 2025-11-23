import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;

class TemperatureRecord {
    private final int temperature;
    private final long timestamp;

    public TemperatureRecord(int temperature) {
        this.temperature = temperature;
        this.timestamp = System.currentTimeMillis();
    }

    public int getTemperature() {
        return temperature;
    }

    public long getTimestamp() {
        return timestamp;
    }
}

class TemperatureManager {
    private final List<TemperatureRecord> records = new ArrayList<>();

    public void addTemperature(int temperature) {
        records.add(new TemperatureRecord(temperature));
    }

    public List<TemperatureRecord> getInRange(int min, int max) {
        List<TemperatureRecord> result = new ArrayList<>();
        for (TemperatureRecord r : records) {
            if (r.getTemperature() >= min && r.getTemperature() <= max) {
                result.add(r);
            }
        }
        return result;
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        TemperatureManager manager = new TemperatureManager();

        int MIN = 20;
        int MAX = 30;

        while (sc.hasNextInt()) {
            int value = sc.nextInt();
            if (value == -100) break;
            manager.addTemperature(value);
        }
        sc.close();

        List<TemperatureRecord> filtered = manager.getInRange(MIN, MAX);
        filtered.sort(Comparator.comparingInt(TemperatureRecord::getTemperature));

        for (TemperatureRecord rec : filtered) {
            System.out.println(rec.getTemperature());
        }
    }
}