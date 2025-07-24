import java.sql.*;

public class MySQLDemo {
    public static void main(String[] args) {
        // 接続情報
        String url = "jdbc:mysql://localhost:3306/sample_db?useSSL=false&serverTimezone=UTC";
        String user = "root"; // デフォルトユーザー（パスワードが設定されている場合はそれも書く）
        String password = ""; // XAMPP初期は空欄

        // DB接続
        try (Connection conn = DriverManager.getConnection(url, user, password)) {
            System.out.println("? MySQLに接続しました");

            // データを挿入する
            String insertSql = "INSERT INTO users (name, email) VALUES (?, ?)";
            try (PreparedStatement stmt = conn.prepareStatement(insertSql)) {
                stmt.setString(1, "田中太郎");
                stmt.setString(2, "tanaka@example.com");
                stmt.executeUpdate();
                System.out.println("? データを挿入しました");
            }

            // データを取得する
            String selectSql = "SELECT * FROM users";
            try (Statement stmt = conn.createStatement()) {
                ResultSet rs = stmt.executeQuery(selectSql);
                System.out.println("== 登録済みユーザー一覧 ==");
                while (rs.next()) {
                    int id = rs.getInt("id");
                    String name = rs.getString("name");
                    String email = rs.getString("email");
                    System.out.printf("ID: %d | 名前: %s | メール: %s\n", id, name, email);
                }
            }

        } catch (SQLException e) {
            System.out.println("? エラー: " + e.getMessage());
        }
    }
}
