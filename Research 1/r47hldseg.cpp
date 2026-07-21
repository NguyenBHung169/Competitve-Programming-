#include<bits/stdc++.h>


using namespace std ; 


#define N 100005
#define mod 100000009

int head[N] , to[N<<1] , nxt[N<<1] , tot ; 
int d[N] , s[N] , f[N] , hs[N] ; 

void add(int x , int y){
	to[++tot] = y ; 
	nxt[tot] = head[x] ; 
	head[x] = tot ; 
}

void dfs1(int x){
	s[x] = 1 ; 
	for(int i = head[x] ; i ; i = nxt[i]){
		if(d[to[i]]) continue ; 
		d[to[i]] = d[x] + 1 ; 
		f[to[i]] = x ; 
		dfs1(to[i]) ; 
		s[x] += s[to[i]] ; 
		if(s[hs[x]] < s[to[i]]) hs[x] = to[i] ;  
	}
}
int top[N] , dfn[N] , rk[N] , res ; 

void dfs2(int x,  int t){
	top[x] = t ; 
	dfn[x] = ++res ; 
	rk[res] = x ; 
	if(hs[x]) dfs2(hs[x] , t) ; 
	for(int i = head[x] ; i ; i = nxt[i]){
		if(to[i] != f[x] && to[i] != hs[x]) dfs2(to[i] , to[i]); 
	}

}

struct tree{
	int k , dk ,ddk ,f ; 
	int ls, rs , v[2] ; 
	tree(){
		 k = dk = ddk = ls = rs = f = v[0] = v[1] = 0 ;
	}
}t[N <<2];

tree operator +(tree a , tree b){
	tree c ; 
	c.k = (a.k + b.k) % mod ; 
	c.dk =(a.dk + b.dk) %mod ;
	c.ddk = (a.ddk + b.ddk) % mod ; 
	return c ; 
}

int D(int l , int r) return (e[r] - e[l-1]) % mod ; 
int DD(int l , int r) return (ee[r] - ee[l-1]) % mod ; 

void build(int &tp , int l , int r){
	tp = ++cnt ; 
	if(l == r){
		int x = rk[l] ;
		t[tp].k = (a[x])%mod ; 
		t[tp].dk = (a[x] *d[x])%mod; 
		t[tp].ddk = (a[x]*d[x]*d[x]) %mod; 
		return; 
	}
	int mid = l + r >> 1 ; 
	build(t[tp].ls ,  l , mid) ; 
	build(t[tp].rs , mid + 1, r) ; 
	t[tp].k = (t[t[tp].ls ].k + t[t[tp].rs ].k) % mod ;
	t[tp].dk = (t[t[tp].ls ].dk + t[t[tp].rs ].dk) % mod ;
	t[tp].ddk = (t[t[tp].ls ].ddk + t[t[tp].rs ].ddk) % mod ;
}

void pushup(tree &tp , int l , int r , int k){
	tp.k = (tp.k + k * (r - l + 1)% mod) %mod ;
	tp.dk = (tp.dk + k *D(l ,r)% mod) % mod ; 
	tp.ddk = (tp.ddl + k *DD(l ,r)% mod) % mod ; 
}
int copy(int tp){
	t[++cnt] = t[tp] ; 
	return cnt ; 
}

void update(int tp , int  l ,int r, int ql , int qr , int  k){
	pushup(t[tp] , max(l ,ql) , min(r,qr) , k) ; 
	if(ql <= l && qr >= r){
		t[tp].f = (t[tp].f + k) % mod ; 
		return ; 
	}
	int mid = l + r >> 1 ; 
	if(ql <= mid){
		if(t[tp].v[0]){
			t[tp].ls = copy(t[tp].ls ) ;
			t[t[tp].ls].v[0] = 1 ; 
			t[t[tp].ls ].v1 = 1 ;
			t[tp].v0= 0 ; 
		}
		update(t[tp].ls  , l ,mid , ql ,qr , k);
	}
	if(qr > mid){
		if(t[tp].v[1]){
			t[tp].rs = copy(t[tp].rs ) ; 
			t[t[tp].rs ].v[0] = 1 ; 
			t[t[tp].rs ].v[1] = 1 ; 
			t[tp].v[1] = 0 ; 
		}
		update(t[tp].rs , mid+1, r , ql,qr , k) ; 
	}
}

tree query(int tp, int l ,int r, int ql , int qr){
	if(ql <= l && r<=qr) return t[tp] ; 
	int mid = l + r >>1;
	tree ans; 
	pushup(ans, max(l,ql),min(r,qr),t[tp].f) ; 
	if(ql <= mid) ans =(ans + query(t[tp].ls , l ,mid ,ql ,qr))%mod;
	if(qr > mid) ans =(ans + query(t[tp].rs , mid+1,r,ql,qr)) ;
	return ans ; 
}

int LCA(int x , int y){
	while(top[x] != top[y]){
		if(d[top[x]] < d[top[y]]) swap(x,y);
		x = f[top[x]] ; 
	}
	return x ; 
}

void upd(int x, int y , int k){
	while(top[x] !=top[y]){
		if(d[top[x]] < d[top[y]]) swap(x,y) ; 
		update(rt[root] , 1, n , dfn[top[x]] , dfn[x] , k) ;
		x = f[top[x]] ; 
	}
	if(dfn[x] > dfn[y]) swap(x,y) ; 
	update(rt[root] , 1, n ,dfn[x] , dfn[y] , k) ;
}


int cal(int c, int op , int l , int r){
	tree g = query(rt[root] , 1, n , l ,r) ; 
	return (g.ddk +(op*g.dk*(c<<1|1))% mod + g.k*((c*c)%mod+c)+mod)%mod;
}

int sum(int x , int y){
	int lca =  LCA(x, y) ; 
	int m1 = (d[y] - d[lca] * 2 + mod) % mod ;
	int m2 = d[y] ;
	int ans = 0  ; 
	while(top[x] != top[y]){
		if(d[top[x]] <= d[top[y]]){
			ans = (ans + cal(m2 , -1 , dfn[top[y]] , dfn[y]))%mod; 
			y = f[top[y]] ; 
		}
		else{
			ans = (ans + cal(m1, dfn[top[x]] , dfn[x]))% mod ; 
			x = f[top[x]] ;
		}
		
	}
	if(d[x] >= d[y]) ans = (ans + cal(m1,1,dfn[y] , dfn[x]))%mod; 
	else ans  = (ans + cal(m2 , 2, dfn[x] , dfn[y]))%mod; 
	return ans *((mod+1)/2)%mod; 
}

signed main(){
	ios :: sync_with_stdio(false) ; 
	cin.tie(0) ; 
	cin >> n >> m ; 
	for(int i = 1 ; i < n ; i++){
		int u , v ; cin >> u >> v ; 
		add(u, v) ; 
		add(v, u) ; 
	}
	for(int i = 1 ; i <= n ;i++){
		cin >> a[i] ;
	}
	d[1] = 1 ; 
	dfs1(1) ; 
	dfs2(1,1) ; 
	for(int i = 1 ; i <= n ;i++){
		e[i] = (e[i-1] + d[rk[i]]) %mod; 
		ee[i] = (ee[i-1] + d[rk[i]] * d[rk[i]]) % mod; 
	}
	build(rt[0] , 1, n) ; 
	int resu = 0 ; 
	int ddd = 0 ; 
	while(m--){
		int op , x , y ; 
		cin >> op ; 
		if(op == 1){
			cin >> x >> y >> k ; 
			x ^= resu  , y ^= resu ; 
		//	cin >> k ; 
			rt[++ddd] = ++cnt ; 
			t[rt[ddd]] = t[rt[root]] ; 
			root =ddd; 
			t[rt[root]].v[0] = t[rt[root]].v1 = 1 ; 
			upd(x, y , k) ; 
		}
		else if(op == 2){
			cin >> x >> y ;
			x^=resu , y^=resu ; 
			last = sum(x, y) ; 
			cout << last << endl ;
		}
		else{
			cin >>x ;
			x^=last ; 
			root = x ;
		}
	}
}























