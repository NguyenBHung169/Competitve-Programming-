#include<bits/stdc++.h>


using namespace std ;

struct ST {
  #define lc (n << 1)
  #define rc ((n << 1) | 1)
  long long t[4 * N], lazy[4 * N];

  ST() {
    memset(t,    0, sizeof t);
    memset(lazy, 0, sizeof lazy);
  }

  // Đẩy tag lazy xuống con và cập nhật giá trị nút
  inline void push(int n, int b, int e) {
    if (lazy[n] == 0) return;
    t[n] += lazy[n] * (e - b + 1);
    if (b != e) {
      lazy[lc] += lazy[n];
      lazy[rc] += lazy[n];
    }
    lazy[n] = 0;
  }

  // Gộp kết quả hai con (sum)
  inline long long combine(long long L, long long R) {
    return L + R;
  }

  // Cập nhật giá trị nút từ hai con
  inline void pull(int n) {
    t[n] = t[lc] + t[rc];
  }

  // Xây cây từ a[b..e]
  void build(int n, int b, int e) {
    lazy[n] = 0;
    if (b == e) {
      t[n] = a[b];
      return;
    }
    int mid = (b + e) >> 1;
    build(lc, b, mid);
    build(rc, mid + 1, e);
    pull(n);
  }

  // Cộng v vào toàn bộ đoạn [i..j]
  void upd(int n, int b, int e, int i, int j, long long v) {
    push(n, b, e);
    if (j < b || e < i) return;        // không giao nhau
    if (i <= b && e <= j) {
      lazy[n] = v;                      // đánh dấu lazy
      push(n, b, e);
      return;
    }
    int mid = (b + e) >> 1;
    upd(lc, b, mid, i, j, v);
    upd(rc, mid + 1, e, i, j, v);
    pull(n);
  }

  // Truy vấn tổng trên đoạn [i..j]
  long long query(int n, int b, int e, int i, int j) {
    push(n, b, e);
    if (j < b || e < i) return 0;      // phần tử trung tính của sum
    if (i <= b && e <= j) return t[n];
    int mid = (b + e) >> 1;
    return combine(
      query(lc, b, mid, i, j),
      query(rc, mid + 1, e, i, j)
    );
  }
}ST[4*N];

void dfs(int u , int fa = -1 , int ww = -inf){
	height[u] = 1;
	for(auto [to , w] : g[u]){
		if(vis[to]) continue ;
		vis[to] = 1; 
		heavy[u] = to ;
		next[to] = u ;
		prev[u] = to ;
		hev_w[u] = w ;
		dfs(to , u , w);
		height[u] = max(height[to]+1) ;
	}
}
int p ;

void dp(int u, int fa=-1){
	int pu = pos[u] ;
	// 
	if(heavy[pu]){
		dp(heavy[pu] , pu) ;
		for(auto [v, w] : g[u]){
			//dp(v, u);
			g[pu] = g[pu+1] - p + hev_w[pu] ;
			f[pu] -= g[pu] ;
		}
		upd(1,1,n,1,f[pu]);
	}
	for(auto [nxt , w] : g[u]){
		if(nxt == fa || nxt == heavy[pu]) continue ; 
		dp(nxt , u);
		int pv = pos[v];
	//	int max_other_side = query(1,1,n,pu , pv) ;
	    g[pu] = w - p + g[pv] + f[gv] + mx_other
		L(i, 0, height[u]){
	    	int mx_other = query(1,1,n,pu + L , pu + min(R, height[u])) ;
	    	ans = max(ans, mx_other + w - p + f[pv] - f[pu]	) ;
      }
       L(i , 0 , height[u]) if(f[pu+i] < f[pv] +w - p -g[pu] + g[pv] ){ f[pu+i] < f[pv] +w-p+g[pv]-g[pu] ;}
	}
}

bool check(int mid){
	memset(vis,0,sizeof(vis));
	int p = mid ;
	int v =dp(p, -1);
	if(v <0) return false;
	return true;
}

signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ;
	cin >> n  >> L >> R ; 
	int mxw  = -1 ;
	L(i, 0, n-1){
		int u , v , w ; cin >> u >> v >> w ;
		g[u].push_back(make_pair(v, w)) ;
		g[v].push_back(make_pair(u, w)) ;
		mxw = max(mxw, w) ;
	}
	dfs(0,-1, g[0].second() );
	
	int l = 0 , r = mxw ;
	int ans = -1
	while(l < r){
		int mid = l+r >>1;
		if(check(mid)) r = mid-1, ans = mid ;
		else l = mid+1 ;
	}
	cout << fixed << setprecision(3) << ans ;
}




/* Algorithm : 

 1. Having dp i,j : calculate path length j considering only in the i's subtrees 
 2. Realize that the complexity is O(n^2) 
 3 . Apply longchain decomposition to deal with the heavy child and light child . Heavy child in O (sqrt n ) and light child in O(n)
 4. With every light child of a part of subtree , try to merge it with other part of subtree ( not exactly the light child ) 
*/















