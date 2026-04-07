#include<bits/stdc++.h>
using namespace std;

#define L(i, a, b) for(int i = (a) ; i < (int)(b) ; i++)
#define R(i, a, b) for(int i = (a) ; i > (int)(b) ; i--)
#define all(x) (x).begin(), (x).end()

const int N = 205;                 
const int MAXV = N * N;

vector<int> g[MAXV];
char grid[N][N];
int col[N][N];
bool vis[MAXV], block[MAXV], win[MAXV];
int match[MAXV];
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int n, m, ans;

void add_edge(int x, int y){
    g[x].push_back(y);
    g[y].push_back(x);
}

int getid(int x, int y){
    return x * m + y;
}

bool check(int x, int y){
    if(x < 0 || y < 0 || x >= n || y >= m) return false;
    if(col[x][y] == 0) return false;
    return true;
}

// hungarian 
bool matching(int u){
    for(auto v : g[u]){
        if(block[v]) continue;
        if(vis[v]) continue;
        vis[v] = true;

        if(!match[v] || matching(match[v])){
            match[v] = u;
            match[u] = v;
            return true;
        }
    }
    return false;
}


bool dfs(int u){
    return matching(u);
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m;

    L(i, 0, n){
        L(j, 0, m){
            cin >> grid[i][j];
        }
    }

  int sx , sy ;

   
    L(i, 0, n){
        L(j, 0, m){
            if(grid[i][j] == '.'){
                sx = i;
                sy = j;
                col[i][j] = 1;
            }
            else if(grid[i][j] == 'O'){
                col[i][j] = 1;
            }
            else{
                col[i][j] = 0;
            }
        }
    }

    
    L(i, 0, n){
        L(j, 0, m){
            if(col[i][j] == 0) continue;

            int id = getid(i, j);
            L(dir, 0, 4){
                int nx = i + dx[dir];
                int ny = j + dy[dir];
                if(!check(nx, ny)) continue;

                int nid = getid(nx, ny);
                add_edge(id, nid);
            }
        }
    }

    L(i, 0, n){
        L(j, 0, m){
            if(col[i][j] == 0) continue;
            if((i + j) % 2 == 1) continue;   // 
            int id = getid(i, j);
            memset(vis, 0, sizeof(vis));
            if(matching(id)) ans++;
        }
    }

    int op; cin >> op;
    L(i, 0, 2 * op){
        int id = getid(sx, sy);
        block[id] = true;

        if(!match[id]){
            win[i] = false;
        }
        else{
            int nid = match[id];
            match[id] = 0;
            match[nid] = 0;

            memset(vis, 0, sizeof(vis));
            if(dfs(nid)){
                win[i] = false;
            }
            else{
                win[i] = true;
            }
        }
    }

    vector<int> res;
    int cnt = 0;

    L(i, 0, op){
        if(win[2 * i] == 1 && win[2 * i + 1] == 1){
            res.push_back(i);
            cnt++;
        }
    }

    cout << cnt << "\n";
    for(auto wrong : res){
        cout << wrong << " ";
    }
    cout << "\n";
}
