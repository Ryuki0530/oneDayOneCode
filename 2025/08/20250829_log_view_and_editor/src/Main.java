// Main.java
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
    private boolean regexMode = true; // ここは必要に応じて引数で切替できるように拡張

    public static void main(String[] args) {
        Main app = new Main();
        int argStatus = app.parseArgs(args);
        if (argStatus != 0) {
            // 引数エラー時は終了
            System.exit(argStatus);
        }

        // 1) 共有 EventBus を1つだけ作る
        EventBus bus = new SimpleEventBus();

        // 2) Model / Controller を組み立て（同じ bus を渡す）
        LogDocument model = new LogDocument(bus);
        Controller controller = new Controller(model, bus);

        // 3) View を EDT 上で起動（→ この時点で購読が張られる）
        SwingUtilities.invokeLater(() -> {
            LogFrame frame = new LogFrame(controller); // 内部で controller.getEventBus() を使っている前提
            frame.setVisible(true);

            // 4) View 構築後に引数オプションを順に反映
            //    open -> filter -> export（export 指定時は完了後に終了）
            try {
                if (app.currentInputFilePath != null && !app.currentInputFilePath.isEmpty()) {
                    controller.openFile(app.currentInputFilePath);
                }

                if (app.currentFilterPattern != null && !app.currentFilterPattern.isEmpty()) {
                    controller.applyFilter(app.currentFilterPattern, app.regexMode);
                }

                // --export が指定されていれば実行
                if (app.outToStdOut || (app.currentExportFilePath != null && !app.currentExportFilePath.isEmpty())) {
                    if (app.outToStdOut) {
                        // stdout へ出力し、そのまま終了
                        controller.exportToStdout();
                    } else {
                        controller.exportToFile(app.currentExportFilePath);
                    }
                    // GUI 起動用途でなければ終了（CI等）
                    System.exit(0);
                }
            } catch (Exception e) {
                System.err.println("ERROR: 起動時処理で例外が発生しました: " + e.getMessage());
                // 必要ならここでダイアログ表示に切り替えてもOK
            }
        });
    }

    private int parseArgs(String[] args) {
        if (args == null || args.length == 0) return 0;

        for (int i = 0; i < args.length; i++) {
            String arg = args[i];

            // --open
            if (arg.startsWith("--open=")) {
                currentInputFilePath = arg.substring("--open=".length());
                continue;
            }
            if (arg.equals("--open")) {
                if (++i >= args.length) {
                    System.err.println("ERROR: --open の後にパスが必要です");
                    return 1;
                }
                currentInputFilePath = args[i];
                continue;
            }

            // --filter
            if (arg.startsWith("--filter=")) {
                currentFilterPattern = arg.substring("--filter=".length());
                continue;
            }
            if (arg.equals("--filter")) {
                if (++i >= args.length) {
                    System.err.println("ERROR: --filter の後に正規表現が必要です");
                    return 1;
                }
                currentFilterPattern = args[i];
                continue;
            }

            // （任意）--regex=[true|false] を受けたいなら
            if (arg.startsWith("--regex=")) {
                String v = arg.substring("--regex=".length());
                regexMode = Boolean.parseBoolean(v);
                continue;
            }
            if (arg.equals("--regex")) {
                if (++i >= args.length) {
                    System.err.println("ERROR: --regex の後に true/false が必要です");
                    return 1;
                }
                regexMode = Boolean.parseBoolean(args[i]);
                continue;
            }

            // --export
            if (arg.startsWith("--export=")) {
                String target = arg.substring("--export=".length());
                if ("stdout".equalsIgnoreCase(target)) {
                    outToStdOut = true;
                } else {
                    currentExportFilePath = target;
                }
                continue;
            }
            if (arg.equals("--export")) {
                if (++i >= args.length) {
                    System.err.println("ERROR: --export の後に stdout または パスが必要です");
                    return 1;
                }
                if ("stdout".equalsIgnoreCase(args[i])) {
                    outToStdOut = true;
                } else {
                    currentExportFilePath = args[i];
                }
                continue;
            }

            // 未知のオプション
            System.err.println("WARN: 未知のオプションを無視します: " + arg);
        }
        return 0;
    }
}
