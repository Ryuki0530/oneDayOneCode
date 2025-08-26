import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class ApiNegotiator {
    protected String city;
    protected int days;
    protected String format;
    protected String out;
    protected boolean debug;

    public ApiNegotiator(String city, int days, String format, String out, boolean debug) {
        this.city = city;
        this.days = days;
        this.format = format;
        this.out = out;
        this.debug = debug;
    }

    // 都市名から緯度・経度を取得
    public double[] getCoordinates() throws Exception {
        if (city == null || city.isEmpty()) {
            throw new IllegalArgumentException("city is null or empty");
        }
        String urlStr = "https://geocoding-api.open-meteo.com/v1/search?name=" +
            java.net.URLEncoder.encode(city, "UTF-8") + "&language=ja&count=1";
        URL url = new java.net.URI(urlStr).toURL();
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");
        int status = conn.getResponseCode();
        if (status != 200) {
            throw new RuntimeException("HTTP error code: " + status);
        }
        BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream(), "UTF-8"));
        StringBuilder response = new StringBuilder();
        String line;
        while ((line = in.readLine()) != null) {
            response.append(line);
        }
        in.close();
        conn.disconnect();

        // 正規表現でlatitudeとlongitudeを抽出
        Pattern latPattern = Pattern.compile("\"latitude\"\\s*:\\s*([0-9.\\-]+)");
        Pattern lonPattern = Pattern.compile("\"longitude\"\\s*:\\s*([0-9.\\-]+)");
        Matcher latMatcher = latPattern.matcher(response);
        Matcher lonMatcher = lonPattern.matcher(response);

        if (latMatcher.find() && lonMatcher.find()) {
            double lat = Double.parseDouble(latMatcher.group(1));
            double lon = Double.parseDouble(lonMatcher.group(1));
            return new double[]{lat, lon};
        } else {
            throw new RuntimeException("No results found for city: " + city);
        }
    }

    // 座標から天気情報を取得（標準ライブラリのみ）
    public String getWeather(double latitude, double longitude, int days) throws Exception {
        String urlStr = "https://api.open-meteo.com/v1/forecast?latitude=" + latitude
                + "&longitude=" + longitude
                + "&hourly=temperature_2m,precipitation,wind_speed_10m"
                + "&timezone=Asia%2FTokyo"
                + "&forecast_days=" + days;
        URL url = new java.net.URI(urlStr).toURL();
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");
        int status = conn.getResponseCode();
        if (status != 200) {
            throw new RuntimeException("HTTP error code: " + status);
        }
        BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream(), "UTF-8"));
        StringBuilder response = new StringBuilder();
        String line;
        while ((line = in.readLine()) != null) {
            response.append(line);
        }
        in.close();
        conn.disconnect();

        // 必要な情報だけ抜き出す例（最初の3時間分の気温・降水量・風速を表示）
        StringBuilder result = new StringBuilder();
        Pattern timePattern = Pattern.compile("\"time\"\\s*:\\s*\\[(.*?)\\]");
        Pattern tempPattern = Pattern.compile("\"temperature_2m\"\\s*:\\s*\\[(.*?)\\]");
        Pattern precPattern = Pattern.compile("\"precipitation\"\\s*:\\s*\\[(.*?)\\]");
        Pattern windPattern = Pattern.compile("\"wind_speed_10m\"\\s*:\\s*\\[(.*?)\\]");

        Matcher timeMatcher = timePattern.matcher(response);
        Matcher tempMatcher = tempPattern.matcher(response);
        Matcher precMatcher = precPattern.matcher(response);
        Matcher windMatcher = windPattern.matcher(response);

        if (timeMatcher.find() && tempMatcher.find() && precMatcher.find() && windMatcher.find()) {
            String[] times = timeMatcher.group(1).replaceAll("\"", "").split(",");
            String[] temps = tempMatcher.group(1).split(",");
            String[] precs = precMatcher.group(1).split(",");
            String[] winds = windMatcher.group(1).split(",");

            int count = Math.min(3, Math.min(times.length, Math.min(temps.length, Math.min(precs.length, winds.length))));
            for (int i = 0; i < count; i++) {
                result.append("時刻: ").append(times[i].trim())
                      .append(" / 気温: ").append(temps[i].trim()).append("℃")
                      .append(" / 降水量: ").append(precs[i].trim()).append("mm")
                      .append(" / 風速: ").append(winds[i].trim()).append("km/h\n");
            }
        } else {
            result.append("天気データの抽出に失敗しました。\n");
        }
        return result.toString();
    }
}


public class Main {

    private String city = null;
    private int days = 3;
    private String format = "table";
    private String out = null;
    private boolean debug = false;


    public static void main(String[] args) {
        Main main = new Main();
        main.argsParser(args);
        if(main.debug){
            System.out.println("city: " + main.city);
            System.out.println("days: " + main.days);
            System.out.println("format: " + main.format);
            System.out.println("out: " + main.out);
            System.out.println("debug: " + main.debug);
        }

        ApiNegotiator api = new ApiNegotiator(main.city, main.days, main.format, main.out, main.debug);
        try {
            double[] coords = api.getCoordinates();
            System.out.println("Latitude: " + coords[0] + ", Longitude: " + coords[1]);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void argsParser(String[] args) {
        
        String city = null;
        int days = 3;
        String format = "table";
        String out = null;
        boolean debug = false;

        for (int i = 0; i < args.length; i++) {
            switch (args[i]) {
            case "--city":
                if (i + 1 < args.length) {
                city = args[++i];
                }
                break;
            case "--days":
                if (i + 1 < args.length) {
                try {
                    days = Integer.parseInt(args[++i]);
                } catch (NumberFormatException e) {
                    days = 3;
                }
                }
                break;
            case "--format":
                if (i + 1 < args.length) {
                String f = args[++i];
                if (f.equals("table") || f.equals("csv") || f.equals("json")) {
                    format = f;
                }
                }
                break;
            case "--out":
                if (i + 1 < args.length) {
                out = args[++i];
                }
                break;
            case "--debug":
                debug = true;
                break;
            }
        }
        this.city = city;
        this.days = days;
        this.format = format;
        this.out = out;
        this.debug = debug;
    }
}