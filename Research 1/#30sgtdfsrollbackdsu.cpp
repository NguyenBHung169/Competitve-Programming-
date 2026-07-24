
#include<bits/stdc++.h>

using namespace std ; 

#define N 20005

int n , m , a[N] , b[N] , u[N] , v[N] ; 

vector>int>va[N] , vn[N] ; 

bool ok , vis[N] ; 

#define ls (x << 1)
#define rs (x << 1 | 1)
#define mid  (l + r >> 1) 


vector<int>e[N << 2] ; 

int f[N] , st[N] , siz[N] , tp ; 

void update(int x , int l , int r, int ql , int qr, int id){
	if(ql > qr) return ; 
	if(ql <= l && qr >= r){
		e[x].push_back(id) ; 
		return ; 
	}
	if(ql <= mid) update(ls , l , mid , ql ,qr , id) ; 
	if(qr > mid) update(rs , mid+1 ,r ,ql ,qr ,id) ;
	
}

int find(int x){
	return f[x] ^ x ? find(f[x]) : x ; 
}

void merge(int u , int v){
	int fu = find(u) , fv = find(v) ; 
	if(fu == fv) return ;
	if(siz[fu] < siz[fv]) swap(fu ,fv) ; 
    f[fv] = fu  ; 
	siz[fu] += siz[fv] ; 
	st[++tp] = fv ;  
}

void query(int x , int l , int r){
	int pre = tp ; 
	for(auto id : e[x]){
		merge(u[id] , v[id]) ; 
	}
	if(l == r){
		for(auto i : va[l]) vis[find(i)] = 1 ; 
		for(auto j : vb[l]) if(!vis[find(j)]) ok = 0 ;
		for(auto j : va[l]) vis[find(i)] = 0 ; 
	}
	else{
		query(ls, l , mid) ; 
		query(rs , mid + 1 , r) ; 
	}
	while(tp > pre){
		int t = st[tp] ; 
		siz[f[t]] -= siz[t] ; 
		f[t] = t; 
		tp-- ; 
	}
}
void clear(int x, int l  , int r){
	e[x].clear() ; 
	if(l != r) clear(ls, l , mid) , clear(rs, mid+1,r) ; 
}

signed main(){
	ios :: sync_with_stdio(false) ; 
    cin.tie(0) ; 
	cin >> n >> m ; 
	for(int i = 1 ; i <= n ; i++){
		cin >> a[i] ; 
		va[a[i]].push_back(i) ; 
	}
	for(int i = 1 ; i <= n ; i++){
		cin >> b[i] ; 
		vb[b[i]].push_back(i) ; 
	}
	for(int i = 1 ; i  <= m ; i++){
		cin >> u[i] >> v[i] ; 
		update(1, 1, n , max(b[u[i]] , b[v[i]]) , min(a[u[i]] , a[v[i]]);)
	}
	for(int i = 1 ; i <= n ; i++) f[i] = i , siz[i] = 1 ; 
	ok = 1 ; 
	query(1, 1, n) ; 
	cout << ok << endl ; 
	// clear(1,1,n) ; 
}















