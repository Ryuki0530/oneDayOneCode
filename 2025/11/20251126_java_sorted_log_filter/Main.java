import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;

class Log {
    final LocalDateTime timestamp;
    final String logtype;
    final String message;

    Log(LocalDateTime timestamp, String logtype, String message) {
        this.timestamp = timestamp;
        this.logtype = logtype;
        this.message = message;
    }
}

class LogManager {
    private final List<Log> store = new ArrayList<>();
    private static final DateTimeFormatter FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
    public void addLog(String line) {
        if (line == null) return;
        line = line.trim();
        if (line.isEmpty()) return;
        String[] parts = line.split(" ", 3);
        if (parts.length < 3) return;
        String timeStr = parts[0] + " " + parts[1];
        try {
            LocalDateTime ts = LocalDateTime.parse(timeStr, FMT);
            String rest = parts[2];
            String[] restParts = rest.split(" ", 2);
            String type = restParts[0];
            String msg = restParts.length > 1 ? restParts[1] : "";
            store.add(new Log(ts, type, msg));
        } catch (DateTimeParseException e) {}
    }
    public List<Log> getLogsByType(String logtype) {
        List<Log> result = new ArrayList<>();
        for (Log l : store) {
            if (l.logtype != null && l.logtype.equals(logtype)) result.add(l);
        }
        result.sort(Comparator.comparing(l -> l.timestamp));
        return result;
    }
}

public class Main {
    public static void main(String[] args) throws IOException {
        LogManager mgr = new LogManager();
        Scanner sc = new Scanner(System.in);

        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            mgr.addLog(line);
        }
        sc.close();

        List<Log> errors = mgr.getLogsByType("ERROR");
        DateTimeFormatter outFmt = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        for (Log log : errors) {
            System.out.println(log.timestamp.format(outFmt) + " " + log.logtype + " " + log.message);
        }
    }
}