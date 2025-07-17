import java.io.BufferedReader;
import java.util.ArrayList;
import java.util.List;
import java.io.FileReader;

class StudentRecord{
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
    private String[] hedder;

    // For analysis
    private boolean isAnalyzed = false;
    private int topScoreStudentIndex;
    private double avg1stGtade;
    private double avg2ndGrade;
    private double avg3rdGrade;
    private int count1stGrade, count2ndGrade, count3rdGrade;


    public DataManager() {
        records = new ArrayList<>();
    }

    public boolean readStudentsCSV(String filePath){
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))){
            String currentLine;
            currentLine = br.readLine();
            hedder = currentLine.split(",");
            while((currentLine = br.readLine()) != null) {
                String[] values = currentLine.split(",");
                if (values.length != 4){
                    System.out.println("[CAUTION] Invalid record: " + currentLine);
                    System.out.println("\t->That record will be skipped.");
                    continue;
                }
                // // Debug
                // System.out.println(values[0] + " " + values[1] + " " + values[2] + " " + values[3]);
                records.add(new StudentRecord(values[0], values[1], Integer.parseInt(values[2]), Integer.parseInt(values[3])));
            }
            return true;
        } catch (Exception e) {
            System.out.println("[ERROR] An error occurred while reading the file: " + e.getMessage());
            e.printStackTrace();
            return false;
        }
       
    }

    public void addRecord(String id, String name, int grade, int score){
            StudentRecord record = new StudentRecord(id, name, grade, score);
            records.add(record);
    }

    public void dump(){
        if (records.isEmpty()) {
            System.out.println("[CAUTION] No records to display.");
            return;
        }
        System.out.println(hedder[0] + "\t" + hedder[1] + "\t" + hedder[2] + "\t" + hedder[3]);
        for (StudentRecord record : records) {
            System.out.println("" + record.id + "\t" + record.name + "\t" + record.grade + "\t" + record.score);
        }
    }

    public void analize(){
        if (records.isEmpty()) {
            System.out.println("[CAUTION] No records to analyze.");
            return;
        }
        
        avg1stGtade = 0;
        avg2ndGrade = 0;
        avg3rdGrade = 0;
        count1stGrade = 0;
        count2ndGrade = 0;
        count3rdGrade = 0;
        int topScore = Integer.MIN_VALUE;

        for (StudentRecord record : records) {
            if (record.grade == 1) {
                count1stGrade++;
                avg1stGtade += record.score;
            } else if (record.grade == 2) {
                count2ndGrade++;
                avg2ndGrade += record.score;
            } else if (record.grade == 3) {
                count3rdGrade++;
                avg3rdGrade += record.score;
            }
            if (record.score > topScore) {
                topScore = record.score;
                topScoreStudentIndex = records.indexOf(record);
            }
        }

        avg1stGtade /= count1stGrade;
        avg2ndGrade /= count2ndGrade;
        avg3rdGrade /= count3rdGrade;
        isAnalyzed = true;
    }

    public StudentRecord getTopScoreStudent(){
        if (!isAnalyzed) {
            System.out.println("[CAUTION] Analysis not performed yet.");
            System.out.println("\t->Please run the analyze method first.");
            return null;
        }
        if (topScoreStudentIndex < 0 || topScoreStudentIndex >= records.size()) {
            System.out.println("[CAUTION] No top score student found.");
            System.out.println("\t->Please run the analyze method first.");
            return null;
        }
        return records.get(topScoreStudentIndex);
    }
    public double[] getAvgGrade() {
        if (!isAnalyzed) {
            System.out.println("[CAUTION] Analysis not performed yet.");
            System.out.println("\t->Please run the analyze method first.");
            return null;
        }
        return new double[]{avg1stGtade, avg2ndGrade, avg3rdGrade};
    }
    public int[] getStundentCount() {
        if (!isAnalyzed) {
            System.out.println("[CAUTION] Analysis not performed yet.");
            System.out.println("\t->Please run the analyze method first.");
            return null;
        }
        return new int[]{count1stGrade, count2ndGrade, count3rdGrade};
    }
}

public class Main {
    public static void main(String[] args) {
        DataManager students = new DataManager();
        if(students.readStudentsCSV("students.csv")) {
            // System.out.println("[INFO] Students data loaded successfully.");
        } else {
            System.out.println("[ERROR] Failed to load students data.");
        }
        // Debug
        // students.dump();
        students.analize();
        StudentRecord topStudent = students.getTopScoreStudent();
        double[] socreAVerage = students.getAvgGrade();
        int[] studentCount = students.getStundentCount();

        System.out.println("[学年別統計]");
        System.out.println("1年: " + studentCount[0] + "人, 平均点: " + socreAVerage[0]);
        System.out.println("2年: " + studentCount[1] + "人, 平均点: " + socreAVerage[1]);
        System.out.println("3年: " + studentCount[2] + "人, 平均点: " + socreAVerage[2]);

        System.out.println("\n[最高得点者]");
        System.out.println(topStudent.name + " さん - " + topStudent.score + "点");


    }
}
