package io;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class FileLoader {
    private String filePath;

    public FileLoader(String filePath) {
        this.filePath = filePath;
    }

    public String[] loadLines() {
        StringBuilder content = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                content.append(line).append("\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return content.toString().split("\n");
    }

    int setFilePath(String filePath) {
        this.filePath = filePath;
        return 0;
    }
}
