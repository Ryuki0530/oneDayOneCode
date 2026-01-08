import java.io.*;
import java.util.*;

public class Main {
    private static final int INF = Integer.MAX_VALUE / 4;
    private static final int[] DR = {-1, 0, 1, 0};
    private static final int[] DC = {0, 1, 0, -1};

    public static void main(String[] args) throws Exception {
        FastScanner fs = new FastScanner(System.in);
        int h = fs.nextInt();
        int w = fs.nextInt();
        char[][] grid = new char[h][];
        int sr = -1, sc = -1, gr = -1, gc = -1;
        for (int r = 0; r < h; r++) {
            grid[r] = fs.next().toCharArray();
            for (int c = 0; c < w; c++) {
                char ch = grid[r][c];
                if (ch == 'S') {
                    sr = r;
                    sc = c;
                } else if (ch == 'G') {
                    gr = r;
                    gc = c;
                }
            }
        }

        int[][] dist = new int[h][w];
        for (int[] row : dist) {
            Arrays.fill(row, INF);
        }
        ArrayDeque<int[]> dq = new ArrayDeque<>();
        dist[sr][sc] = 0;
        dq.offerFirst(new int[]{sr, sc});

        while (!dq.isEmpty()) {
            int[] cur = dq.pollFirst();
            int r = cur[0], c = cur[1];
            if (r == gr && c == gc) {
                break;
            }
            for (int d = 0; d < 4; d++) {
                int nr = r + DR[d];
                int nc = c + DC[d];
                if (nr < 0 || nr >= h || nc < 0 || nc >= w) {
                    continue;
                }
                int cost = grid[nr][nc] == '#' ? 1 : 0;
                int nd = dist[r][c] + cost;
                if (nd < dist[nr][nc]) {
                    dist[nr][nc] = nd;
                    if (cost == 0) {
                        dq.offerFirst(new int[]{nr, nc});
                    } else {
                        dq.offerLast(new int[]{nr, nc});
                    }
                }
            }
        }

        int ans = dist[gr][gc];
        System.out.println(ans == INF ? -1 : ans);
    }

    private static class FastScanner {
        private final BufferedReader br;
        private StringTokenizer st;

        FastScanner(InputStream in) {
            br = new BufferedReader(new InputStreamReader(in));
        }

        String next() throws IOException {
            while (st == null || !st.hasMoreElements()) {
                String line = br.readLine();
                if (line == null) return null;
                st = new StringTokenizer(line);
            }
            return st.nextToken();
        }

        int nextInt() throws IOException {
            return Integer.parseInt(next());
        }
    }
}