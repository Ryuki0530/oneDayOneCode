package controller;

public class Controller {
    private String currentInputFilePath;
    private String currentExportFilePath;
    private String currentFilterPattern;
    private boolean outToStdOut;
    
    public Controller(
        String currentInputFilePath,
        String currentExportFilePath,
        String currentFilterPattern,
        boolean outToStdOut
    ) {
        this.currentInputFilePath = currentInputFilePath;
        this.currentExportFilePath = currentExportFilePath;
        this.currentFilterPattern = currentFilterPattern;
        this.outToStdOut = outToStdOut;

        LogDocument logDocument = new LogDocument();
        logDocument.loadFile(currentInputFilePath);

    }
}
