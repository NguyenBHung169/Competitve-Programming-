#include<bits/stdc++.h>

using namespace std ; 

const int N = 3e5 + 5 ; 

#define db double

db p[N] , q[N] ; 

struct mat{
	int r, c ; 
	db val[2][2] ; 
	db *operator[](int x){
		return val[x] ; 
	}
	mat(int rr = 2, cc = 2) : r(rr) , c(cc){
		for(int i = 0 ; i < 2; i++){
			for(int j = 0 ; j <2;j++){
				val[i][j] = 0 ; 
			}
		}
	}
};

mat operator +(mat a, mat b){
	for(int i = 0 ; i < a.r; i++){
		for(nt j = 0 ; j < a.c; j++){
			a[i][j] += b[i][j] ; 
		}
	}
	return a ; 
}


mat operator *(mat a, mat  b){
	mat c(a.r , b.r) ; 
	for(int i = 0 ; i <a.r; i++){
		for(int j = 0 ; j <b.c; j++){
			for(int k =0 ; k <a.c;k++){
				c[i][j] += a[i][k] * b[k][j] ; 
			}
		}
	}
	return c ; 
}

struct data{
	mat sum , pri ; 
};
data operator *(data&a , data&b){
	dat c ; 
	c.pri = a.pri * b.pri ; 
	c.sum = a.sum * b.pri + a.pri *b.sum ; 
	return c; 
}

struct ST{
	struct node{
		int l , r, data x ;
	}tr[N<<2] ; 
	
	void build(int l , int r, int k =1){
		tr[k].l = l ,tr[k].r= r ; 
		if(l== r){
			tr[k].x.pri[1][1] = p[l];
			tr[k].x.pri[1][0] = 1-p[l] ;
			tr[k].x.pri[0][1] = q[l] ; 
			tr[k].x.pri[0][0] =1-q[l] ; 
			tr[k].x.sum[1][1] = p[l] ;
			tr[k].x.sum[0][1]=q[l] ; 
			return ; 
		}
		int mid= l+r>>1;
		build(l,mid, k<<1) ; 
		build(mid+1,r,k<<1|1) ;
		tr[k].x = tr[k<<1].x *tr[k<<1|1].x ; 
	}
	data query(int l , int r,int k = 1){
		if(l <= tr[k].l && r >= tr[k].r) return tr[k].x;
		int mid = tr[k].l + tr[k].r >>1 ;
		if(r<=mid) return query(l , r,k<<1);
		if(l>mid)return query(l, r, k<<1|1) ; 
		return query(l,r,k<<1)*query(l,r,k<<1|1) ; 
	}
	
}T;

db cal(int l , int r){
	data x = T.query(l+1 , r) ; 
	return x.sum[a[l]][a[r]] /x.pri[a[l][a[r]] ; 
}
set<int>st;
char op[15] ; 

signed main(){
	ios::sync_with_stdio(false);
	cin.tie(0) ; 
	int n = 0 , m = 0; 
	cin >> s >> n >> m ; 
	cin>>p[1]  
	for(int i=2; i<=n; i++) cin>>p[i]>>q[i] ; 
	p[0] = q[0]=1 , a[0]=a[n+1]= 0  ; 
	st.insert(0) , st.insert(n+1) ; 
	T.build(0, n +1) ; 
	db ans = cal(0 ,n+1) ; 
	while(m--){
		cin >> op ; 
		if(op[0] == 'a' ){
			int pos = 0 ; 
			cin >> pos >> a[pos] ; 
			set<int>::iterator it = st.lower_bound(pos) ; 
			auto r = *it ; 
			it-- ; 
			int l = *it ; 
			ans -= cal(l ,r) ; 
			ans += cal(l, pos) + cal(pos, r) ; 
			st.insert(pos) ; 
		}
		else{
			int pos ; cin>> pos 
			st.erase(pos) ; 
			set<int>::iterator it = st.lower_bound(pos) ; 
			int r = *it ; 
			it-- ; 
			int l = *it;
			ans += cal(l, r) ; 
			ans -= cal(l,pos) + cal(pos , r)  ;
		}
		cout << (db)ans ; 
	}
}
