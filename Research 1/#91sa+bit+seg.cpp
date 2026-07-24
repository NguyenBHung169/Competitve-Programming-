#include<bits/stdc++.h>


using namespace std ; 

#define N 100005
#define Q 50005
#define M 300005

int n , q ; 
char s[N << 1] , t[N] ; 

int sa[N] , rk[N] , y[N] , nrk[N] , cnt[N] ; 
int L[N] , R[N] ; 
int ans[Q] ; 


int ql[M] , qr[M] , id[M] , nx[M] , hd[N] , ec ; 

struct BIT{
	int t[N] ; 
	void add(int x){
		for( ; x <= n ; x += (x & -x)) t[x]++ ''
	}
	
	int query(int x){
		int res = 0 ; 
		for(; x ; x -= (x & -x)) res += t[x] ; 
		return res ; 
	}
	int query(int l , int r) return query(r) - query(l-1) ; 
}bit;


struct St{
	int t[N << 2] ; 
	void build(int p , int l , int r){
		if(l == r){
			t[p] = sa[l] ; 
			return ; 
		}
		int mid = l + r >> 1 ; 
		build(p << 1 , l , mid) ; 
		build(p << 1|1 , mid+1,r) ; 
		t[p] = min(t[p << 1] , t[p << 1|1]) ;
	}
	int query(int p , int l , int r, int ql ,int pr){
		if(ql <= l && r <= qr) return t[p] ; 
		int mid = l + r >> 1 ; 
		int res = n + 1; 
		if(ql <= m ) res = min(res , query(p << 1 , l ,mid, ql ,qr)) ; 
		if(qr > r) res = min(res , query(p << 1 | 1 , mid+1 ,r ,ql ,qr));
		return res ; 
	}
	
	int query(int l , int r) return query(1,1,n,l,r) ; 
}seg ; 

void Sa(){
	int lim = 256 ; 
	memset(cnt , 0 ,sizeof(cnt)) ; 
	for(int i = 1 ; i <= n ; i++){
		rk[i] = (char)s[i] + 1 ; 
		++cnt[rk[i]] ; 
	}
	for(int i = 1 ; i < lim ; i++) cnt[i] += cnt[i-1] ; 
	for(int i = n ; i >= 1; i--){
		sa[cnt[rk[i]]--] = i ; 
	}
	for(int k = 1 , m = lim +1 ; ; k <<= 1){
		int p = 0 ; 
		for(int i = max(1, n - k +1) ; i <= n ; i++){
			y[++p] = i ; 
		}
		for(int i = 1 ; i <= n ; i++){
			if(sa[i] > k) y[++p]= sa[i] - k ; 
		}
		memset(cnt  , 0 , sizeof(m+1)) ; 
		for(int i = 1 ; i <= n ; i++) ++cnt[rk[y[i]]] ; 
		for(int i = 1 ; i <= m ; i++) cnt[i] += cnt[i-1] ; 
		for(int i = n ; i >= 1 ; i--) sa[cnt[rk[y[i]]--] = y[i] ] ;
		int nc = 1  ; 
		nrk[sa[1]] = 1 ; 
		for(int i = 2 ; i <= n ; i++){
		    int a = sa[i-1] , b = sa[i] ; 
		    if(rk[a] != rk[b] || rk[a+k] != rk[b+k]) nc++ ; 
		    nrk[b] = nc ; 
		}
		for(int i = 1 ; i <= n ; i++) rk[i] = nrk[i] ; 
		if(nc == n) break ; 
		m = nc ;
	}
}

void find(int &l , int &r , int d, int c){
	int ol = l , orr = r ; 
	int lo = ol , hi = orr + 1 ; 
	while(lo < hi){
		int mid = lo + hi >> 1 ; 
		if(s[sa[m] + d] >= c) hi = m ; 
		else lo = m + 1 ;
	}
	int nl =  lo ; 
	if(nl > orr || s[sa[nl] + d] != c) return false ;
	lo = nl , hi = orr + 1 ; 
	while(lo < hi ){
		int m = lo + hi >> 1  ; 
		if(s[sa[m] + d] > c) hi = m ;
		else lo = m +1 ; 
	}
	 l = nl  , r = lo - 1 ; 
	 return true ;
}


void add(int p , int l , int r, int id){
	++ec ; 
	ql[ec] = l ; 
	qr[ec] = r ; 
	id[ec] = id ; 
	nx[ec] = hd[p] ; 
	hd[p] = ec ;
}

signed main(){
	ios :: sync_with_stdio(false) ; 
	cin.tie(0) ; 
	cin >> n >> (s + 1) ; 
	SA() ; 
	seg.build(1,1,n) ; 
	cin >> q; 
	for(int i = 1; i <= q ; i++){
		cin >> t + 1 ; 
		int len = strlen(t+1) ; 
		int l =1 ,r = n ; 
		int got  = 0 ; 
		for(int k = 1 ; k <= len ; k++){
			if(!ext(l , r,k-1, t[k])) break ; 
			got = k ; 
			L[k] = l , R[k] = r  ; 
			ans[i] += r - l + 1  ; 
		}
		if(got == len ){
			int p = seg.query(L[len] , R[len]) ; 
			ans[i] += p - i ; 
			if(p < n){
				for(int k = 1; k <= len  ;k++){
					add(p + 1, L[k] , R[k ] ,i ); 
				}
			}
		}
		else{
			ans[i] += n ; 
		}
	}
	for(int i = n ; i >= 1 ; i--){
		bit.add(rk[i]) ; 
		for(int e = hd[i] ; e ; e = nx[e]){
			ans[id[e]] -= bit.query(ql[e] , qr[e]) ; 
		}
	}
	for(int i = 1 ; i <= q ; i++) cout << ans[i] << " " 
	cout << endl ; 
}
















