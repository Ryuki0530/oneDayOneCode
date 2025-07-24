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
        System.out.println("== ユーザー一覧 == ");

        // ユーザー情報をDBに登録
        List<User> users = db.getAllRecords();
        if (users.isEmpty()) {
            System.out.println("登録されているユーザーはありません。");
        } else {
            for (User user : users) {
                System.out.printf("ID: %d | 名前: %s | メール: %s | 年齢: %d\n", user.id, user.name, user.email, user.age);
            }
        }
        //Debug 
        // db.dumpAllRecords();
    }
}