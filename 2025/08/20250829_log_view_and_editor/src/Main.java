import view.LogFrame;
import controller.Controller;

public class Main {
    private String currentInputFilePath;
    private String currentExportFilePath;
    private String currentFilterPattern;
    private boolean outToStdOut;
    
    public static void main(String[] args) {
        Main main = new Main();
        int srgStatus = main.ArgsParser(args);
        // System.out.println("ArgStatus: " + status);
        // System.out.println("Current Input File Path: " + main.currentInputFilePath);
        // System.out.println("Current Export File Path: " + main.currentExportFilePath);
        // System.out.println("Current Filter Pattern: " + main.currentFilterPattern);
        // System.out.println("Output to StdOut: " + main.outToStdOut);
        LogFrame logFrame = new LogFrame(new Controller(
            main.currentInputFilePath,
            main.currentExportFilePath,
            main.currentFilterPattern,
            main.outToStdOut
        ));

        logFrame.setVisible(true);
    }

    private int ArgsParser(String[] args) {
        if (args.length == 0) {
            System.out.println("No arguments provided.");
            return 0;
        }

        for (int i = 0; i < args.length; i++) {
            String arg = args[i];
            if (arg.startsWith("--open=")) {
                currentInputFilePath = arg.substring("--open=".length());
            } else if (arg.equals("--open")) {
                if (++i >= args.length) {
                    System.err.println("ERROR: --open の後にパスが必要です");
                    return 1;
                }
                currentInputFilePath = args[i];
            } else if (arg.startsWith("--filter=")) {
                currentFilterPattern = arg.substring("--filter=".length());
            } else if (arg.equals("--filter")) {
                if (++i >= args.length) {
                    System.err.println("ERROR: --filter の後に正規表現が必要です");
                    return 1;
                }
                currentFilterPattern = args[i];
            } else if (arg.startsWith("--export=")) {
                currentExportFilePath = arg.substring("--export=".length());
            } else if (arg.equals("--export")) {
                if (++i >= args.length) {
                    System.err.println("ERROR: --export の後に stdout または パスが必要です");
                    return 1;
                }
                if (args[i].equals("stdout")) {
                    outToStdOut = true;
                } else {
                    currentExportFilePath = args[i];
                }
            }
        }
        return 0;
    }
}