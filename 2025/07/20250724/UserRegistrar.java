import java.util.Scanner;

public class UserRegistrar {
    public static void main(String[] args) {
        
        Scanner sc;
        User userToAdd = new User();
        DBManager db = new DBManager(
            "localhost",
            "user_db",
            "users",
            "root",
            "",
            false
        );

        sc = new Scanner(System.in);
        System.out.println("== ユーザー登録 == ");
        System.out.print("名前を入力してください: ");
        userToAdd.name = sc.nextLine();
        System.out.print("メールアドレスを入力してください: ");
        userToAdd.email = sc.nextLine();
        System.out.print("年齢を入力してください: ");
        userToAdd.age = sc.nextInt();
        sc.close();

        // ユーザー情報をDBに登録
        if(!db.addUserRecord(userToAdd)) {
            System.out.println("登録に失敗しました。");
        } else {
            System.out.println("登録しました。");
        }
        //Debug 
        // db.dumpAllRecords();
    }
}