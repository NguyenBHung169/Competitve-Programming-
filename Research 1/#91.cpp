#include<bits/stdc++.h>


using namespace std ; 


int n , m , s ,t , x ,y , k ; 

vector<int>G[N] ; 
int ds[4][N] ; 
int que[N] , lh , rt ; 
#define d ds[t] 
#define eb emplace_back

void bfs(int s, int t){
	lh = rt = 1 ; 
	que[lh] = s ; 
	d[s] = 0 ; 
	while(lh <= rt){
		int u = que[lh++] ; 
		for(auto v : G[u]){
			if(d[v] < 0 ){
				d[v] = d[u] + 1 ; 
				que[++rt] = v ;
			}
		}
	}
}


signed main(){
	ios :: sync_with_stdio(false) ; 
	cin.tie(0) ;
	cin >> n >> m >> s >> t >> x >> y ; 
	for(int i = 1 ; i <= m ; i++){
		int u , v ; 
		cin >> u >> v ; 
		G[u].eb(v) ; 
		G[v].eb(u) ;
	}
	memset(ds , -1 , sizeof(ds)) ; 
	bfs(s, 0) , bfs(t, 1) ; 
	int d1 = ds[1][x] - ds[0][x] ; 
	int d2 = ds[1][y] - ds[0][y] ;
	if(min(d1 , d2) < 0){
		cout << -1  ;
		exit(0) ; 
	}
	else if(d1 != d2){
		cout << n << " " << k ; 
	}
	else{
		int p = 0 , q = 0 ; 
		bfs(x, 2) , bfs(y, 3) ; 
		for(int i = 1 ; i <= n ; i++){
			if(ds[2][i ] == ds[3][i]){
				if(ds[0][i] + ds[2][i] == ds[0][x]) p = max(p , ds[0][i]); 
				if(ds[1][i] + ds[3][i] == ds[1][y]) q = max(q, ds[1][i]);
			}
			
		}
		cout << n << " " << k - (p + k < q) ; 
	}
}










