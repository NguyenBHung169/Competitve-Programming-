#include<bits/stdc++.h>

using namespace std;

#define L(i, a, b) for(int i = (a) ; i < (int)(b) ; i++)
#define R(i, a, b) for(int i = (a) ; i > (int)(b) ; i--)
#define all(z) (z).begin() , (z).end()

const int MOD = 19901013;
const int N = 25;
const int S = 1 << 20;

int n , m;
char a[N][N];

// way[top][bot][l][r] : free put domino 
// in rectangle row top..bot, column l..r
int way[N][N][N][N];


int dp[2][S];

// f[i] : valid ways for all rectangles on row 1..i
int f[N];

void add(int &x, int y){
	x += y;
	if(x >= MOD) x -= MOD;
}

void sub(int &x , int y){
	x -= y;
	if(x < 0) x += MOD;
}

int mul(int x, int y){
	return 1LL * x * y % MOD;
}

// build way[top][bot][l][r] : rectangle with row top -> bot , column l -> r
void build_way(int l, int r, int top){
	int range = r - l + 1;
	int lim = 1 << range;
	int full = lim - 1;

	memset(dp, 0, sizeof(dp));
	dp[0][full] = 1;

	int cur = 0;

	for(int row = top; row <= n; row++){
		for(int col = l, k = 0; col <= r; col++, k++){
			memset(dp[cur ^ 1], 0, sizeof(int) * lim);

			for(int mask = 0; mask < lim; mask++){
				int val = dp[cur][mask];
				if(val == 0) continue;

				if(a[row][col] == 'x'){
					// ô bị chặn -> ép bit k thành 1 (coi như đã kín)
					add(dp[cur ^ 1][mask | (1 << k)], val);
				}
				else{
					// 1) khép với ô phía trên
					if(((mask >> k) & 1) == 0){
						add(dp[cur ^ 1][mask | (1 << k)], val);
					}

					// 2) ghép ngang với ô bên trái
					if(k > 0 && ((mask >> (k - 1)) & 1) == 0){
						add(dp[cur ^ 1][mask | (1 << k) | (1 << (k - 1))], val);
					}

					// 3) không dùng ô này để khép gì cả, để mở xuống dưới
					add(dp[cur ^ 1][mask & (full ^ (1 << k))], val);
				}
			}
			cur ^= 1;
		}

		
		for(int mask = 0; mask < lim; mask++){
			add(way[top][row][l][r], dp[cur][mask]);
		}
	}
}

int solve_mask(int mask){
	vector<pair<int, int>> seg; // group of the column base on mask

	// m-1 bitmask ,bit c-1 = 1 mean ban the domino on col c and c+1
	int last = 1;
	for(int c = 1; c < m; c++){
		if((mask >> (c - 1)) & 1){
			seg.push_back(make_pair(last, c));
			last = c + 1;
		}
	}
	seg.push_back(make_pair(last, m));

	memset(f, 0, sizeof(f));

	
	f[0] = MOD - 1;

	for(int bot = 1; bot <= n; bot++){
		for(int top = 0; top < bot; top++){
			int val = 1;

			//  ways put freely domino on row top..bot is multiple of putting on dependent blocks
		
			for(int id = 0; id < (int)seg.size(); id++){
				int left = seg[id].first;
				int right = seg[id].second;
				val = mul(val, way[top + 1][bot][left][right]);
			}

			// f[bot] += - f[top] * val
			add(f[bot], MOD - mul(f[top], val));
		}
	}

	return f[n];
}

signed main(){
	ios :: sync_with_stdio(false);
	cin.tie(0);

	cin >> n >> m;
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			cin >> a[i][j];
		}
	}

	for(int i = 1; i <= n; i++){
		for(int r = 1; r <= m; r++){
			for(int l = 1; l <= r; l++){
				build_way(l, r, i);
			}
		}
	}

	int ans = 0;
	for(int mask = 0; mask < (1 << (m - 1)); mask++){
		int value = solve_mask(mask);
		if(__builtin_popcount(mask) & 1) sub(ans, value);
		else add(ans, value);
	}

	cout << ans << endl;
}



/* Algorithms : 
  1. Precalculate number of ways to pick the domino into the rectangle [t....b] * [l....r]
  2. Go over all masks , assume that the index i , representation the column i and column i+1 can not be put 
combine with the  precalcute : way[a][b][c][d] of part 1 . we multiple the way of the  independent groups of columns
  3. We have f[i] : number of valid ways to put on all rectangle on the first i rows , cur = number of valid ways
to put on rectangle on the row x to row y being check . 
  4. Realize that those ways duplicate so we apply PIE 
