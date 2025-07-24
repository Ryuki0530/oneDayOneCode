import java.sql.*;

public class MySQLDemo {
    public static void main(String[] args) {
        // �ڑ����
        String url = "jdbc:mysql://localhost:3306/sample_db?useSSL=false&serverTimezone=UTC";
        String user = "root"; // �f�t�H���g���[�U�[�i�p�X���[�h���ݒ肳��Ă���ꍇ�͂���������j
        String password = ""; // XAMPP�����͋�

        // DB�ڑ�
        try (Connection conn = DriverManager.getConnection(url, user, password)) {
            System.out.println("? MySQL�ɐڑ����܂���");

            // �f�[�^��}������
            String insertSql = "INSERT INTO users (name, email) VALUES (?, ?)";
            try (PreparedStatement stmt = conn.prepareStatement(insertSql)) {
                stmt.setString(1, "�c�����Y");
                stmt.setString(2, "tanaka@example.com");
                stmt.executeUpdate();
                System.out.println("? �f�[�^��}�����܂���");
            }

            // �f�[�^���擾����
            String selectSql = "SELECT * FROM users";
            try (Statement stmt = conn.createStatement()) {
                ResultSet rs = stmt.executeQuery(selectSql);
                System.out.println("== �o�^�ς݃��[�U�[�ꗗ ==");
                while (rs.next()) {
                    int id = rs.getInt("id");
                    String name = rs.getString("name");
                    String email = rs.getString("email");
                    System.out.printf("ID: %d | ���O: %s | ���[��: %s\n", id, name, email);
                }
            }

        } catch (SQLException e) {
            System.out.println("? �G���[: " + e.getMessage());
        }
    }
}
