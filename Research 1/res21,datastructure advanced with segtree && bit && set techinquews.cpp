#include<bits/stdc++.h>

using namespace std ; 
#define f first 
#define s second
#define ll long long
const int N = 1e5 + 5 ;
const int inf = 1e9 + 7 ; 
ll val[N << 2] ; 

void build(int l, int r, int x){
	if(l == r){
		val[x] = d[l] ; 
		return ; 
	}
	int m = l + r >> 1 ; 
	build(l , m , x << 1) ; 
	build(m + 1 , r , x<<1|1) ; 
	val[x] = max(val[x << 1] , val[x << 1|1]) ; 
}

void modify(int l , int r , int p , int x , ll v){
	if(l == r){
		val[x] = v ; 
		return ; 
	}
	int m = l + r >> 1 ; 
	if(p <= m ) modify(l , m ,p , x<<1 , v) ; 
	else modify(m + 1 , r, p , x<<1|1 , v) ; 
	val[x] = max(val[x<<1] , val[x <<1|1]) ; 
}

ll query(int  l ,int r, int ql , int qr , int x){
	if(ql <= l && r <= qr) return val[x] ; 
	int m = l + r >> 1 ; 
	ll res = -inf ; 
	if(ql <= m) res = max(res, query(l, m , ql ,qr , x<<1)) ; 
	if(qr > m) res = max(res, query(m+1,r,ql ,qr,x<<1|1)) ; 
	return res ; 
}

ll c[N] ; 

void add(int x, ll v){
	while(x <= n) c[x] += v ,  x += x & -x ;
}

ll query(int x){
	ll res = 0 ; 
	while(x) x += c[x] ,  x -= x & -x ;
	return res ; 
}
 
ll query(int l , int r) return query(r) - query(l-1) ; 

set<pair<int, int>> s; 
ll contri[N] ; 

ll R, node ; 
ll ls[N << 4] , rs[N << 4] ; 
ll mval[N << 4] , sum[N << 4] ; 

void ins(ll l , ll r ,ll p ,ll &x ,bool tp){
	if(!x) x = ++ node ; 
	if(tp) sum[x]++  , val[x] += p - inf ; 
	else sum[x]-- , val[x] -= p - inf ; 
	if(l == r) return  ; 
	ll m = l + r >> 1 ; 
	if(p <= m ) ins(l , m , p , ls[x] , tp) ; 
	else ins(m+1 , r, p ,rs[x] , tp) ; 
}

ll query(ll l , ll r , ll k , ll x){
	if(!x) return -inf ; 
	if(l == r){
		if(k + val[x] <= 0) return (k-1)/(W-l)+1;
		return -inf ;
		
	}
	ll m = l + r >> 1 ; 
	if(k + val[ls[x]] <= 0) return query(l , m ,k ,ls[x]) ; 
	return sum[ls[x]] + query(m + 1 , r , k +val[ls[x]] , rs[x]) ; 
 }

void clear(pair<int, int>x){
	if(x.f == 1 || x.s == n-1) return ; 
	ins(1,inf,contri[x.f] +inf , R , 0) ;
}

void upd(int l , int r){
	if(l == 1 || r == n-1) return ; 
	contri[l] = query(l ,r) - query(1,n,l ,r,1) ; 
	ins(1,inf, contri[l] + inf , R , 1) ; 
}

void change(int pos , ll &val , ll add){
	auto it = s.upper_bound(make_pair(pos, N)) ; 
	if(val <= 0 && val+add >0){
		ans += val + add ; 
		pair<int,int> x = *--it ; 
		clear(x) ;
		s.erase(x) ; 
		if(x.f == x.se){
			val += add ; 
			return ; 
		}
		if(x.f == pos){
			x.f++ ;
			s.insert(x) ; 
			upd(x.f , x.s) ; 
		}
		else if(x.s == pos){
			--x.s ; 
			s.insert(x) ; 
			upd(x.f , x.s) ; 
		}
		else{
			s.insert({x.f , pos-1}) ;
			upd(x.f , pos-1) ; 
			s.insert({pos+1 , x.s});
			upd(pos+1,x.s) ; 
		}
	}
	else if(val > 0 && val + add <= 0){
		ans -= val; 
		pair<int,int>x = {pos , pos} ;
		if(it!=s.begin()){
			pair<int,int>tmp = *--it ; ++it; 
			if(tmp.s == pos - 1){
				clear(tmp) ; 
				s.erase(tmp) ; 
				x.f = tmp.f ; 
			}
		}
		if(it != s.end()){
			pair<int,int>tmp = *it ; 
			if(tmp.fi == pos+1){
				clear(tmp) ; 
				s.erase(tmp) ;
				x.s = tmp.s ; 
			}
			s.insert(x) ; 
			upd(x.f , x.s ) ; 
		}
	}
	else if(val <= 0 && val + add <= 0){
		--it ; 
		pair<int,int>x = *it ; 
		clear(x) ; 
		update(x.f ,x.s) ; 
	}
	else ans += add;
	val += add ; 
}

void solve(){
	cin >> n >> q >> u[0] ; 
	for(int i = 1 ; i < n ; i++){
		cin >> u[i] ; 
		d[i] = u[i] - u[i-1] ; 
		if(d[i] > 0) ans += d[i] ; 
		add(i , d[i]) ; 
	}
	build(1,n,1) ; 
	for(int i = 1 , pre = 1 ; i < n ; i++){
		if(d[i] <= 0 && d[i-1] > 0) pre = i ; 
		if(d[i] > 0 &&d[i-1] <= 0 ){
			s.insert({pre , i-1}) ; 
			if(pre!=-1) upd(pre, i-1) ; 
		}
		if(i==n-1 && d[i]<=0) s.insert({pre,  n-1}) ; 
	}
	for(int i =1  ; i < q+1; i++){
		int op ; cin >> op ; 
		if(op == 0){
			 ll l , r, c; cin >> l >> r >> c ; 
			 l-- , r-- ; 
			 if(l){
			 	add(l, c) ;
			 	modify(1,n,l,1,d[l]) ; 
			 	change(l , d[l] ,c) ; 
			 }
			 if(r < n-1){
			 	add(r+1 , -c) ; 
			 	modify(1,n,r+1,1,d[r+1]-c) ; 
			 	change(r+1 , d[r+1] , -c) ; 
			 }
		}
		else{
			cout << ans << " " << (ans ? max(-1ll , query(1, inf , ans, R) : 0ll)) << endl; 
		}
	}
}



 

