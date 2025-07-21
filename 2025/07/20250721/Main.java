import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Spot{
    int id;
    String prefecture;
    String spot_name;
    String category;
    public Spot(int id, String prefecture, String spot_name, String category) {
        this.id = id;
        this.prefecture = prefecture;
        this.spot_name = spot_name;
        this.category = category;
    }
}

class Spots{
    private List<Spot> data = new ArrayList<>();
    private String[] header;
    private boolean debug = false;
    
    public Spots(String filePath, boolean debug) throws IOException {
        this.debug = debug;
        if (!readCsv(filePath)) {
            throw new IOException("Failed to read CSV file.");
        }
        if (debug) System.out.println("CSV file read successfully.");
        if (debug) this.dump();
    }

    public boolean readCsv(String filePath) throws IOException {
        data.clear();
        header = null;
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            if ((header = br.readLine().split(",")) == null) {
                if (debug) System.out.println("WCSV file is empty or header is missing.");
                return false;
            }
            String line;
            while ((line = br.readLine()) != null) {
                String[] row = line.split(",");
                data.add(new Spot(Integer.parseInt(row[0]), row[1], row[2], row[3]));
            }
        } catch (Exception e) {
            // TODO: handle exception
            if (debug) System.out.println("Error reading CSV file: " + e.getMessage());
            System.out.println("Error reading CSV ");
            return false;
        }
        return true;
    }

    public void dump() {
        if (header != null) {
            if (debug) System.out.println(String.join(",", header));
        }
        for (Spot spot : data) {
            if (debug) System.out.println(spot.id + "," + spot.prefecture + "," + spot.spot_name + "," + spot.category);
        }
    }

    public List<Spot> searchByPrefecture(String prefecture) {
        List<Spot> results = new ArrayList<>();
        for (Spot spot : data) {
            if (spot.prefecture.equals(prefecture)) {
                results.add(spot);
            }
        }
        return results;
    }
}


public class Main {
    public static void main(String[] args) throws IOException {
        final String FILE_PATH = "./spots.csv";
        Spots spots = new Spots(FILE_PATH,false);
        Scanner sc = new Scanner(System.in);
        System.out.print("åßñºÇì¸óÕÇµÇƒÇ≠ÇæÇ≥Ç¢\n>>");
        String searchPrefecture = sc.nextLine();
        List<Spot> results = spots.searchByPrefecture(searchPrefecture);
        for (Spot spot : results) {
            System.out.println(spot.spot_name + "(ÉJÉeÉSÉä: "+spot.category + ")");
        }

        
    }
}
