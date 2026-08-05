#include<bits/stdc++.h>

using namespace std ; 


const int N = 10005 ; 
const int inf  = 0x3f3f ; 

struct Seg{
	int l = 0 , r = 0 ; 
	int w = 0 ; 
	pair<int, int>st ,ed ; 
	int type = 0 ; 
	bool ok = 0 ; 
	Seg(int L , int R){
		l = L , r = R , w = 0 , st = ed = {L , R} ,type = 0 ;
		ok = 1 ;
	}
};

int upd(int x , int l ,int r) return min(max(x, l) , r) ; 

Seg merge(Seg &s , Seg &b){
	if(!a.ok) return b ; 
	if(!b.ok) return a ; 
	Seg c ; 
	c.ok = 1 ; 
	c.st = a.st , c.ed = b.ed ; 
	if(a.type == 0 && b.type ==0 ){
		int nl = max(a.l , b.l) ;
		int nr = min(a.r , b.r) ; 
		if(nl <= nr){
			c.type = 0 ; 
			c.l = nl , c.r = nr ;
			c.w = 0 ; 
		}
		else{
			c.type = 1 ; 
			if(a.r < b.l){
				c.l = a.r ; 
				c.r = b.l ;
				c.w = b.l - a.r ;
			}
			else{
				c.l = a.l ;
				c.r - b.r ;
				c.w = a.l - b.r ;
			}
		}
	}
	else if(a.type == 1 && b.type ==1){
		c.type = 1 ; 
		c.l = a.l ; 
		c.r = b.r ; 
		c.w = a.w + b.w + abs(a.r - b.l) ; 
	}
	else if(a.type == 1){
		int val = upd(a.r , b.l .b.r) ; 
		c.type = 1 ;
		c.l = a.l ; 
		c.r = q ; 
		c.w = a.w + b.w + abs(a.r -q) ; 
	}
	else{
		int val = upd(b.l , a.l ,a.r) ; 
		c.type = 1 ; 
		c.l = p ; 
		c.r = b.r ;
		c.w = a.w + b.w + abs(p - b.l) ; 
	}
	return c ; 
}

struct Tree{
	int n ; 
	vector<Seg>tree; 
	Tree(vector<int>&L , vector<int>&R){
		n = (int)L.size()-1; 
		tree.resize(n <<2 +2) ; 
		build(1,1,n, L ,R) ; 
	}
	
	void build(int id, int l , int r, vector<int>&L, vector<int>&R){
		if(l == r){
			tree[id] = Seg(L[l] , R[l]) ; 
			return ;
		}
		int mid = l + r >> 1 ; 
		build(id <<1 ,l , mid,  L , R) ;
		build(id<<1|1 , mid + 1, r, L , R) ; 
		tree[id] = merge(tree[id<<1 ] , tree[id<<1|1]) ; 
	}
	Seg query(int id, int l ,int r, int ql , int qr){
		if(qr < l || r < ql) return Seg() ; 
		if(ql <= l && r <= qr) return tree[id] ; 
		int mid = l + r >> 1; 
		Seg left = query(id<<1 ,l , mid, ql , qr) ;
		Seg right = query(id <<1|1 , mid+1, r, ql, qr);
		return merge(left . right) ; 
	}
	Seg query(int l, int r) return query(1,1,n,l,r) ; 
};
int L[N] , R[N] ; 

signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ; 
	int n ; cin >> n ; 
	for(int i =1 ; i<=n ;i++) cin >> L[i] >> R[i] ; 
	Tree seg(L, R) ; 
	int q ; cin >> q; 
	while(q--){
		int sx, tx , sy ,ty ; 
		cin >>sx >> sy >> tx >> ty ; 
		if(sx > tx) swap(sx,tx), swap(sy, ty) ; 
		Seg res = seg.query(sx, tx) ; 
		int ans = tx - sx ; 
		if(res.type ==1){
			ans += res.w ; 
			ans += abs(sy - res.l) ; 
			ans += abs(ty - res.r) ; 
		}
		else{
			if(sy < res.l & ty < res.l) ans += (res.l -sy) + res.l -ty ;
			else if(sy > res.r && ty > res.r) ans += (sy - res.r) + (ty - res.r );
			else ans += abs(sy -ty) ;
		}
		cout << ans << endl ; 
	}
}
