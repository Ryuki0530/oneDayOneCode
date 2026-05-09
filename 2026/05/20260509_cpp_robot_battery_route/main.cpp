#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H,W,B;
    int N;
    cin >> H >> W >> B >> N;

    vector<string> grid(H);
    for(int i = 0; i< H ;i++) cin >> grid[i];

    string commands;
    cin >> commands;

    int r = -1,c = -1;
    for (int i = 0l; i< H; i++){
        for (int j = 0; j < W; j++){
            if (grid[i][j] == 'S'){
                r = i;
                c = j;
            }
        }
    }

    int battery = B;
    long long success_count = 0;

    auto inside = [&](int x, int y){
        return x >= 0 && x < H && y >= 0 && y < W;
    };

	int limit = min<int>(N, (int)commands.size());
    for (int i = 0; i < limit; i++){
        
        if (battery == 0) break;
        
        char cmd = commands[i];
        int nr = r, nc = c;

        if (cmd == 'U') nr--;
        else if (cmd == 'D') nr++;
        else if (cmd == 'L') nc--;
        else if (cmd == 'R') nc++;

        battery--;

        if (inside(nr, nc) && grid[nr][nc] != '#') {
            r = nr;
            c = nc;
            success_count++;
        }

        if (grid[r][c] == 'C'){
            battery = B;
        }

    }

    cout << (r + 1) << ' '
         << (c + 1) << ' '
         << battery << ' '
         << success_count << '\n';

    return 0;

}