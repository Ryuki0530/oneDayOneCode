import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

class LogTimeChecker{

    List<String> logLines;

    //constructor
    public LogTimeChecker(String filePath, boolean debug){ 
        logLines = new ArrayList<>();
        try(BufferedReader br = new BufferedReader(new FileReader(filePath))){
            String line;
            while((line = br.readLine()) != null){
                logLines.add(line);
            }
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    private static int _toSeconds(String time) {
        String[] parts = time.split(":");
        if (parts.length != 3) return -1;
        int h = Integer.parseInt(parts[0]);
        int m = Integer.parseInt(parts[1]);
        int s = Integer.parseInt(parts[2]);
        return h * 3600 + m * 60 + s;
    }

    List<String> timestampConsistencyChecker(){
        List<String> inconsistentLogs = new ArrayList<>();
        if (logLines == null || logLines.size() < 2) return inconsistentLogs;

        int prevTimeInSeconds = -1;
        for (String line : logLines) {
            if (line.length() < 9 || line.charAt(0) != '[' || line.charAt(9) != ']') {
                continue;
            }
            String timeStr = line.substring(1, 9);
            int currentTimeInSeconds = _toSeconds(timeStr);
            if (currentTimeInSeconds == -1) {
                continue;
            }
            if (prevTimeInSeconds != -1 && currentTimeInSeconds < prevTimeInSeconds) {
                inconsistentLogs.add(line);
            }
            prevTimeInSeconds = currentTimeInSeconds;
        }
        return inconsistentLogs;
    }

    public void dumpLogs(){
        if (logLines == null) return;
        int itr = 0;    
        for(String line : logLines){
            System.out.println(itr+": "+line);
            itr++;
        }
    }

    public void printInconsistentLogs(){
        List<String> inconsistentLogs = timestampConsistencyChecker();
        if (inconsistentLogs.isEmpty()) {
            System.out.println("No inconsistent timestamps found.");
        } else {
            System.out.println("Inconsistent Timestamps:");
            for (String log : inconsistentLogs) {
                System.out.println(log);
            }
        }
    }

    public List<String> getLogLines(){
        return logLines;
    }


}

public class Main{
    private static final String LOG_FILE_PATH = "system.log";
    public static void main(String[] args) {
        boolean debug = Arrays.stream(args).anyMatch("--debug"::equals);
        LogTimeChecker checker = new LogTimeChecker(LOG_FILE_PATH, debug);
        checker.printInconsistentLogs();

        if (debug) checker.dumpLogs();
    }

}