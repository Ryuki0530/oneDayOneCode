import java.io.File;

public class Main {
    public static void main(String[] args) {
        java.util.Map<String, Integer> format_counter = new java.util.HashMap<>();

        if (args.length == 1) {
            //debug
            // System.out.println("Argument: " + args[0]);
        } else {
            System.out.println("No arguments provided.");
            System.exit(1);
        }
        String filePathString = args[0];
        File currentDir = new File(filePathString);
        File[] files = currentDir.listFiles();
        if (files == null) {
            System.out.println("Error: Unable to access the directory.");
            System.exit(1);
        }
        for (File file : files) {
            String currentFileName = file.getName();
            if(!currentFileName.contains(".")) continue;

            String[] parts = currentFileName.split("\\.");
            String currentFormat = parts[parts.length - 1].toLowerCase();
            if (format_counter.containsKey(currentFormat)) {
                format_counter.put(currentFormat, format_counter.getOrDefault(currentFormat, 1) + 1);
            }else {
                format_counter.put(currentFormat, 1);
            }
            
            //debug
            // System.out.println("File: " + currentFileName + ", Format: " + currentFormat);
        }
        System.out.println("File format counts:");
        for (java.util.Map.Entry<String, Integer> entry : format_counter.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }
}