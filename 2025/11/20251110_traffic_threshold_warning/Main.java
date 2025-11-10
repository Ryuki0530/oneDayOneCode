import java.util.*;
import java.time.LocalDateTime;

class OneTrafficInfo {
    private final LocalDateTime time;
    private final int volume;

    public OneTrafficInfo(LocalDateTime time, int volume) {
        this.time = time;
        this.volume = volume;
    }

    public LocalDateTime getTime() {
        return time;
    }

    public int getVolume() {
        return volume;
    }
}


class TrafficManager {
    private Queue<OneTrafficInfo> TrafficLog;
    private int currentSum;
    private int threshold;
    private int windowSizeMin;
    private LocalDateTime threshholdTime;
    private LocalDateTime lastAddedTime;
    
    public TrafficManager(int windowSizeMin,int threshold) {
        this.TrafficLog = new LinkedList<>();
        this.windowSizeMin = windowSizeMin;
        this.currentSum = 0;
        this.threshold = threshold;
        this.threshholdTime = null;
        this.lastAddedTime = null;
    }
    
    public int addInfoAndIsSafe(OneTrafficInfo info) {
        /*
        正常に追加され、問題が無ければ0
        正常に追加され、追加されたOneTrafficInfoのlocalDateTimeから
        直近windowSizeMin分前までの合計通信料がthresholdを超えていれば1
        異常が発生し、追加されなければ-1
        例えば、windowSizeMinが5分、thresholdが1000MBの場合
        1. 現在時刻が12:00で、過去5分
        2. 12:00に200MBの通信が発生し追加
        3. 12:01に300MBの通信が発生し追加
        4. 12:02に600MBの通信が発生し追加
        5. 12:03に100MBの通信が発生し追
        この場合、12:02に追加された600MBの通信を含む
        直近5分間の通信量は200+300+600=1100MBとなり、thresholdを超えているため4件目のデータ入力時に1を返す。
        */
        
        LocalDateTime thresholdTime = info.getTime().minusMinutes(windowSizeMin);
        TrafficLog.add(info);
        lastAddedTime = info.getTime();
        currentSum += info.getVolume();
        while (!TrafficLog.isEmpty() && TrafficLog.peek().getTime().isBefore(thresholdTime)) {
            OneTrafficInfo oldInfo = TrafficLog.poll();
            currentSum -= oldInfo.getVolume();
        }
        if (currentSum > threshold) {
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
        int T = sc.nextInt();
        sc.nextLine();

        int log_len = N;
        int check_window_min = W;
        int threshold = T;
        boolean DEBUG = false;

        TrafficManager manager = new TrafficManager(check_window_min, threshold);
        
        for (int i = 0; i < log_len; i++) {
            String input = sc.nextLine();
            String[] parts = input.split(" ");
            String DateStr = parts[0];
            String timeStr = DateStr + "T" + parts[1];
            int volume = Integer.parseInt(parts[2]);
            LocalDateTime time = LocalDateTime.parse(timeStr);
            OneTrafficInfo info = new OneTrafficInfo(time, volume);
            int result = manager.addInfoAndIsSafe(info);
            if (DEBUG) {
                System.out.println("Added: " + timeStr + " Volume: " + volume + " Result: " + result);
            }
        }
        sc.close(); 
        if (manager.isSafe()) {
            System.out.println("OK");
        } else {
            System.out.println(manager.getThreshholdTime());
        }
    }
}
