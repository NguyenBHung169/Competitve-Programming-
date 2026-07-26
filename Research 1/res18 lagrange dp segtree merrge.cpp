#include<bits/stdc++.h>

#define N 1000005
#define M 10005
#define MOD 10000000007
#define L(i , j , k) for(int  i = (j) ; i <=(int)(k) ; i++)
#define R(i , j , k) for(int  i = (j) ; i >=(int)(k) ; i--)
using namespace std ;
using uint = unsigned int;
struct edge{
	int nxt ; 
	int v ; 
}E[M] ; 

int head[N] ; 
int cnte ; 

int rt[N] ; 
int stk[N << 2] ; 
int top ;
int cnt ;


int d[N] ; 
int n, k, w , u ,v ; 
int ans[N] , f[N] , tmp[N] ;  
int inv[N] ; 


void add(int u , int v){
	E[++cnte].v = v ;
	E[cnte].nxt = head[u] ; 
	head[u] =cnte ; 
}

struct data{
	int a, b , c, d ;
	
	void clear(){
		a = 1 ; 
		b = c = d = 0 ; 
	}
	data(int _a = 1 , int _b = 0 , int _c = 0 , int _d = 0) : a(_a) , b(_b) , c(_c) , d(_d) {}
	
	
	data operator * (const data &rhs) const{
		return data(a * rhs.a % MOD , (rhs.b + b * rhs.a % MOD) % MOD , (a * rhs.c %MOD + c)%MOD ,(b *rhs.c%MOD + d+rhs.d)%MOD ); 
	}
	data operator *= (const data &rhs) {
		return (*this) = (*this) * rhs ;
	}
};

struct node{
	int ls , rs ; 
	data val ; 
	node(){
		val.a = 1 ; 
		val.b = val.c = val.d = ls = rs = 0 ; 
	}
	void clear(){
		val.a = 1 ; 
		val.b = val.c = val.d = ls = rs = 0 ; 
	}
}t[N << 4] ; 

int newnode(){
	if(top) return stk[top--] ; 
	return ++cnt ; 
}

void delnode(int &x){
	if(!x) return ; 
	delnode(t[x].ls ) ; 
	delnode(t[x].rs ) ; 
	stk[++top] = x ; 
	t[x].clear() ; 
	x = 0 ; 
}

void pushdown(int &x){
	if(!t[x].ls ) t[x].ls = newnode() ; 
	if(!t[x].rs) t[x].rs = newnode() ;
	
	t[t[x].ls ].val *= t[x].val ; 
	t[t[x].rs ].val *= t[x].val ;
	t[x].val.clear()  ; 
}


void change(int &x , int l , int r, int cl , int cr, data val){
	if(!x) x = newnode() ; 
	if(cl <= l && r <= cr){
		t[x].val *= val ; 
		return ; 
	}
	int mid = l + r >> 1 ; 
	pushdown(x) ; 
	if(cl <= mid){
		change(t[x].ls , l , mid, cl , cr, val) ; 
	}
	if(cr > mid) change(t[x].rs , mid+1 , r, cl ,cr,val );
}

int merge(int &x, int &y){
	if(!x || !y) return x | y ; 
	if(!t[x].ls && !t[x].rs) swap(x, y) ;
	if(!t[y].ls && !t[y].rs){
		t[x].val *= data(t[y].val.b , 0 , 0 , 0) ; 
		t[x].val *= data(1, 0 , 0 , t[y].val.d) ; 
        return x ; 
	}
	pushdown(x) ; 
	pushdown(y) ; 
	t[x].ls = merge(t[x].ls , t[y].ls ) ; 
	t[x].rs = merge(t[x].rs , t[y].rs ) ; 
	return x ; 
	
}

int query(int x, int l , int r){
	if(l == r) return t[x].val.d ; 
	int mid = l + r >> 1 ; 
	int res = 0 ; 
	pushdown(x) ; 
	res = query(t[x].ls, l , mid) ; 
	res = (res + query(t[x].rs, mid + 1 , r)) % MOD ; 
	return res ; 
}


void dfs(int x , int fa ,int k0){
	change(rt[x] , 1, w, 1, w, data(0, 1, 0 , 0)) ; 
	for(auto i = head[x] ; i ; i= E[i].nxt ){
		int v = E[i].v ; 
		if(v == fa) continue ; 
		dfs(v, x, k0) ; 
		merge(rt[x] , rt[v]) ; 
		delnode(rt[v]) ; 
	}
	change(rt[x] , 1, w,1, d[x] , data(k0 , 0 , 0, 0)) ; 
	change(rt[x] , 1, w, 1 ,w, data(1, 0 ,1, 0) ) ; 
	change(rt[x] , 1, w, 1, w, data(1,1,0 ,0)) ; 
	
}

uint Lagrange(){
	uint res = 0 ; 
	memset(f, 0 ,sizeof(f)) ; 
	f[0] = 1 ; 
	for(int i = 1 ; i <= n + 1 ; i++){
		for(int j = n + 1 ; j >= 1 ; j--){
			f[j] = f[j] * (MOD - i) % MOD ; 
			f[j] = (f[j] + f[j-1] ) % MOD ; 
		}
		f[0] = f[0] * (MOD - i) % MOD  ;
	}
	for(int i = 1; i <= n +1 ; i++){
		memcpy(tmp , f, sizeof(uint)*(n+1)) ; 
		for(int j = 0 ; j <= n ; j++){
			tmp[j] = MOD - tmp[j] * inv[i] % MOD ; 
			tmp[j+1] = (tmp[j+1] - tmp[j] + MOD) % MOD ; 
		}
		uint tans = 0 ; 
		for(int j = k ; j < n+1 ; j++){
			tans = (tans + tmp[j] ) % MOD ; 
		}
		for(int j = 1 ; j <= n + 1 ; j++){
			if(i == j) continue ; 
			if(j < i) tans = tans * inv[i-j] % MOD ;
			else tans = tans *(MOD - inv[j-i]) % MOD  ;
		}
		res = (res + tans * ans[i] % MOD) % MOD ; 
		
	}
	return res  ; 
}

signed main(){
	ios::sync_with_stdio(false) ; 
	cin.tie(0) ;
	cin >> n >> k >> w ; 
	for(int i = 1 ; i <= n ; i++) cin >> d[i] ; 
	for(int i = 1 ; i < n ; i++){
		cin >> u >> v ; 
		add(u, v) , add(v, u) ; 
	}
	for(int i = 1; i <= n+1 ; i++){
		dfs(1, 0 ,i) ; 
		ans[i] = query(rt[1] , 1 , w) ; 
		delnode(rt[1]) ; 
	}
	inv[1] = 1 ; 
	for(int i = 2; i < MOD ; i++){
		inv[i] = (MOD - (MOD / i) * inv[MOD % i] % MOD) % MOD ; 
		assert(inv[i] > 0) ; 
 	}
 	cout << Lagrange() ; 
}
