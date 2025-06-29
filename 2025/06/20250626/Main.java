import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Comparator;
import java.util.stream.Stream;

public class Main {

    public static void main(String[] args) {
        Path dir = Paths.get("C:/Users/ryuki/Desktop/oneDayOneCode/20250626/watchdir");
        Path backupDir = Paths.get("C:/Users/ryuki/Desktop/oneDayOneCode/20250626/backup");

        try {
            if (!Files.exists(backupDir)) {
                Files.createDirectories(backupDir);
            }
        } catch (IOException e) {
            System.err.println("Error creating backup directory: " + e.getMessage());
            return;
        }

        try (WatchService watchService = FileSystems.getDefault().newWatchService()) {
            dir.register(watchService,
                         StandardWatchEventKinds.ENTRY_CREATE,
                         StandardWatchEventKinds.ENTRY_MODIFY,
                         StandardWatchEventKinds.ENTRY_DELETE);

            System.out.println("Monitoring directory: " + dir);

            while (true) {
                WatchKey key = watchService.take();

                for (WatchEvent<?> event : key.pollEvents()) {
                    WatchEvent.Kind<?> kind = event.kind();
                    Path fileName = (Path) event.context();
                    Path sourceFile = dir.resolve(fileName);

                    if (kind == StandardWatchEventKinds.ENTRY_DELETE) {
                        System.out.println("Deleted: " + fileName);
                        continue;
                    }

                    if ((kind == StandardWatchEventKinds.ENTRY_CREATE || kind == StandardWatchEventKinds.ENTRY_MODIFY)
                            && Files.isRegularFile(sourceFile)) {
                        try {
                            String backupFileName = getBackupFileName(fileName);
                            Path backupFile = backupDir.resolve(backupFileName);
                            Files.copy(sourceFile, backupFile, StandardCopyOption.REPLACE_EXISTING);
                            System.out.println("Backup created: " + backupFile.getFileName());
                        } catch (IOException e) {
                            System.err.println("Backup failed: " + e.getMessage());
                        }
                    }
                }

                boolean valid = key.reset();
                if (!valid) {
                    break;
                }
            }

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
    private static String getBackupFileName(Path fileName) {
        String name = fileName.getFileName().toString();
        int dot = name.lastIndexOf('.');
        String base = (dot == -1) ? name : name.substring(0, dot);
        String ext = (dot == -1) ? "" : name.substring(dot);
        String timestamp = LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyyMMdd_HHmmss"));
        return base + "_" + timestamp + ext;
    }
}
