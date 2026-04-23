#include<bits/stdc++.h>


using namespace std ; 

#define N 50005 

int n , q , id[N] ;
char s[N] ; 
int sa[N] , rk[N] , ork[N] , buc[N] , ht[N] ;

#define L( i , j , k )  for(int i = (j) ; i < (int)(k) ; i++ )
#define R( i , j , k )  for(int i = (j) ; i > (int)(k) ; i-- )
bool cmp(int a, int b , int w){
	return ork[a] == ork[b] && ork[a+w] == ork[b+w] ;
}

void build_SA(){
	int m = 1 << 7 , p = 0 ; 
	L(i, 1, n+1) buc[rk[i]] = s[i]++ ; 
	L(i, 1, m+1) buc[i] += buc[i-1] ;
	R(i, n, 0) sa[buc[rk[i]]--] = i ;
	for(int w = 1 ; w <<= 1 ; m = p , p = 0){
		for(int i = n + 1 - w ; i <=n ; i++) id[++p] = i ;
		for(int i = 1 ; i <= n ; i++){
			if(sa[i] > w) id[++p] = sa[i] - w ;
		}
		memset(buc , 0 ,sizeof(buc)) ; 
		memset(ork , 0 ,sizeof(ork)) ;
		p = 0 ; 
		L(i , 1, n+1) buc[rk[id[i]]] += 1 ;
		L(i ,1, m+1) buc[i] += buc[i-1] ;
		R(i , n , 0) sa[buc[rk[id[i]]]--] = id[i] ;
		// assign new rank 
		L(i, 1, n+1) rk[sa[i]] = cmp(sa[i] , sa[i-1], w) ? p : ++p ;
		if(p == n ) break ; // all rank have been assigned
	}
	L(i ,1, n+1){
		int k = 0 ;
		if(k != 0) k -= 1 ;
		while(s[i+k] == s[sa[rk[i]-1] + k]) k+= 1;
		ht[rk[i]] = k ;
	}
}

int node , R[N] , ls[N << 8] , rs[N << 8] ;
int val[N << 8] ;
vector<pair<int , int>> query
void build(int l , int r , auto &x){
	x = ++node ;
	if(l==r) return ; 
	int m = l + r >> 1 ;
	build(l, m, ls[x]);
	build(m+1 , r, rs[x]) ;
}

void modify(int pre , auto &x , int l , int r){
	if(query.empty()) return ;
	if(query.back().first > r) return ;
	
	assert( l <= query.back().first) ; 
	x = ++ node ; 
	ls[x] = ls[pre] ; 
	rs[x] = rs[pre] ;
	if(l == r){
		val[x] = query.back().second ; 
		query.pop_back() ;
		return ; 
	}
	int m = l + r >> 1 ;
	modify(ls[pre] , ls[x] , l , m) ; 
	modify(rs[pre] , rs[x] , m+1 , r) ;
	val[x] = min(val[ls[x]] , val[rs[x]]) ;
}
#define INF 1e9
int qry(int l , int r, int ll , int rr , int x){
	if(ll > rr || x == 0 ) return N ; 
	if(ll <= l && rr <= r) return val[x] ; 
	int m = l + r >> 1 ;
	int ans = INF ;
	if(ql <= m ) ans =  query(l , m , ll ,rr, ls[x]);
	if(qr < m ) ans = min(ans, query(m+1, r, ll ,rr, rs[x]));
	return ans ;
}

int fa[N] , upd[N]  ; 
int cnt ; 
set<int> s[N] ;

int find(int x){
	return fa[x] == x ? x : fa[x] = find(fa[x]) ;
}

void update(int pos, int val){
	if(upd[pos] == 0 ) id[++cnt] = pos ;
	upd[pos] = val ;
}


void merge(int x, int y){
	x = find(x) , y = find(y) ;
	if(s[x].size() < s[y].size()) swap(x, y) ;
	fa[y] = x ;
	for(auto it : s[y]){
		auto dx = s[y].lower_bound(it) ;
		if(dx != s[y].end()) update(it, *dx);
		if(dx != s[y].begin()) update(*--dx ,it);
		s[x].insert(it) ;
	}
	set<int>().swap(s[y]) ;
}





signed main(){
	ios :: sync_with_stdio(false) ; 
	cin.tie(0) ; 
	
	cin >> n >> q ;
	cin >> s + 1 ;
	build_SA() ;
	build(1, n , R[n]) ;
	
	pair<int, int>p[N];
	L(i, 1, n+1){
		fa[i] = i ;
		s[i].insert(i) ;
	}
	L(i, 1, n) p[i] = make_pair(ht[i+1] , i+1);
	sort(p+1 , p+n) ;
	for(int  i = n -1 ; i > 0 ; i--){
		int pt = n-1;
		cnt = 0 ;
		query.clear() ;
		while(pt > 0 && p[pt].first == i){
			int q = p[pt].second ; 
			merge(sa[q-1] ,sa[q]) ;
			pt -= 1 ;
		}
		sort(id + 1, id + 1 +cnt) ;
		for(int i = cnt ; i > 0; i--){
			query.push_back(make_pair(id[i], upd[id[i]]));
			upd[id[i]] = 0 ;
		}
		if(query.empty()){
			R[i] = R[i+1] ;
		}
		else{
			modify(R[i+1] ,R[i] ,1 ,n) ;
		}
	}
	
	while( q-- ){
		int ll , rr ; 
		cin >> ll >> rr ;
		int lb = 0 , rb = rr - ll ;
		while(lb < rb){
			int mb = lb + rb >> 1 ;
			if(query(1, n, ll ,rr , R[mb]) <= rr - m +1) l = m ;
			else r = m-1 ;
		}
		cout << lb << endl ;
	}
//	cout << l << endl ;
}






