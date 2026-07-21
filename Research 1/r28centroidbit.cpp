#include<bits/stdc++.h>

using namespace std ; 

const int N = 10005


struct BIT{
	int n ; 
	vector<int>t; 
	void init(int _n){
		n = _n ; 
		t.assign(n+1 , 0) ;
	}
	
	void add(int x , int v){
		for(; x <= n ; x += x&-x) t[x] += v ;
	}
	int sum(int x){
		int res = 0 ; 
		for(; x ; x -= x&-x) res += t[x] ;
		return res ;
	}
	int sum(int l , int r) return sum(r) - sum(l-1);
}bit;

struct Query{
	int id  , l ,r ; 
};

struct Point{
	int c, mn , mx ; 
};

int n , m ; 
int col[N] , sz[N] , ban[N] , ans[N] ; 
vector<int>g[N] ; 
vector<Query>que[N],qs ; 
vector<Point>pt ; 
vector<int>best  , used ; 

int dfs(int u , int p){
	sz[u] = 1 ; 
	for(auto v : g[u]){
		if(v == p || ban[v]) continue ; 
		sz[u] += dfs(v, u) ; 
	}
	return sz[u] ; 
}

int cen(int u , int p , int tot){
	for(auto v : g[u]){
		if(v == p || ban[v]) continue ; 
		if(sz[v] > tot / 2)return cen(v, u , tot) ; 
	}
	return  u ; 
}
void collect(int u , int p , int mn , int mx){
	pt.push_back({col[u] , mn , mx}) ; 
	for(auto [id , l ,r] : qs[u]){
		if(ans[id] == -1 && mn >= l && mx <= r) que.push_back({id,l,r}) ; 
		
	} 
	for(auto v : g[u]){
		if(v==p||ban[v]) continue ; 
		collect(v, u , min(mn, v) , max(mx, v)) ; 
	}
	
}

bool cmp(Point&a,Point&b){
	return a.mx < b.mx ; 
}
bool cmp2(Query&a,Query&b) return a.r < b.r ; 

void cal(int rt){
	pt.clear() ; 
	que.clear() ;
	collect(rt, 0 ,rt,rt) ; 
	sort(all(pt) , cmp) ;
	sort(all(qu) , cmp2) ; 
	int j = 0 ; 
	for(auto [id, l , r] : que){
		while(j < (int)pt.size() &&pt[j].mx<= r){
			int c = pt[j].c , x = pt[j].mn;
			if(!best[c]) best[c] = x , bit.add(x,1) , used.push_back(c);
			else if(x>best[c]) bit.add(best[c] , -1) ; 
			best[c] = x ; 
			bit.add(best[c] , 1) ; 
			j++;
 		}
 		ans[id] = bit.sum(l, n) ;
	}
	 for(int c : used){
	 	bit.add(best[c] , -1) ; 
	 	best[c] = 0 ; 
	 }
	 used.clear() ; 

}

void cd(int s){
	int tot = dfs(s, 0 ); 
	int rt = cen(s, 0 ,tot) ; 
	cal(rt) ; 
	ban[rt] = 1 ; 
	for(auto v : g[rt]){
		if(ban[v] == 0) cd(v) ; 
	}
}


signed main(){
	ios::sync_with_stdio(0); 
	cin.tie(0) ; 
	cin >> n >> m  ;
	vector<int>a(n) , vl ; 
	for(int i = 0 ; i < n ; i++){
		cin >>a[i] ; 
		vl.push_back(a[i]  ) ;
	}
	sort(all(vl)) ; 
	vl.erase(unique(all(vl)), vl.end()) ;
	for(int i = 1;  i <= n ; i++){
		col[i] = lower_bound(all(vl) , a[i-1]) - vl.begin() +1;
	}
	for(int i=1 ; i < n ;i++){
		int u,v; cin >> u >> v ; 
		g[u].push_back(v) ; 
		g[v].push_back(u) ; 
		
	}
	memset(ans+1,ans+m+1,-1);
	for(int  i =1 ; i <=m ;i++){
		int l , r,x; cin >> l >> r >> x ;
		que[x].push_back({i , l ,r})  ;
	}
	bit.init(n) ; 
	best.assign((int)vl.size()+1,0 ) ;
	used.reserve((int)vl.size()); 
	cd(1) ; 
	for(int i  = 1; i <=m; i++) cout<<ans[i] <<" " ;
	cout << endl ; 
}













