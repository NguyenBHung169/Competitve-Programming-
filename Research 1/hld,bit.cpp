#include<bits/stdc++.h>

using namespace std ; 

const int N = 3e5 + 5 ;
const int inf  = 0x3f3f ; 

int n , q , c[N] ;
int fa[N] ,sz[N] , son[N] ,top[N] ,dfn[N] , rev[N] , out[N];
int tim; 
int w[N] ; 
vector<int>g[N] ; 
set<int>s[N][2] ; 

struct BIT{
	int t[N] ; 
	void add(int x , int v){
		for(; x ; x -= x & -x) t[x] += v ; 
	}
	int ask(int x){
		int res= 0 ; 
		for(; x <=n ; x += x &-x) res += t[x] ; 
		return res ; 
	}
	int sum(int l, int r){
		return ask(l) - ask(r +1) ; 
	}
}bit[2];


void hld(){
	vector<int>ord(n), stk ;
	stk.push_back(1);
	while(!stk.empty()){
		int u= stk.back() ;
		stk.pop_back();
		ord.push_back(u);
		sz[u]=1 ; 
		for(int v :g[u]){
			if(v == fa[u]) continue;
			fa[v] = u ; 
			stk.push_back(v) ; 
		}
	}
	for(int i =n-1; i; i--){
		int u = ord[i] , p = fa[u] ; 
		sz[p] += sz[u] ; 
		if(sz[u] > sz[son[p]]) son[p] = u ; 
	}
	vector<pair<int, int>>stk2;
	stk2.push_back({1,1}) ; 
	while(!stk2.empty()){
		auto [u,tp] = stk2.back() ; 
		stk2.pop_back() ; 
		for(int x = u ; x ; x = son[x]){
			top[x] = tp ; 
			dfn[x] = ++tim ; 
			rev[tim] = x ; 
			for(int v : g[x]){
				if(fa[v]==x && v!= son[x]){
					stk2.push_back({v, v}) ;
				}
			}
		}
	}
	for(int i = 1; i<=n; i++) out[u] = dfn[u] +sz[u]-1 ; 
}

int go(int u , int col){
	while(u){
		auto dif = st[top[u]][col^1] ; 
		if(!s.empty() && *s.begin() <= dfn[u]){
			auto it = dif.upper_bound(dfn[u]);
			return rev[*prev(it)+1] ; 
		}
		int p = fa[top[u]] ; 
		if(c[p] != col) return top[i] ; 
		u= p; 
	}
	return 1 ;
}

void add(int  u ,int v , int col){
	int p = fa[u] ; 
	int z = fa[fa[go(u, col)]] ; 
	bit[col].add(dfn[p] , v) ; 
	bit[col].add(dfn[z] , -v) ; 
}

int ask(int u){
	return w[u] + bit[c[u]].sum(dfn[u] , out[u]) ; 
}

signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ; 
	cin >> n >> q ; 
	for(int i =1 ;i <= n ; i++) cin >> w[i] ; 
	for(int i=1 ; i<=n ; i++) cin >> c[i] ; 
	c[0] = 2 ; 
	for(int i = 1 ; i< n ; i++){
		int u , v ; cin >> u >> v ; 
		g[u].push_back(v) ; 
		g[v].push_back(u) ; 
	}
	hld() ; 
	for(int u = 1 ; u <=n ;u++){
		st[top[u]][c[u]].insert(dfn[u]) ; 
	}
	for(int u =1 ; u <= n ;u++){
		add(u, w[u] ,c[u]) ; 
	}
	while(q--){
		int op , v ; 
		cin >> op >> v ; 
		if(op ==1){
			int s = ask(v) ; 
			add(v, -s, c[v]) ; 
			st[top[v]][c[v]].erase(dfn[v]) ; 
			c[v] ^= 1; 
			st[top[v]][c[v]].insert(dfn[v]) ; 
			add(v, ask(v) , c[v]) ; 
		}
		else if(op ==2){
			int x ; cin >>x ;
			w[v] += x ; 
			add(v, x, c[v]) ; 
		}
		else{
			int u = go(v, c[v]) ; 
			cout << ask(u) << endl ; 
		}
	}
}




