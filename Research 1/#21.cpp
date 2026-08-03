#include<bits/stdc++.h>


using namespace std ; 




#define N 1000005
#define inf 10000000009

#define m (l + r)>>1
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int hsh[N] , a[N] , rt[N] ;


struct PST{
	int lz[N << 2] , tr[N << 2] , ls[N << 2] , rs[N << 2];
	int top = 0 ; 
	int cop(int x){
		int ret = ++top ; 
		tr[ret] = top ; 
		ls[ret] = top ; 
		rs[ret] = top ; 
		lz[ret] = top ; 
		return ret ; 
	}
	
	void pushup(int x, int l , int r){
		tr[x] = tr[ls[x]] + tr[rs[x]] + lz[x]*(r - l + 1) ;
	}
	
	int build(int a[] , int l , int r){
		int x = ++top ; 
		if(l == r){
			tr[x] = a[l] ; 
			return x; 
		}
		ls[x] = build(a, l , m) ; 
		rs[x] = build(a, m+1 , r) ;
		pushup(x, l , r) ; 
		return x ;
	}
	
	int upd(int x, int l , int r, int ll , int rr , int v){
		x = cop(x) ;
		if(ll <= l && r <= rr){
			tr[x] += v *(r - l +1) ; 
			lz[x] += v ;
			return x ;
		}
		if(ll <= m ){
			ls[x] = upd(ls[x] , l , m , ll , rr, v) ; 
		}
		if(rr > m ){
			ls[x] = upd(rs[x] , m+1 ,r ,ll , rr , v);
		}
		pushup(x, l ,r);
		return x ;
	}
	
	int query(int x, int l , int r , int ll , int rr, int tag = 0){
		if(ll <= l && r <= rr){
			return tr[x] + tag * ( r - l + 1) ;
		}
		int res = 0 ; 
		if(ll <= m) res += query(ls[x] , l ,m , ll ,rr , tag + lz[x]);
		if(m < rr) res += query(rs[x] , m+1, r, ll , rr, tag+lz[x]);
		return res ; 
	}
	
	
}tr;

int dfn[N] , dfnn ;
vector<int>ve[N] ;
int l[N] , r[N] ;

vector<int>t[N] ;
int mi[25][N] ;

int get(int u, int v){
	if(dfn[u] < dfn[v]){
		return u ;
	}
	return v ; 
}

void dfs(int x, int fa){
	dfn[x] = ++dfnn ; 
	mi[0][dfn[x]] = fa ;
	for(auto y : ve[x]){
		if(y != fa) dfs(y, x) ;
	}
	l[x] = dfn[x] , r[x] = dfnn ;
}

int lca(int u , int v){
	if(u == v) return u ;
	u = dfn[u] , v=  dfn[v] ; 
	if(u > v) swap(u, v) ;
	int g = __lg(v- u);
	return get(mi[g][u+1] ,mi[g][v -(1 << g) +1] );
}

int pwp[N] ; 

signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ;
	int n ; cin >> n ; 
	for(int i = 0 ; i < n -1 ; i++){
		int u , v ; cin >> u >> v ; 
		ve[u].push_back(v) ;
		ve[v].push_back(u) ;
	}
	for(int i =1 ; i < n +1 ; i++){
		int x ; cin >> x ; 
		pwp[i] = x ;
		t[x].push_back(i) ;
	}
	dfs(1, 0) ;
	for(int i = 1; i <= 20 ; i++){
		for(int j =1 1 ; j + (1 << i) <= n ; j++){
			mi[i][j] = get(mi[i-1][j] , mi[i-1][j + (1 <<(i-1))]);
		}
	}
	
	int lst = 0 ; 
	lst = rt[0] = tr.build(a, 1, n) ; 
	for(int i =1 ; i < n +1 ; i++){
		hsh[i] = rng() ;
	}
	for(int i =1 ; i <= n ; i++){
		for(int j : t[i]){
			lst = tr.upd(lst ,1 , n, l[j], r[j] , hsh[i]);
		}
		rt[i] = lst ;
	}
	
	int q; cin >> q ;
	for(int  i = 0 ; i < q ; i++){
		int u1, v1 ,u2 , v2 ;
		cin >> u1 >> v1 >> u2 >> v2 ;
		int l1 = lca(u1, v1) ;
		int l2 = lca(u2, v2) ;
		int vl1 = pwp[l1] , vl2 = pwp[l2] ;
		u1 = dfn[u1] , v1 =dfn[v1] , l1 = dfn[l1];
		u2 = dfn[u2] , v2 = dfn[v2] , l2 = dfn[l2] ;
		
		int l = 0 , r = n ;
		while(l < r){
			int mid = (l + r)>>1 ;
			if(rt[mid] == 0){
				 l = mid ;
				 continue ;
			}
			int col1 = tr.query(rt[mid], 1, n , u1, u1) + tr.query(rt[mid],1,n,v1,v1) + 2*tr.query(rt[mid],1,n,l1,l1)+(vl1 <= mid ? hsh[vl1] : 0);
			int col2 = tr.query(rt[mid],1,n,u2,u2)+tr.query(rt[mid],1,n,v2,v2)+2*tr.query(rt[mid],1,n,l2,l2)+(vl2 <= mid ? hsh[vl2] : 0);
			if(col1 == col2){
				l = mid ; 
				
			}
			else {
				r = mid -1 ;
			}
		}
		cout << l << endl ;
	}
}
