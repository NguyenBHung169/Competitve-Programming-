#include<bits/stdc++.h>

using namespace std ;

#define N  50005

inr n , q ; 

int a[N] , h[N] , l[N] , r[N] ; 
vector<int> o[N] ; 

vector<array<int,2>>ql[N] , qr[N] ; 

struct fw{
	int tr[N] , s ; 
	void init(){
		memset(tr, 0 ,sizeof(tr)) ; 
		
	}
	void add(int x){
		for(; x ; x&== x -1){
			++tr[x] ; 
		}
	}
	
	int query(int x){
		for(s= 0  ; x <= n ; x += x&-x ) s+= tr[x] ;
		return s ; 
	}
}T; 

int f[N] , stk[N] , tp; 


void solve(int l , int r, vector<int>&id){
	if(l == r){
		for(int x : id) f[x] = l ; 
		return ; 
	}
	int mid = l + r>>1; 
	stk[tp=0] = mid-1 ; 
	vector<int>li ,ri ; 
	for(int i : id){
		if(i < mid){
			li.push_back(i) ; 
			continue ;
		}
		while(tp &&a[stk[tp]] + h[stk[tp]] <= a[i]) tp-- ;
		if(a[stk[tp]] +h[i] > a[i]){
			li.push_back(i) ; 
			stk[++tp] = i ; 
		}
		else{
			ri.push_back(i) ; 
			solve(l , mid-1,li) ; 
			solve(mid, r, ri) ; 
		}
	}
}


signed main(){
	ios::sync_with_stdio(false) ; 
	cin.tie(0) ; 
	for(int i =1; i <= n ; i++){
		cin >>a[i] >> h[i] ; 
	}
	vector<int>id(n) ; 
	iota(id.begin(),id.end(),1) ; 
	a[0] = a[1] ; 
	solve(0 , n ,id) ; 
	for(int i = 1; i <= n ; i++) L[i] = f[i] ; 
	reverse(a+1,a+n+1) , reverse(b+1,b+n+1) ; 
	for(int i = n ; i>0;i--) a[i]*=-1 ; 
	a[0] = a[1] ; 
	solve(0 , n ,id) ; 
	for(int i =1; i <= n ;i++) R[n-i+1] = n-f[i]+1 ;
	for(int i =1 ; i<=q;i++){
		int l ,r ; cin >>l  >> r; 
		ql[l].push_back({r,i}) ; 
		qr[r].push_back({l , i}) ; 
	}
	for(int i =1; i<= n ; i++){
		o[R[i]].push_back(L[i]) ; 
	}
	for(int i=1; i<=n; i++){
		T.add(L[i]) ; 
		for(auto z  :qr[i]){
			int l = z[0] ; 
			int idd=z[1] ; 
			ans[id+=T.query(l) ;
		
		}
	}
	T.init();
	for(int i = n; i>0 ; i--){
		T.add(n-R[i]+1);
		for(int z : ql[i]){
			int r = z[0] ;
			 int idd = z[1] ; 
			 ans[idd] += T.query(n-r+1) ; 
		}
	}
	T.init(); 
	for(int i =1 ; i<=n;i++){
		for(int z : o[i]){
			T.add(z) ; 
		}
		for(int z : qr[i]){
			int l = z[0] ; 
			int idd = z[1] ;
			ans[idd] -= T.query(i) ; 
		}
	}
	for(int  i=1; i<=q; i++){
		cout << ans[i] << endl ; 
	}
}

