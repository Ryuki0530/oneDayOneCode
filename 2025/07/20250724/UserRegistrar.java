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
        System.out.println("== ���[�U�[�o�^ == ");
        System.out.print("���O����͂��Ă�������: ");
        userToAdd.name = sc.nextLine();
        System.out.print("���[���A�h���X����͂��Ă�������: ");
        userToAdd.email = sc.nextLine();
        System.out.print("�N�����͂��Ă�������: ");
        userToAdd.age = sc.nextInt();
        sc.close();

        // ���[�U�[����DB�ɓo�^
        if(!db.addUserRecord(userToAdd)) {
            System.out.println("�o�^�Ɏ��s���܂����B");
        } else {
            System.out.println("�o�^���܂����B");
        }
        //Debug 
        // db.dumpAllRecords();
    }
}