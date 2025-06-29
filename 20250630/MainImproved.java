import java.io.File;
import java.util.Map;
import java.util.TreeMap;

public class MainImproved {
    public static void main(String[] args) {
        Map<String, Integer> formatCounter = new TreeMap<>();

        if (args.length != 1) {
            System.out.println("No arguments provided.");
            System.exit(1);
        }

        File dir = new File(args[0]);
        File[] files = dir.listFiles();
        if (files == null) {
            System.out.println("Error: Unable to access the directory.");
            System.exit(1);
        }

        for (File file : files) {
            if (!file.isFile()) continue;

            String fileName = file.getName();
            int lastDot = fileName.lastIndexOf('.');
            if (lastDot <= 0 || lastDot == fileName.length() - 1) continue;

            String ext = fileName.substring(lastDot + 1).toLowerCase();
            formatCounter.put(ext, formatCounter.getOrDefault(ext, 0) + 1);
        }

        System.out.println("File format counts:");
        for (Map.Entry<String, Integer> entry : formatCounter.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }
}