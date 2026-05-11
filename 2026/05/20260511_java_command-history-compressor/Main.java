import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;

class CommandHistoryCompressor {
    public List<String> compress(List<String> commands) {
        List<String> result = new ArrayList<>();
        
        if (commands.isEmpty()) {
            return result;
        }
        
        String currentCommand = commands.get(0);
        int count = 1;
        
        for (int i = 1; i < commands.size(); i++) {
            if (commands.get(i).equals(currentCommand)) {
                count++;
            } else {
                result.add(currentCommand + " " + count);
                currentCommand = commands.get(i);
                count = 1;
            }
        }
        
        result.add(currentCommand + " " + count);
        
        return result;
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        int n = scanner.nextInt();
        scanner.nextLine();
        
        List<String> commands = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            commands.add(scanner.nextLine());
        }
        
        CommandHistoryCompressor compressor = new CommandHistoryCompressor();
        List<String> compressed = compressor.compress(commands);
        
        for (String line : compressed) {
            System.out.println(line);
        }
        
        scanner.close();
    }
}
