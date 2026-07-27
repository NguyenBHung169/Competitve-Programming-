#include<bits/stdc++.h>

using namespace std ; 


const int N  = 20005 ; 
const int inf  = 1e9  ;

struct Node{
	int w , mn ,ls, rs ; 
}tr[N << 2] ; 

void update(int &p , int l , int r, int delta){
	tr[++tot] = tr[p] ; 
	p = tot ; 
	tr[p].w += delta ; 
	if(l == r){
		if(delta == 1){
			pos[l] = p ; 
		}
		return ; 
	}
	int mid = l + r >> 1 ; 
	if(ql <= mid) update(tr[p].ls ,l , mid, delta) ; 
	else update(tr[p].rs , mid+1, r, delta) ; 
}

void pull(int p){
	tr[p].w = tr[tr[p].ls ].w + tr[tr[p].rs].w ;
	tr[p].mn = min(tr[tr[p].ls ].mn , tr[tr[p].rs ].mn ) ; 
}

int get(int p , int l , int r){
	if(!tr[p].w) return 0 ; 
	if(l == r){
		tr[p].w = 0 ; 
		tr[p].mn = ++tim ; 
		return l ; 
	}
	int mid = l + r >> 1 ; 
	int x = 0 ; 
	if(ql <= mid){
		x = get(tr[p].ls , l, mid) ;
		if(x){
			pull(p) ; 
			return x ;
		}
	}
	if(mid < qr){
		x = get(tr[p].rs , mid+1, r) ; 
		if(x){
			pull(p) ; 
			return x ; 
		}
	}
	pull(p) ; 
	return  0 ; 
}

int query(int p , int l , int r){
	if(p == 0 || (ql <= l && qr >= r)){
		return tr[p].mn ; 
	}
	int mid = l + r>>1; 
	if(qr <= mid) return query(tr[p].ls, l, mid) ; 
	if(mid < ql) return query(tr[p].rs  , mid+1,r) ;
	return min(query(tr[p].ls,l,mid) , query(tr[p].rs, mid+1,r)) ; 
}

void tar(int x , bool irt , int cnt){
	int child = 0 ; 
	dfn[x] = low[x] = tim ; 
	ql = al[x] , qr = ar[x] ; 
	while(int y = get(rt[x] , 1, cnt)){
		tar(y, 0 , cnt);
		child++; 
		low[x] = min(low[x] , low[y]) ; 
		if(low[x] == dfn[x]) cut[x]=1; 
		ql = al[x] , qr = ar[x] ; 
	}
	ql = a[x] , qr = ar[x] ; 
	low[x] = min(low[x] , query(rt[x],1,cnt)) ; 
	if(irt) cut[x] = (child>=2) ; 
}


signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ;
	int n ; cin >> n ; 
	tr[0].mn = inf ; 
	for(int i =1 ; i <= n  ; i++){
		cin >> al[i] >> ar[i] ; 
		al[i] += n , ar[i] += n ; 
		add[al[i]] += i ;
		del[ar[i]+1] += i ; 
	}
	for(int i = n +1;  i <= 2*n ; i++){
		cin >> al[i] >>ar[i] ; 
		add[al[i] ]+= i ; 
		add[ar[i] +1] += i ; 
	}
	for(int i =1 ; i <= m; i++){
		rt[i] =rt[i-1] ; 
		for(int x :add[i]){
			ql = x ; 
			update(rt[i] ,1,m,1) ; 
		}
		for(int x :del[i]){
			ql =x ; 
			update(rt[i],1,m,-1) ; 
		}
	}
	for(int i =1; i <= m ; i++){
		if(dfn[i]) continue ; 
		tr[pos[i]] = {0 , ++tim , 0 , 0} ; 
		tar(i,1,m) ; 
	}
	for(int i =1; i <= n ; i++) cout << cut[i] ; 
	cout <<endl ; 
	for(int i =n+1; i <= 2*n; i++) cout<<cut[i] ; 
	cout <<endl ; 
	
}
