import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

class Main {
    public static void main(String[] args) {
        Books books = new Books("books.csv");
        BorrowRecords records = new BorrowRecords("borrow.csv");
        Scanner sc = new Scanner(System.in);
        System.out.print("���[�U�[ID����͂��Ă�������: ");
        String userId = sc.nextLine();
        int[] bookIds = records.checkBookIDsByUserID(userId);
        System.out.println("���[�U�[ID: " + userId + " �̎؂�Ă���{:");
        for (int bookId : bookIds) {
            String bookTitle = books.refeBookTitleByID(bookId);
            String bookAuthor = books.refeBookAuthorByID(bookId);
            if (bookTitle != null) {
                System.out.print(bookTitle + " (" + bookAuthor + ")");
                if(records.isDateLimitOver(bookId, userId)) {
                    System.out.println(" - �ԋp�����؂�");
                }else{
                    System.out.println("");
                }
            }
        }
    }
}
class Book {
    int bookId;
    String title;
    String author;

    Book(int bookId, String title, String author) {
        this.bookId = bookId;
        this.title = title;
        this.author = author;
    }
}

class BorrowRecord {
    String userId;
    int bookId;
    LocalDate borrowDate;

    BorrowRecord(String userId, int bookId, LocalDate borrowDate) {
        this.userId = userId;
        this.bookId = bookId;
        this.borrowDate = borrowDate;
    }
}

class Books {
    Map<Integer, Book> bookMap = new HashMap<>();

    // �R���X�g���N�^�ɏC���ivoid���폜�j
    public Books(String filePath) {
        File file = new File(filePath);
        if (!file.exists()) {
            System.out.println(filePath + "�����݂��܂���B");
            return;
        }
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String header = reader.readLine(); // �w�b�_�[�s���X�L�b�v�i�ϐ������Ӗ��̂�����̂ɕύX�j
            String rawCurrentData;
            while ((rawCurrentData = reader.readLine()) != null) {
                String[] currentData = rawCurrentData.split(",");
                if (currentData.length >= 3) { // �z��̒������`�F�b�N
                    int bookId = Integer.parseInt(currentData[0].trim());
                    String title = currentData[1].trim();
                    String author = currentData[2].trim();
                    bookMap.put(bookId, new Book(bookId, title, author));
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String refeBookTitleByID(int bookId) {
        Book book = bookMap.get(bookId);
        if (book != null) {
            return book.title;
        }
        return null; // �{��������Ȃ��ꍇ
    }
    public String refeBookAuthorByID(int bookId) {
        Book book = bookMap.get(bookId);
        if (book != null) {
            return book.author;
        }
        return null; // �{��������Ȃ��ꍇ
    }
}

class BorrowRecords {
    Map<Integer, BorrowRecord> recordMap = new HashMap<>();
    String DATE_TIME_FORMAT = "yyyy-MM-dd"; // ���̃t�H�[�}�b�g��啶���ɏC��

    // �R���X�g���N�^�ɏC���ivoid���폜�j
    public BorrowRecords(String filePath) {
        int key = 0;
        File file = new File(filePath);
        if (!file.exists()) {
            System.out.println(filePath + "�����݂��܂���B");
            return;
        }
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            
            String header = reader.readLine(); // �w�b�_�[�s���X�L�b�v�i�ϐ������Ӗ��̂�����̂ɕύX�j
            String rawCurrentData;
            while ((rawCurrentData = reader.readLine()) != null) {
                String[] currentData = rawCurrentData.split(",");
                if (currentData.length >= 3) { // �z��̒������`�F�b�N
                    String uId = currentData[0].trim();
                    int bookId = Integer.parseInt(currentData[1].trim());
                    String rawDate = currentData[2].trim();
                    LocalDate date = LocalDate.parse(rawDate, DateTimeFormatter.ofPattern(DATE_TIME_FORMAT));
                    recordMap.put(key, new BorrowRecord(uId, bookId, date));
                    key++;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public int[] checkBookIDsByUserID(String userId) {
        ArrayList<Integer> result = new ArrayList<>();
        for (BorrowRecord record : recordMap.values()) {
            if (record.userId.equals(userId)) {
                result.add(record.bookId);
            }
        }
        int[] ids = new int[result.size()];
        for (int i = 0; i < result.size(); i++) {
            ids[i] = result.get(i);
        }
        return ids;
    }

    public boolean isDateLimitOver(int bookId, String userId) {
        for (BorrowRecord record : recordMap.values()) {
            if (record.bookId == bookId && record.userId.equals(userId)) {
                return record.borrowDate.plusDays(14).isBefore(LocalDate.now());
            }
        }
        return false;
    }
}

