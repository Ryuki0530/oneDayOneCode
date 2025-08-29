// Controller.java
package controller;

import java.nio.file.Paths;

import eventbus.EventBus;
import model.LogDocument;

public class Controller {
    private final LogDocument model;
    private final EventBus bus;

    // 起動オプション（任意）
    private String currentInputFilePath;
    private String currentExportFilePath;
    private String currentFilterPattern;
    private boolean outToStdOut;
    private boolean regexMode = true; // 必要なら Main 側で設定

    public Controller(LogDocument model, EventBus bus) {
        this.model = model;
        this.bus = bus;
    }

    // --- 起動オプションを後から渡したい場合用 ---
    public void setStartupOptions(
            String inputPath,
            String exportTarget,
            String filterPattern,
            boolean toStdout,
            boolean regexMode) {
        this.currentInputFilePath = inputPath;
        this.currentExportFilePath = exportTarget;
        this.currentFilterPattern = filterPattern;
        this.outToStdOut = toStdout;
        this.regexMode = regexMode;
    }

    /** View 構築（購読完了）後に呼ぶと安全：open → filter → export の順で実行 */
    public void applyStartupOptions() {
        if (currentInputFilePath != null && !currentInputFilePath.isEmpty()) {
            openFile(currentInputFilePath);
        }
        if (currentFilterPattern != null && !currentFilterPattern.isEmpty()) {
            applyFilter(currentFilterPattern, regexMode);
        }
        if (outToStdOut) {
            exportToStdout();
        } else if (currentExportFilePath != null && !currentExportFilePath.isEmpty()) {
            exportToFile(currentExportFilePath);
        }
    }

    // ============ View から呼ばれる公開API ============
    public EventBus getEventBus() { return bus; }

    public void openFile(String path) {
        model.loadFile(path); // Model 側が LinesUpdated を publish する
    }


    public void applyFilter(String pattern, boolean regex) {
        model.applyFilter(pattern, regex);
    }

    public void exportToStdout() {
        model.exportToStdout();
    }

    public void saveViewToSameFile() {
        model.saveViewToSameFile();
    }

    public void exportToFile(String path) {
        model.exportToFile(Paths.get(path));
    }
}
