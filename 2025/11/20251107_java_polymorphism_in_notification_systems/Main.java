public class Main {
    public static void main(String[] args) {
        String message = "！！！！通知内容！！！！";
        Notifier[] notifiers = {
            new EmailNotifier(),
            new SMSNotifier(),
            new AppNotifier()
        };
        for (Notifier notifier : notifiers) {
            notifier.notifyUser(message);
        }
    }
}

interface Notifier {
    void notifyUser(String message);
}

class EmailNotifier implements Notifier {
    @Override
    public void notifyUser(String message) {
        System.out.println("メールで通知 :" + message);
    }
}

class SMSNotifier implements Notifier {
    @Override
    public void notifyUser(String message) {
        System.out.println("SMSで通知 :" + message);
    }
}

class AppNotifier implements Notifier {
    @Override
    public void notifyUser(String message) {
        System.out.println("アプリ通知 :" + message);
    }
}
