import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.URLEncoder;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    private static final HttpClient CLIENT = HttpClient.newBuilder()
            .connectTimeout(Duration.ofSeconds(10))
            .build();

    public static void main(String[] args) {
        try {
            String city = readCityName();
            if (city.isBlank()) {
                System.out.println("地名が入力されていません。");
                return;
            }

            Location location = fetchLocation(city);
            if (location == null) {
                System.out.println("地名が見つかりませんでした: " + city);
                return;
            }

            Weather weather = fetchWeather(location.latitude, location.longitude);
            if (weather == null) {
                System.out.println("天気情報の取得に失敗しました。");
                return;
            }

            List<String> advice = buildAdvice(weather);
            printResult(location, weather, advice);
        } catch (Exception e) {
            System.out.println("エラーが発生しました: " + e.getMessage());
        }
    }

    private static String readCityName() throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8));
        return br.readLine();
    }

    private static Location fetchLocation(String city) throws IOException, InterruptedException {
        String encodedCity = URLEncoder.encode(city, StandardCharsets.UTF_8);
        String url = "https://geocoding-api.open-meteo.com/v1/search?name="
                + encodedCity + "&count=1&language=ja&format=json";

        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .timeout(Duration.ofSeconds(10))
                .GET()
                .build();

        HttpResponse<String> response = CLIENT.send(request, HttpResponse.BodyHandlers.ofString(StandardCharsets.UTF_8));
        if (response.statusCode() != 200) {
            throw new IOException("地名検索APIの応答コード: " + response.statusCode());
        }

        String body = response.body();
        if (!body.contains("\"results\"")) {
            return null;
        }

        String name = findString(body, "name");
        Double latitude = findDouble(body, "latitude");
        Double longitude = findDouble(body, "longitude");
        String country = findString(body, "country");

        if (name == null || latitude == null || longitude == null) {
            return null;
        }

        return new Location(name, country == null ? "" : country, latitude, longitude);
    }

    private static Weather fetchWeather(double latitude, double longitude) throws IOException, InterruptedException {
        String url = String.format(Locale.US,
                "https://api.open-meteo.com/v1/forecast?latitude=%.6f&longitude=%.6f"
                        + "&current=temperature_2m,wind_speed_10m,rain,showers,snowfall"
                        + "&hourly=precipitation_probability&forecast_days=1&timezone=Asia%%2FTokyo",
                latitude, longitude);

        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .timeout(Duration.ofSeconds(10))
                .GET()
                .build();

        HttpResponse<String> response = CLIENT.send(request, HttpResponse.BodyHandlers.ofString(StandardCharsets.UTF_8));
        if (response.statusCode() != 200) {
            throw new IOException("天気APIの応答コード: " + response.statusCode());
        }

        String body = response.body();

        Double temperature = findDoubleInObject(body, "current", "temperature_2m");
        Double windSpeed = findDoubleInObject(body, "current", "wind_speed_10m");
        Double rain = findDoubleInObject(body, "current", "rain");
        Double showers = findDoubleInObject(body, "current", "showers");
        Double snowfall = findDoubleInObject(body, "current", "snowfall");
        Integer precipitationProbability = findFirstIntInArrayObject(body, "hourly", "precipitation_probability");

        if (temperature == null || windSpeed == null || precipitationProbability == null) {
            return null;
        }

        return new Weather(
                temperature,
                windSpeed,
                precipitationProbability,
                rain == null ? 0.0 : rain,
                showers == null ? 0.0 : showers,
                snowfall == null ? 0.0 : snowfall
        );
    }

    private static List<String> buildAdvice(Weather weather) {
        List<String> advice = new ArrayList<>();

        if (weather.precipitationProbability >= 50 || weather.rain > 0 || weather.showers > 0 || weather.snowfall > 0) {
            advice.add("傘を持っていきましょう");
        }
        if (weather.temperature < 10) {
            advice.add("暖かい服装がおすすめです");
        } else if (weather.temperature >= 28) {
            advice.add("飲み物を持っていきましょう");
        }
        if (weather.windSpeed >= 10) {
            advice.add("風が強いので注意してください");
        }
        if (advice.isEmpty()) {
            advice.add("特別な持ち物は不要そうです");
        }

        return advice;
    }

    private static void printResult(Location location, Weather weather, List<String> advice) {
        System.out.println("場所: " + location.name + (location.country.isEmpty() ? "" : " (" + location.country + ")"));
        System.out.printf(Locale.US, "気温: %.1f℃%n", weather.temperature);
        System.out.println("降水確率: " + weather.precipitationProbability + "%");
        System.out.printf(Locale.US, "風速: %.1f km/h%n", weather.windSpeed);
        System.out.println();
        System.out.println("持ち物アドバイス:");
        for (String item : advice) {
            System.out.println("- " + item);
        }
    }

    private static String findString(String json, String key) {
        Pattern pattern = Pattern.compile("\\\"" + Pattern.quote(key) + "\\\"\\s*:\\s*\\\"(.*?)\\\"");
        Matcher matcher = pattern.matcher(json);
        if (matcher.find()) {
            return matcher.group(1);
        }
        return null;
    }

    private static Double findDouble(String json, String key) {
        Pattern pattern = Pattern.compile("\\\"" + Pattern.quote(key) + "\\\"\\s*:\\s*(-?\\d+(?:\\.\\d+)?)");
        Matcher matcher = pattern.matcher(json);
        if (matcher.find()) {
            return Double.parseDouble(matcher.group(1));
        }
        return null;
    }

    private static Double findDoubleInObject(String json, String objectKey, String valueKey) {
        String object = extractObject(json, objectKey);
        if (object == null) {
            return null;
        }
        return findDouble(object, valueKey);
    }

    private static Integer findFirstIntInArrayObject(String json, String objectKey, String valueKey) {
        String object = extractObject(json, objectKey);
        if (object == null) {
            return null;
        }

        Pattern pattern = Pattern.compile("\\\"" + Pattern.quote(valueKey) + "\\\"\\s*:\\s*\\[(.*?)\\]", Pattern.DOTALL);
        Matcher matcher = pattern.matcher(object);
        if (!matcher.find()) {
            return null;
        }

        String arrayContent = matcher.group(1).trim();
        if (arrayContent.isEmpty()) {
            return null;
        }

        String firstValue = arrayContent.split(",")[0].trim();
        if (firstValue.equals("null")) {
            return null;
        }
        return Integer.parseInt(firstValue);
    }

    private static String extractObject(String json, String objectKey) {
        String startToken = "\"" + objectKey + "\"";
        int keyIndex = json.indexOf(startToken);
        if (keyIndex < 0) {
            return null;
        }

        int colonIndex = json.indexOf(':', keyIndex);
        if (colonIndex < 0) {
            return null;
        }

        int braceStart = json.indexOf('{', colonIndex);
        if (braceStart < 0) {
            return null;
        }

        int depth = 0;
        for (int i = braceStart; i < json.length(); i++) {
            char c = json.charAt(i);
            if (c == '{') {
                depth++;
            } else if (c == '}') {
                depth--;
                if (depth == 0) {
                    return json.substring(braceStart, i + 1);
                }
            }
        }
        return null;
    }

    private static class Location {
        final String name;
        final String country;
        final double latitude;
        final double longitude;

        Location(String name, String country, double latitude, double longitude) {
            this.name = name;
            this.country = country;
            this.latitude = latitude;
            this.longitude = longitude;
        }
    }

    private static class Weather {
        final double temperature;
        final double windSpeed;
        final int precipitationProbability;
        final double rain;
        final double showers;
        final double snowfall;

        Weather(double temperature, double windSpeed, int precipitationProbability,
                double rain, double showers, double snowfall) {
            this.temperature = temperature;
            this.windSpeed = windSpeed;
            this.precipitationProbability = precipitationProbability;
            this.rain = rain;
            this.showers = showers;
            this.snowfall = snowfall;
        }
    }
}
