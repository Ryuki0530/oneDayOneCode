import java.io.*;
import java.util.*;
import java.time.LocalDate;

class CsvManager {
    private List<String[]> data = new ArrayList<>();
    private String[] header;

    public void readCsv(String filePath) throws IOException {
        data.clear();
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            if ((line = br.readLine()) != null) {
                header = line.split(",");
            }
            while ((line = br.readLine()) != null) {
                String[] row = line.split(",");
                data.add(row);
            }
        }
    }
    public List<String[]> getData() {
        return data;
    }

    public List<String[]> getDataByUid(String userId) {
        List<String[]> results = new ArrayList<>();
        for (String[] record : data) {
            if (record[0].equals(userId)) {
                results.add(record);
            }
        }
        return results;
    }

    public String[] getRow(int index) {
        if (index >= 0 && index < data.size()) {
            return data.get(index);
        }
        return null;
    }

    public void clear() {
        data.clear();
    }

    public void dump(){
        for (String[] row : data) {
            System.out.println(Arrays.toString(row));
        }
    }

    public Map<LocalDate, Integer> DailyLendingQuantity() {
        Map<LocalDate, Integer> lendingMap = new HashMap<>();
        for (String[] record : data) {
            LocalDate date = LocalDate.parse(record[2]);
            lendingMap.put(date, lendingMap.getOrDefault(date, 0) + 1);
        }
        return lendingMap;
    }
}


public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        CsvManager csvManager = new CsvManager();
        
        try {
            csvManager.readCsv("./borrow_records.csv");
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        // Debug
        // csvManager.dump();
        
        System.out.print("���[�U�[ID����� :");
        String userId = sc.nextLine();

        System.out.println("�y" + userId + "�̎؏o�����z");
        List<String[]> userRecords = csvManager.getDataByUid(userId);
        for (String[] record : userRecords) {
            System.out.println(record[1]   + " (" + record[2] + ")");
        }

        // ���ʑݏo����\��
        System.out.println("�y���ʑݏo���z");
        Map<LocalDate, Integer> dailyLending = csvManager.DailyLendingQuantity();
        for (Map.Entry<LocalDate, Integer> entry : dailyLending.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue() + "��");
        }
    }
}