import java.io.*;
import java.util.*;

public class Main {
    static int H, W;
    static char[][] maze;
    static int[][] dist;
    static int[][] prevH, prevW;
    static int[] dh = {-1, 1, 0, 0};
    static int[] dw = {0, 0, -1, 1};
    
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringTokenizer st = new StringTokenizer(br.readLine());
        H = Integer.parseInt(st.nextToken());
        W = Integer.parseInt(st.nextToken());
        
        maze = new char[H][W];
        dist = new int[H][W];
        prevH = new int[H][W];
        prevW = new int[H][W];
        
        int sh = -1, sw = -1, gh = -1, gw = -1;
        
        for (int i = 0; i < H; i++) {
            String line = br.readLine();
            for (int j = 0; j < W; j++) {
                maze[i][j] = line.charAt(j);
                if (maze[i][j] == 'S') {
                    sh = i;
                    sw = j;
                } else if (maze[i][j] == 'G') {
                    gh = i;
                    gw = j;
                }
            }
            Arrays.fill(dist[i], -1);
        }
        
        bfs(sh, sw);
        
        if (dist[gh][gw] == -1) {
            System.out.println(-1);
        } else {
            System.out.println(dist[gh][gw]);
            restorePath(gh, gw, sh, sw);
            for (int i = 0; i < H; i++) {
                for (int j = 0; j < W; j++) {
                    System.out.print(maze[i][j]);
                }
                System.out.println();
            }
        }
    }
    
    static void bfs(int sh, int sw) {
        ArrayDeque<int[]> queue = new ArrayDeque<>();
        queue.add(new int[]{sh, sw});
        dist[sh][sw] = 0;
        prevH[sh][sw] = -1;
        prevW[sh][sw] = -1;
        
        while (!queue.isEmpty()) {
            int[] cur = queue.poll();
            int h = cur[0], w = cur[1];
            
            for (int i = 0; i < 4; i++) {
                int nh = h + dh[i];
                int nw = w + dw[i];
                
                if (nh >= 0 && nh < H && nw >= 0 && nw < W && 
                    maze[nh][nw] != '#' && dist[nh][nw] == -1) {
                    dist[nh][nw] = dist[h][w] + 1;
                    prevH[nh][nw] = h;
                    prevW[nh][nw] = w;
                    queue.add(new int[]{nh, nw});
                }
            }
        }
    }
    
    static void restorePath(int gh, int gw, int sh, int sw) {
        int h = gh, w = gw;
        while (h != sh || w != sw) {
            if (maze[h][w] != 'G') {
                maze[h][w] = '*';
            }
            int nh = prevH[h][w];
            int nw = prevW[h][w];
            h = nh;
            w = nw;
        }
    }
}