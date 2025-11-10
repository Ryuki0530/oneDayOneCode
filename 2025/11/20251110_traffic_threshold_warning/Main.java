// Main.java
import java.util.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

class OneTrafficInfo {
    private final LocalDateTime time;
    // ★変更: volumeは合計でintを超える可能性があるためlongに
    private final long volume;

    // ★変更: 引数型をlongへ
    public OneTrafficInfo(LocalDateTime time, long volume) {
        this.time = time;
        this.volume = volume;
    }

    public LocalDateTime getTime() {
        return time;
    }

    public long getVolume() {
        return volume;
    }
}

class TrafficManager {
    private Queue<OneTrafficInfo> TrafficLog;
    // ★変更: 合計はlongで持つ
    private long currentSum;
    // ★変更: しきい値もlong
    private long threshold;
    private int windowSizeMin;
    private LocalDateTime threshholdTime; // NOTE: スペルは元のまま保持
    private LocalDateTime lastAddedTime;

    public TrafficManager(int windowSizeMin, long threshold) {
        this.TrafficLog = new LinkedList<>();
        this.windowSizeMin = windowSizeMin;
        this.currentSum = 0L; // ★変更: long初期化
        this.threshold = threshold;
        this.threshholdTime = null;
        this.lastAddedTime = null;
    }

    /**
     * 正常に追加 & 未到達: 0
     * 正常に追加 & 到達(初回かは呼び出し側で制御): 1
     * 異常: -1
     */
    public int addInfoAndIsSafe(OneTrafficInfo info) {
        // ★変更: ウィンドウ境界のオフバイワン修正
        // 仕様: [t - (W - 1) 分, t] の閉区間
        // → 捨てる条件: old.time < t - (W - 1) 分
        LocalDateTime startInclusive = info.getTime().minusMinutes(Math.max(0, windowSizeMin - 1));

        TrafficLog.add(info);
        lastAddedTime = info.getTime();
        currentSum += info.getVolume();

        while (!TrafficLog.isEmpty() && TrafficLog.peek().getTime().isBefore(startInclusive)) {
            OneTrafficInfo oldInfo = TrafficLog.poll();
            currentSum -= oldInfo.getVolume();
        }

        // ★変更: ">" を "?" に
        if (currentSum >= threshold) {
            // ★補足: 最初の検知時刻は呼び出し側で即時出力＆終了するため、
            // ここでは記録だけ残す（互換性維持のため既存フィールドに格納）
            threshholdTime = info.getTime();
            return 1;
        }
        return 0;
    }

    public boolean isSafe() {
        return threshholdTime == null;
    }

    public LocalDateTime getThreshholdTime() {
        return threshholdTime;
    }

    public LocalDateTime getLastAddedTime() {
        return lastAddedTime;
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int N = sc.nextInt();
        int W = sc.nextInt();
        // ★変更: Tはlongで受ける
        long T = sc.nextLong();
        sc.nextLine();

        boolean DEBUG = false;
        DateTimeFormatter outFmt = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");

        TrafficManager manager = new TrafficManager(W, T);

        for (int i = 0; i < N; i++) {
            String input = sc.nextLine();
            // 行は "YYYY-MM-DD HH:MM bytes"
            String[] parts = input.split(" ");
            String timeStr = parts[0] + "T" + parts[1];

            // ★変更: volumeをlongでパース
            long volume = Long.parseLong(parts[2]);

            LocalDateTime time = LocalDateTime.parse(timeStr); // ISO-LOCAL-DATE-TIME 形式
            OneTrafficInfo info = new OneTrafficInfo(time, volume);

            int result = manager.addInfoAndIsSafe(info);

            if (DEBUG) {
                System.out.println("Added: " + parts[0] + " " + parts[1] + " Volume: " + volume + " Result: " + result);
            }

            // ★変更: 「最初に到達した時刻」を検出した瞬間に出力して終了
            if (result == 1) {
                System.out.println(manager.getThreshholdTime().format(outFmt));
                sc.close();
                return;
            }
        }
        sc.close();

        // ★変更: 最後まで未到達ならOK
        System.out.println("OK");
    }
}
