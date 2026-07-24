#include<bits/stdc++.h>

using namespace std ; 


#define N 100010
#define M 5005

bool flag = 0 ; 
int n , m ; 
int k[N] , a1[N] , a2[N] ; 

bool vis[N] ; 
int c , cyc[N] , in[N] ;
int val[N] , pre[N] ; 
vector<int>e[N] ;  

struct DSU{
	int fa[N] ; 
	int find(int x){
		return fa[x] == x ? x : fa[x] = find(fa[x]) ; 
	}
	void merge(int a, int b) fa[find(b)] = find(a) ; 
} cy  , ch ;



signed main(){
	ios :: sync_with_stdio(false) ; 
	cin.tie(0) ; 
	cin >> n >> m ; 
	for(int i = 1 ; i <= n; i++) e[i].clear() ; 
	memset(vis, 0 , sizeof(vis)) ; 
	for(int i = 1 ; i < = n ; i++) cin >> k[i] ; 
	for(int i = 1 ; i <= m ; i++){
		int u , v ; cin >> u >> v ; 
		e[v].push_back(u) ; 
	}
	for(int i = 1 ; i <= n ; i++){
		if(vis[i]) continue ; 
		int  p =  i ; 
		while(in[p] == 0){
			cyc[++c] = p ; 
			in[p] = c ; 
			p = k[p] ;
		}
		for(int j = c << 1 ; j ; j--){
			cy.fa[j] = ch.fa[j] = j ; 
			val[j] = pre[j] = 0 ; 
			iny id = cyc[ j > c  ? j -c : j] ;
			for(auto it : e[id]){
				if(in[it] == 0) continue ; 
				it = in[it] ; 
				if(it + c < j) it += c ; 
				else it -= c ; 
				pre[j] = max(pre[j] , it) ; 
				if(it < j) it += j ; 
				if(it <= c << 1){
					val[ch.find(it)] = max(val[ch.find(it)] , cy.find(it));
				}
				
			}
			while(true){
				int cyid = cy.find(j) , chid = ch.find(j) ; 
				if(cyid < val[chid]) cy.merge(cyid, cyid+1) ;
				else break ; 
			}
			int cyid = cy.find(j) , chid = ch.find(j) ; 
			val[chid] = 0 ; 
			if(chid == c<<1 || cyid != chid || pre[chid+1] < j){
				break ; 
			}
			ch.merge(chid+1,chid) ; 
		}
		a1[id] = min(c, ch.find(j) - j +1) ; 
		a2[id] = min(c, cy.find(j) - j +1 );
	}
	for(int i = 1  ; i <= c ; i++) in[cyc[i]] = 0 , vis[cyc[i]] = 1 ; 
	c= 0 ; 
	for(int i = 1 ; i <= n ; i++) cout << a1[i] << " " << a2[i] ; 
	cout << endl ; 
}
