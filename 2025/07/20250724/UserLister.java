import java.util.List;

public class UserLister {
    public static void main(String[] args) {
        
        DBManager db = new DBManager(
            "localhost",
            "user_db",
            "users",
            "root",
            "",
            false
        );
        System.out.println("== ���[�U�[�ꗗ == ");

        // ���[�U�[����DB�ɓo�^
        List<User> users = db.getAllRecords();
        if (users.isEmpty()) {
            System.out.println("�o�^����Ă��郆�[�U�[�͂���܂���B");
        } else {
            for (User user : users) {
                System.out.printf("ID: %d | ���O: %s | ���[��: %s | �N��: %d\n", user.id, user.name, user.email, user.age);
            }
        }
        //Debug 
        // db.dumpAllRecords();
    }
}