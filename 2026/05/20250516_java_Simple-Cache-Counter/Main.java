import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringTokenizer st = new StringTokenizer(br.readLine());
        int N = Integer.parseInt(st.nextToken());
        int K = Integer.parseInt(st.nextToken());
        st = new StringTokenizer(br.readLine());

        Queue<Integer> queue = new LinkedList<>();
        Set<Integer> cache = new HashSet<>();

        int hit = 0, miss = 0;
        for (int i = 0; i < N; i++) {
            int a = Integer.parseInt(st.nextToken());
            if (cache.contains(a)) {
                hit++;
            } else {
                miss++;
                if (cache.size() >= K) {
                    if (!queue.isEmpty()) {
                        int old = queue.poll();
                        cache.remove(old);
                    }
                }
                if (K > 0) {
                    queue.offer(a);
                    cache.add(a);
                }
            }
        }

        System.out.println(hit + " " + miss);
    }
}
