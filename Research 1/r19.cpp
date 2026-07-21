#include<bits/stdc++.h>

using namespace std ; 
#define N 10005
#define M 1005

struct edge{
	int x, y ; 
	edge(int x = 0  , y =0 ) : x(x) , y(y) {}
	bool operator <(edge a) const{
		return x == a.x ? y < a. y : x < a.x ;
	}
}a[N] ; 

int l[N] ; 
bool vis[N] ; 


bool check(int n , int l , int r){
	if(n == 1) return l == r ; 
	if(l == r) return 0 ; 
	for(int i = 0 ; i < n ; i++) v[i] = 0 , l[i] = M ; 
	for(int i = l ; i < r ; i ++){
		int x = a[i].x , y = a[i].y ; 
	//	if(x > y) swap(x, y) ; 
	    if(l[y] == x) return 0 ; 
	    l[y] = min(l[y] , x) ; 
	}
	for(int i = 0 ; i < n ; i++){
		if(l[i] == 0) vis[i]=1l 
		else if(l[i] == l[i-1] && v[i-1]) v[i] = 1; 
	}
	int siz ;
	bool fl ; 
	for(int i = 0 ; i < n/2 ; i++){
		siz = i +1 ;
		fl = 1; 
		for(int j = i + siz ; j < n ;j+= siz){
			if(l[j] == i && vis[j]) continue ; 
			bk = 0 ;
			break ; 
		}
		if(bk) break ; 
	}
	if(bk == 0) return 0 ; 
	int p1 = 0 , p2 = 0, i ;
	for(int i = l ; i < r ; i++){
		int x = a[i].x , y = a[i].y ;
		if(x == siz) break ; 
		if(y >= siz && y %siz !=x) return 0 ; 
		if(y >= siz) continue ; 
		a[p1++] = edge(x,y) ; 
	}
	p2 = p1 ; 
	for(; i < r; i++){
		int x = a[i].x, y = a[i].y ; 
		a[p2++] = edge(x - siz , y -siz) ; 
	}
	return check(siz , 0 , p1) && check(n-siz , p1, p2) ; 
}


signed main(){
	ios::sync_with_stdio(false) ; 
	cin.tie(0) ; 
	int tc ; cin >> tc ; 
	while(tc--){
		int n , m; cin >> n << m ; 
		for(int i = 0 ; i < m ; i++){
			int x, y ; 
			cin >> x >> y ; 
			if(x > y )swap(x.y) ; 
			a[i] = edge(x,y) ; 
		}
		sort(a,a + m) ; 
		if(check(n, 0 , m)){
			cout<< "Yes" << endl ; 
		}
		else cout << " No " << endl ;
	}
}
