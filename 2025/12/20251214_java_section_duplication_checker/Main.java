public class Main {
    public static void main(String[] args) {
        int[][] intervals1 = {{1, 3}, {5, 7}, {2, 4}};
        System.out.println(hasOverlap(intervals1));
        
        int[][] intervals2 = {{1, 2}, {3, 4}, {5, 6}};
        System.out.println(hasOverlap(intervals2));
    }
    
    public static boolean hasOverlap(int[][] intervals) {
        if (intervals == null || intervals.length < 2) {
            return false;
        }
        
        java.util.Arrays.sort(intervals, (a, b) -> a[0] - b[0]);
        
        for (int i = 0; i < intervals.length - 1; i++) {
            if (intervals[i][1] >= intervals[i + 1][0]) {
                return true;
            }
        }
        
        return false;
    }
}