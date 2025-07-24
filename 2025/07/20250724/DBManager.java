import java.sql.Connection;
import java.sql.Driver;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class DBManager {
    private String url;
    private String user;
    private String password;
    private String tableName;
    private boolean debug;

    public DBManager(String endpoint , String dname ,String tableName, String user , String password,boolean debug) {
        this.url = "jdbc:mysql://" + endpoint + ":3306/" + dname + "?useSSL=false&serverTimezone=UTC";
        this.user = user;
        this.password = password;
        this.debug = debug;
        this.tableName = tableName;
    }

    public boolean addUserRecord(User newUser) {
        String name = newUser.name;
        String email = newUser.email;
        int age = newUser.age;
        try (Connection conn = DriverManager.getConnection(url, user, password)) {
            if (debug) System.out.println("[INFO] MySQLに接続しました");

            // データを挿入する
            String insertSql = "INSERT INTO " + tableName + " (name, email, age) VALUES (?, ? , ?)";
            try (PreparedStatement stmt = conn.prepareStatement(insertSql)) {
                stmt.setString(1, name);
                stmt.setString(2, email);
                stmt.setInt(3, age);
                stmt.executeUpdate();
                if (debug) System.out.println("[INFO] データを挿入しました");
            }
        } catch (SQLException e) {
            if (debug) {System.out.println("[ERROR]: " + e.getMessage());}
            else {System.out.println("[ERROR] データの挿入に失敗しました");}
            return false;
        }
        return true;
    }

    public void dumpAllRecords() {
        String query = "SELECT * FROM " + tableName;
        try (Connection conn = DriverManager.getConnection(url, user, password);
             PreparedStatement stmt = conn.prepareStatement(query);
             ResultSet rs = stmt.executeQuery()) {

            if (debug) System.out.println("[INFO] 全レコードを取得しました");
            while (rs.next()) {
                String name = rs.getString("name");
                String email = rs.getString("email");
                int age = rs.getInt("age");
                System.out.println("Name: " + name + ", Email: " + email + ", Age: " + age);
            }
        } catch (SQLException e) {
            if (debug) { System.out.println("[ERROR]: " + e.getMessage()); }
            else {System.out.println("[ERROR] レコードの取得に失敗しました");}
        }
    }

    public List<User> getAllRecords() {
        List<User> userList = new ArrayList<>();
        String query = "SELECT * FROM " + tableName;
        try (Connection conn = DriverManager.getConnection(url, user, password);
             PreparedStatement stmt = conn.prepareStatement(query);
             ResultSet rs = stmt.executeQuery()) {

            if (debug) System.out.println("[INFO] 全レコードを取得しました");
            while (rs.next()) {
                User user = new User();
                user.id = rs.getInt("id"); // IDフィールドの取得    
                user.name = rs.getString("name");
                user.email = rs.getString("email");
                user.age = rs.getInt("age");
                userList.add(user);
            }
        } catch (SQLException e) {
            if (debug) { System.out.println("[ERROR]: " + e.getMessage()); }
            else {System.out.println("[ERROR] レコードの取得に失敗しました");}
        }
        return userList;
    }
    
}
