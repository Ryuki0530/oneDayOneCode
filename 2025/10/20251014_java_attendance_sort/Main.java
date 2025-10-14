import java.util.*;

public class Main {
    static class Student {
        int number;
        String name;

        Student(int number, String name) {
            this.number = number;
            this.name = name;
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        List<Student> students = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            int number = sc.nextInt();
            String name = sc.next();
            students.add(new Student(number, name));
        }
        students.sort(Comparator.comparingInt(s -> s.number));
        for (Student s : students) {
            System.out.println(s.name);
        }
    }
}