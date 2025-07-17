// 参考用　Stream APIを使用した場合の実装
// すべてLLMによって生成されたコードです。

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;

class StudentRecord {
    String id;
    String name;
    int grade;
    int score;

    StudentRecord(String id, String name, int grade, int score) {
        this.id = id;
        this.name = name;
        this.grade = grade;
        this.score = score;
    }
}

class DataManager {
    private List<StudentRecord> records;
    private String[] header;

    // 分析結果
    private Map<Integer, Long> studentCountMap;
    private Map<Integer, Double> averageScoreMap;
    private StudentRecord topScoreStudent;
    private boolean isAnalyzed = false;

    public DataManager() {
        records = new ArrayList<>();
    }

    public boolean readStudentsCSV(String filePath) {
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String currentLine = br.readLine(); // header
            header = currentLine.split(",");

            while ((currentLine = br.readLine()) != null) {
                String[] values = currentLine.split(",");
                if (values.length != 4) {
                    System.out.println("[CAUTION] Invalid record: " + currentLine);
                    continue;
                }
                records.add(new StudentRecord(
                    values[0], values[1],
                    Integer.parseInt(values[2]),
                    Integer.parseInt(values[3])
                ));
            }
            return true;
        } catch (IOException e) {
            System.out.println("[ERROR] " + e.getMessage());
            return false;
        }
    }

    public void analyzeWithStream() {
        if (records.isEmpty()) {
            System.out.println("[CAUTION] No records to analyze.");
            return;
        }

        // 平均点
        averageScoreMap = records.stream()
            .collect(Collectors.groupingBy(
                r -> r.grade,
                Collectors.averagingInt(r -> r.score)
            ));

        // 学年別人数
        studentCountMap = records.stream()
            .collect(Collectors.groupingBy(
                r -> r.grade,
                Collectors.counting()
            ));

        // 最高得点者
        topScoreStudent = records.stream()
            .max(Comparator.comparingInt(r -> r.score))
            .orElse(null);

        isAnalyzed = true;
    }

    public void printAnalysis() {
        if (!isAnalyzed) {
            System.out.println("[CAUTION] Analysis not yet performed.");
            return;
        }

        System.out.println("[学年別統計]");
        for (int grade = 1; grade <= 3; grade++) {
            long count = studentCountMap.getOrDefault(grade, 0L);
            double avg = averageScoreMap.getOrDefault(grade, 0.0);
            System.out.printf("%d年: %d人, 平均点: %.1f\n", grade, count, avg);
        }

        System.out.println("\n[最高得点者]");
        if (topScoreStudent != null) {
            System.out.println(topScoreStudent.name + " さん - " + topScoreStudent.score + "点");
        } else {
            System.out.println("見つかりませんでした。");
        }
    }
}

public class Main {
    public static void main(String[] args) {
        DataManager manager = new DataManager();
        if (manager.readStudentsCSV("students.csv")) {
            manager.analyzeWithStream();
            manager.printAnalysis();
        } else {
            System.out.println("[ERROR] ファイルの読み込みに失敗しました。");
        }
    }
}
