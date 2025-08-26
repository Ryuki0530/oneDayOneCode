abstract class ApiNegotiator {
    protected String city;
    protected int days;
    protected String format;
    protected String out;
    protected boolean debug;

    public ApiNegotiator(String city, int days, String format, String out, boolean debug) {
        this.city = city;
        this.days = days;
        this.format = format;
        this.out = out;
        this.debug = debug;
    }

    // 共通の呼び出し手順
    public final void call() {
        if (debug) log("Start negotiation...");
        negotiate();
        if (debug) log("Negotiation finished.");
    }

    // サブクラスでAPIごとの処理を実装
    protected abstract void negotiate();

    // 共通のデバッグ出力
    protected void log(String msg) {
        System.out.println("[DEBUG] " + msg);
    }

    // 共通の出力処理
    protected void output(String result) {
        if (out != null) {
            // ファイル出力処理
        } else {
            System.out.println(result);
        }
    }
}

public class Main {

    private String city = null;
    private int days = 3;
    private String format = "table";
    private String out = null;
    private boolean debug = false;


    public static void main(String[] args) {
        System.out.println("Hello, OneDayOneCode!");
        Main main = new Main();
        main.argsParser(args);
        System.out.println("city: " + main.city);
        System.out.println("days: " + main.days);
        System.out.println("format: " + main.format);
        System.out.println("out: " + main.out);
        System.out.println("debug: " + main.debug);
    }

    private void argsParser(String[] args) {
        
        String city = null;
        int days = 3;
        String format = "table";
        String out = null;
        boolean debug = false;

        for (int i = 0; i < args.length; i++) {
            switch (args[i]) {
            case "--city":
                if (i + 1 < args.length) {
                city = args[++i];
                }
                break;
            case "--days":
                if (i + 1 < args.length) {
                try {
                    days = Integer.parseInt(args[++i]);
                } catch (NumberFormatException e) {
                    days = 3;
                }
                }
                break;
            case "--format":
                if (i + 1 < args.length) {
                String f = args[++i];
                if (f.equals("table") || f.equals("csv") || f.equals("json")) {
                    format = f;
                }
                }
                break;
            case "--out":
                if (i + 1 < args.length) {
                out = args[++i];
                }
                break;
            case "--debug":
                debug = true;
                break;
            }
        }

        this.city = city;
        this.days = days;
        this.format = format;
        this.out = out;
        this.debug = debug;
    }
}