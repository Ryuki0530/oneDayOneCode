import javax.swing.SwingUtilities;

import view.LogFrame;
import controller.Controller;
import model.LogDocument;
import eventbus.*;

public class Main {
    private String currentInputFilePath;
    private String currentExportFilePath;
    private String currentFilterPattern;
    private boolean outToStdOut;
    private boolean regexMode = true; // --regex で切り替え可

    public static void main(String[] args) {
        Main app = new Main();
        int argStatus = app.parseArgs(args);
        if (argStatus != 0) System.exit(argStatus);

        // 1) 共有 EventBus
        EventBus bus = new SimpleEventBus();

        // 2) Model / Controller
        LogDocument model = new LogDocument(bus);
        Controller controller = new Controller(model, bus);
        controller.setStartupOptions(
            app.currentInputFilePath,
            app.currentExportFilePath,
            app.currentFilterPattern,
            app.outToStdOut,
            app.regexMode
        );

        // 3) View をEDTで起動（購読が張られる）
        SwingUtilities.invokeLater(() -> {
            LogFrame frame = new LogFrame(controller);
            frame.setVisible(true);

            // 4) View 構築後に起動オプション適用（購読が間に合っている状態）
            controller.applyStartupOptions();

            // エクスポート指定だった場合は即終了（GUIツールをCIでも使えるように）
            if (app.outToStdOut || (app.currentExportFilePath != null && !app.currentExportFilePath.isEmpty())) {
                System.exit(0);
            }
        });
    }

    private int parseArgs(String[] args) {
        if (args == null || args.length == 0) return 0;

        for (int i = 0; i < args.length; i++) {
            String arg = args[i];

            // --open
            if (arg.startsWith("--open=")) { currentInputFilePath = arg.substring("--open=".length()); continue; }
            if (arg.equals("--open")) {
                if (++i >= args.length) { System.err.println("ERROR: --open の後にパスが必要です"); return 1; }
                currentInputFilePath = args[i]; continue;
            }

            // --filter
            if (arg.startsWith("--filter=")) { currentFilterPattern = arg.substring("--filter=".length()); continue; }
            if (arg.equals("--filter")) {
                if (++i >= args.length) { System.err.println("ERROR: --filter の後に正規表現が必要です"); return 1; }
                currentFilterPattern = args[i]; continue;
            }

            // --regex
            if (arg.startsWith("--regex=")) { regexMode = Boolean.parseBoolean(arg.substring("--regex=".length())); continue; }
            if (arg.equals("--regex")) {
                if (++i >= args.length) { System.err.println("ERROR: --regex の後に true/false が必要です"); return 1; }
                regexMode = Boolean.parseBoolean(args[i]); continue;
            }

            // --export
            if (arg.startsWith("--export=")) {
                String target = arg.substring("--export=".length());
                if ("stdout".equalsIgnoreCase(target)) outToStdOut = true;
                else currentExportFilePath = target;
                continue;
            }
            if (arg.equals("--export")) {
                if (++i >= args.length) { System.err.println("ERROR: --export の後に stdout または パスが必要です"); return 1; }
                if ("stdout".equalsIgnoreCase(args[i])) outToStdOut = true;
                else currentExportFilePath = args[i];
                continue;
            }

            System.err.println("WARN: 未知のオプションを無視します: " + arg);
        }
        return 0;
    }
}
