package model;
import io.*;

public class LogDocument {
    private String filePath;
    private String content;

    private String[] data;

    public LogDocument() {
        this.filePath = "";
        this.content = "";
    }

    public int loadFile(String filePath) {
        this.filePath = filePath;
        FileLoader fileLoader = new FileLoader(filePath);
        data = fileLoader.loadLines();
        return 0;
    }

    public String[] getData() {
        return data;
    }
}
