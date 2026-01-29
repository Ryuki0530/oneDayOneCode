import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int K = sc.nextInt();
        int[] A = new int[N];
        for (int i = 0; i < N; i++) {
            A[i] = sc.nextInt();
        }

        Map<Integer, Integer> count = new HashMap<>();
        int maxLen = 0;
        int left = 0;

        for (int right = 0; right < N; right++) {
            count.put(A[right], count.getOrDefault(A[right], 0) + 1);

            while (count.size() > K) {
                int leftVal = A[left];
                count.put(leftVal, count.get(leftVal) - 1);
                if (count.get(leftVal) == 0) {
                    count.remove(leftVal);
                }
                left++;
            }

            maxLen = Math.max(maxLen, right - left + 1);
        }

        System.out.println(maxLen);
    }
}