// model/LogDocument.java
package model;
import io.FileLoader;
import java.util.Arrays;
import java.util.List;
import eventbus.*;

public class LogDocument {
    private final EventBus bus;
    private List<String> data = List.of();
    private String filePath;

    public LogDocument(EventBus bus) {
        this.bus = bus;
    }

    public int loadFile(String filePath) {
        this.filePath = filePath;
        FileLoader fileLoader = new FileLoader(filePath);
        data = Arrays.asList(fileLoader.loadLines());

        System.out.println("Model loaded " + data.size() + " lines from " + filePath);
        for (String line : data) System.out.println("Line: " + line);

        // View が購読済みのタイミングで呼ばれるようにするのが肝
        bus.publish(new LinesUpdated(data, data.size(), data.size()));
        return 0;
    }

    // “あと追い”で View から現在状態を欲しくなる時のための補助（任意）
    public List<String> getCurrentLines() { return data; }
}
