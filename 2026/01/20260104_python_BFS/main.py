from collections import deque

def solve():
    H, W = map(int, input().split())
    grid = [input().strip() for _ in range(H)]
    
    start = goal = None
    for i in range(H):
        for j in range(W):
            if grid[i][j] == 'S':
                start = (i, j)
            elif grid[i][j] == 'G':
                goal = (i, j)
    
    dist = [[-1] * W for _ in range(H)]
    dist[start[0]][start[1]] = 0
    
    queue = deque([start])
    directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
    
    while queue:
        r, c = queue.popleft()
        
        if (r, c) == goal:
            print(dist[r][c])
            return
        
        for dr, dc in directions:
            nr, nc = r + dr, c + dc
            
            if 0 <= nr < H and 0 <= nc < W and dist[nr][nc] == -1 and grid[nr][nc] != '#':
                dist[nr][nc] = dist[r][c] + 1
                queue.append((nr, nc))
    
    print(-1)

solve()