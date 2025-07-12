import java.time.LocalDate;
import java.util.HashMap;
import java.util.Map;
import java.util.LinkedHashMap;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;




public class Main {
    public static void main(String[] args) {
        // Example usage of SalesDataAnalyzer
        String filePath = "sales.csv";
        SalesDataAnalyzer analyzer = new SalesDataAnalyzer(filePath);

        // // Debug
        // Map<LocalDate, SalesData> salesData = analyzer.getSalesDataMap();
        // for (Map.Entry<LocalDate, SalesData> entry : salesData.entrySet()) {
        //     LocalDate date = entry.getKey();
        //     SalesData data = entry.getValue();
        //     System.out.println("Date: " + date);
        //     System.out.println("Customer Data: " + data.customerData);
        //     System.out.println("Price: " + data.price);
        // }


        // Get monthly sales summary
        Map<String, Integer> monthlySales = analyzer.getMonthlySalesSummary(true);
        System.out.println("\nMonthly Sales Summary:");
        for (Map.Entry<String, Integer> entry : monthlySales.entrySet()) {  
            String month = entry.getKey();
            int totalSales = entry.getValue();
            System.out.println( month + ": " + totalSales);
        }
    }
}

class SalesData {
    public String customerData;
    public int price;
    SalesData(String customerData, int price) {
        this.customerData = customerData;
        this.price = price;
    }
}

class SalesDataAnalyzer {
    //constructor
    private Map<LocalDate, SalesData> salesDataMap = new HashMap<>();
    public SalesDataAnalyzer(String filePath) {
        // Initialize the sales data map
        readFile(filePath);
    }

    private void readFile(String filePath){
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = br.readLine()) != null) {
                // Process the line and extract data
                String[] parts = line.split(",");
                LocalDate date = LocalDate.parse(parts[0]);
                String customerData = parts[1];
                int price = Integer.parseInt(parts[2]);
                salesDataMap.put(date, new SalesData(customerData, price));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public Map<LocalDate, SalesData> getSalesDataMap() {
        return salesDataMap;
    }
    public Map<String, Integer> getMonthlySalesSummary(boolean sort) {
        Map<String, Integer> monthlySales = new HashMap<>();
        for (Map.Entry<LocalDate, SalesData> entry : salesDataMap.entrySet()) {
            LocalDate date = entry.getKey();
            String monthKey = String.format("%d-%02d", date.getYear(), date.getMonthValue());
            monthlySales.put(monthKey, monthlySales.getOrDefault(monthKey, 0) + entry.getValue().price);
        }

        if (sort) {
            // Sort monthlySales by totalSales (ascending)
            Map<String, Integer> sortedMonthlySales = new LinkedHashMap<>();
            monthlySales.entrySet().stream()
            .sorted(Map.Entry.comparingByValue())
            .forEachOrdered(e -> sortedMonthlySales.put(e.getKey(), e.getValue()));
            return sortedMonthlySales;
        }
        return monthlySales;
    }
}