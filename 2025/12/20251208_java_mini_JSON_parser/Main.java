import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

class MiniJSONParser{

    String jsonString;
    Map<String, String> jsonMap = new HashMap<>();

    MiniJSONParser(String jsonString){
        this.jsonString = jsonString;
    }

    private List<String> _oneLinePerser( String jsonString ){
        //validate
        if( !( jsonString.contains(":") ) ){
            return null;
        }

        // perse
        String key = null;
        String value = null;
        List<String> result = new ArrayList<>();
        result.add(jsonString.substring(0, jsonString.indexOf(":")).trim());
        result.add(jsonString.substring(jsonString.indexOf(":") + 1).trim());
        

        return result;
    }

    public boolean allParse(){
        if (jsonString == null) {
            return false;
        }
        String trimmed = jsonString.trim();
        if (trimmed.length() < 2 || trimmed.charAt(0) != '{' || trimmed.charAt(trimmed.length() - 1) != '}') {
            return false;
        }
        trimmed = trimmed.substring(1, trimmed.length() - 1).trim();
        List<String> lines = new ArrayList<>();
        if (!trimmed.isEmpty()) {
            for (String line : trimmed.split("\\r?\\n")) {
                if (!line.trim().isEmpty()) {
                    lines.add(line.trim());
                }
            }
        }

        for (String line : lines) {
            List<String> keyValue = _oneLinePerser(line);
            if (keyValue == null || keyValue.size() != 2) {
                return false;
            }
            String key = keyValue.get(0);
            String value = keyValue.get(1);
            if (key.startsWith("\"") && key.endsWith("\"")) {
                key = key.substring(1, key.length() - 1);
            }
            if (value.startsWith("\"") && value.endsWith("\"")) {
                value = value.substring(1, value.length() - 1);
            }
            jsonMap.put(key, value);
        }

        return true;
    }

    public String getValue(String key){
        return jsonMap.get(key);
    }

    public List<String> getKeys(){
        return new ArrayList<>(jsonMap.keySet());
    }

    public Map<String, String> getJsonMap(){
        return jsonMap;
    }
    

}
public class Main {

    static String jsonFilePath;
    static String jsonString;

    public static void main(String[] args) {

        if (args.length > 0) {
            jsonFilePath = args[0];
        } else {
            jsonFilePath = "data.json";
        }

        jsonString = readFile(jsonFilePath);

        MiniJSONParser parser = new MiniJSONParser(jsonString);

        if (!parser.allParse()) {
            System.out.println("JSON parse failed.");
            return;
        }

        System.out.println("=== JSON Key-Value Parsed Result ===");
        for (Map.Entry<String, String> entry : parser.getJsonMap().entrySet()) {
            System.out.println(entry.getKey() + " = " + entry.getValue());
        }

        System.out.println("\nKeys: " + parser.getKeys());

        System.out.println("name = " + parser.getValue("name"));
    }

    static String readFile(String filePath){
        StringBuilder stb = new StringBuilder();
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String oneline = null;
            while ((oneline = br.readLine()) != null) {
                stb.append(oneline).append("\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return stb.toString();
    }
}