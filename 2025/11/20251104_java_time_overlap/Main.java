import java.util.*;

public class Main {
    static class Interval {
        int start;
        int end;
        
        Interval(int start, int end) {
            this.start = start;
            this.end = end;
        }
    }
    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        int n = sc.nextInt();
        List<Interval> intervals = new ArrayList<>();
        
        for (int i = 0; i < n; i++) {
            int start = sc.nextInt();
            int end = sc.nextInt();
            intervals.add(new Interval(start, end));
        }
        
        boolean hasOverlap = false;
        int totalOverlapDuration = 0;
        
        // Check all pairs of intervals
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                Interval a = intervals.get(i);
                Interval b = intervals.get(j);
                
                // Check if intervals overlap: a.start < b.end && b.start < a.end
                if (a.start < b.end && b.start < a.end) {
                    hasOverlap = true;
                    
                    // Calculate overlap duration for advanced challenge
                    int overlapStart = Math.max(a.start, b.start);
                    int overlapEnd = Math.min(a.end, b.end);
                    totalOverlapDuration += (overlapEnd - overlapStart);
                }
            }
        }
        
        if (hasOverlap) {
            System.out.println("Overlap");
            System.out.println("Overlap Duration: " + totalOverlapDuration);
        } else {
            System.out.println("No Overlap");
        }
        
        sc.close();
    }
}