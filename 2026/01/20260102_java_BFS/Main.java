import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int H = sc.nextInt(), W = sc.nextInt();
        sc.nextLine();
        char[][] maze = new char[H][W];
        int[][] dist = new int[H][W];
        int sh = -1, sw = -1, gh = -1, gw = -1;

        for (int i = 0; i < H; i++) {
            String line = sc.nextLine();
            maze[i] = line.toCharArray();
            Arrays.fill(dist[i], -1);
            for (int j = 0; j < W; j++) {
                if (maze[i][j] == 'S') { sh = i; sw = j; }
                if (maze[i][j] == 'G') { gh = i; gw = j; }
            }
        }

        int[] dh = {1, -1, 0, 0}, dw = {0, 0, 1, -1};
        ArrayDeque<int[]> q = new ArrayDeque<>();
        dist[sh][sw] = 0;
        q.add(new int[]{sh, sw});

        while (!q.isEmpty()) {
            int[] cur = q.poll();
            int h = cur[0], w = cur[1];
            for (int d = 0; d < 4; d++) {
                int nh = h + dh[d], nw = w + dw[d];
                if (nh < 0 || nh >= H || nw < 0 || nw >= W) continue;
                if (maze[nh][nw] == '#' || dist[nh][nw] != -1) continue;
                dist[nh][nw] = dist[h][w] + 1;
                q.add(new int[]{nh, nw});
            }
        }

        System.out.println(dist[gh][gw]);
    }
}