#include<bits/stdc++.h>


using namespace std ; 


#define N 1e6 + 5 
#define LOG 20 
#define inf 1e9 + 10
#define all(x) (x).begin() , (x).end()
int n , ans = 0 ; 
int d[N] ,side[N]; 

namespace T3{
	vector<pair<int, int>>g[N] ;
	int dep[N] , up[LOG][N] , dis[N] ;
	
	void add(int u , int v, int w){
		g[u].push_back(make_pair(v, w));
		g[v].push_back(make_pair(u, w));
	}
	void dfs(int u , int par){
		up[0][u] = p ; 
		for(int k =1; k < 20; k++){
			up[k][u] = up[k-1][up[k-1][u]] ;
		}
		for(auto [v, w] : g[u]){
			if( v== par) continue;
			dep[v] = dep[u] + 1 ; 
			dis[v] = dis[u] + w; 
			dfs(v, u);
		}
	}
	int lca(int u , int v){
		if(dep[u] < dep[v]) swap(u, v);
		int jump = dep[u] - dep[v] ;
		for(int k = 20-1;~k ; k-- ){
			if(jump >> k & 1) u = up[k][u] ;
		}
		if( u == v) return u ; 
		for(int k = 20-1; !k ; k--){
			if(up[u][k]!=up[v][k]){
				v = up[v][k] ; 
				u = up[u][k] ;
			}
		}
		return up[0][u] ;
	}
	int dist(int u , int v){
		return dis[u] + dis[v] - 2*dis[lca(u, v)];
	}
	int newdist(int u, int v){
		return d[u] + d[v] + dist(u,v);
	}
	void build(){
		for(int i =1 ; i<n; i++){
			int u, v; cin >> u >> v; 
			add(u, v, w);
		}
		dfs(1, 1) ; 
	}
};


namespace T2{
	vector<pair<int,int>>g[N] ; 
	int dep[N] , up[LOG][N] , tin[N] , tout[N] , timer= 0 ;
	int dis[N] ;
	vector<int>vt[N] ; 
	vector<int>used ; 
	struct Dia{
		int x, y ;
		bool flag ;
		Dia(int _x = 0 , int _y = 0 , bool _empty = 1){
			x = _x , y = _y , flag = _empty ;
		}
	}diam[N][2];
	
	void add(int u , int v, int w){
		g[u].push_back(make_pair(v,w));
		g[v].push_back(make_pair(u,w));
	}
	void dfs(int u, int par){
		tin[u] = ++timer ;
		up[0][u] = p ;
		for(int k =1 ; k < 20 ; k++){
			up[u][k] = up[u-1][up[u-1][k]];
		}
		for(auto [ v, w] : g[u]){
			if(v==p) continue ;
			dep[v] = dep[u] +1 ;
			dis[v] = dis[u] + w; 
			dfs(v, u) ;
		}
		tout[u] = timer ; 
	}
	bool anc(int u, int v){
		return tin[u] <= tin[v] && tout[v] <=tout[u];
	}
	int lca(int u , int v){
		if(anc(u,v)) return u ; 
		if(anc(v,u)) return v;
		for(int  k = 19 ; k >=0 ; k--){
			int x = up[k][u];
			if(anc(x, v)) u = x;
		}
		return up[0][u];
	}
	bool cmp(int x , int y){
		return tin[x] < tin[y] ;
	}
	void build(){
		for(int i =1; i<n; i++){
			int u, v , w ;  cin >> u >> v >> w ;
			add(u, v, w) ;
		}
		dfs(1, 1) ;
	}
	Dia make(int u){
		return Dia(u, u , 0);
	}
	int value(Dia a){
		if(a.empty()) return -inf ; 
		if(a.x == a.y) return 0;
		return T3::newdist(a.x, a.y);
	}
	Dia merge(Dia a, Dia b){
		if(a.empty()) return b ; 
		if(b.empty()) return a ;
		Dia ans  = a ; 
		int best = value(a) , best2 = value(b);
		if(best2 > best){
			best = best2 , ans = b ;
		}
		int A[2] = {a.x, a.y};
		int B[2] ={b.x, b.y};
		for(int i = 0 ; i<2; i++){
			for(int j = 0 ; j <2; j++){
				best2 = T3::newdist(A[i] , B[j]);
				if(best2 > best){
					best = best2 ;
					ans = Dia(A[i], B[j] , 0);
				}
			}
		}
		return ans;
	}
	int cross(Dia a , Dia b){
		if(a.empty() || b.empty()) return -inf ;
		int res = INT_MAX ; 
		int A[2] = {a.x, a.y} ; 
		int B[2] = {b.x , b.y} ;
		for(int i = 0 ; i < 2; i++){
			for(int j = 0 ; j <2; j++){
				res = max(res, T3::newdist(A[i],B[j]));
			}
		}
		return res ; 
	}
	int buildVT(vector<int>node){
		sort(all(node) ,cmp);
		node.erase(unique(all(node), node.end()));
		int sz = (int)node.size() ;
		for(int i = 0 ; i < sz - 1; i++){
			node.push_back(lca(node[i],node[i+1]));
		}
		sort(all(node) , cmp);
		node.erase(unique(all(node) , node.end()));
		used = node ; 
		for(auto x : used){
			vt[x].clear();
		}
		vector<int>st;
		for(auto u : node){
			if(st.empty()){
				st.push_back(x);
				continue ;
			}
			
			while(!st.empty()&&!anc(st.back(), u)){
				st.pop_back();
			}
			vt[st.back()].push_back(u);
			st.push_back(u) ;
		}
	}
	return node[0] ;
}

void calc









