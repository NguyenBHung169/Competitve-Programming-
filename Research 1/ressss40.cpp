#include<bits/stdc++.h>


using namespace std ; 



const int N = 5e5 + 10 ; 

#define L( i , j , k )   for( int i = (j) ; i < (int)(k) ; i++ )

struct BIT{
	int n ; 
	vector<int> bit ;
	
	void init(int _n){
		n = _n ;
		bit.assign(n+1, 0) ;
	}
	void add(int x, int value){
		for( ; x <= n ; x += (x & -x)){
			bit[x] += value  ;
		}
	}
	
	int prefixSum(int x){
		int sum = 0 ; 
		for(; x > 0 ; x -= (x & -x) ) sum += bit[x];
		return sum ;
	}
	
	int rangeSum(int l, int r){
		if(l > r) return 0 ; 
		return prefixSum(r) - prefixSum(l-1) ;
	}
}fenwick;


int n, q ;
vector<int> g[N];

int fa[N], dep[N] , sz[N] ,son[N] ;
int top[N] , dfn[N] , rdfn[N] , time = 0 ;

void dfs1(int u , int p){
	fa[u] = p ; 
	sz[u] = 1 ; 
	son[u] = 0 ; 
	for(auto v : g[u]){
		if(v == p) continue ; 
		dep[v] =dep[u] + 1 ;
		dfs1(v, u) ;
		sz[u] += sz[v] ; 
		if(!son[u] || sz[v] > sz[son[u]]){
			son[u] = v ;
		}
	}
}

void dfs2(int u, int chain ){
	top[u] = chain ; 
	dfn[u] = ++time ; 
	rdfn[time] = u ; 
	if(son[u]) dfs2(son[u] ,chain) ; 
	for(auto v : g[u]){
		if(v== fa[u] || v== son[u]) continue ; 
		dfs2(v, u);
	}
}

int get(int u){
	return fenwick.rangeSum(dfn[u], dfn[u] + sz[u] - 1) ;
}

bool in[N] ;  // in[x] : check if x is in the set C 
int X = 0 , Y = 0  ; // variables apply for the inside-set and outside-set 

set<int>ins[N], out[N] ; 

struct Data{
	set<int>inside , outside ; 
	int idIn  = 0, idOut = 0  ;  
	int valIn = 0,  valOut = 0  ;
}chain[N];

void removeIDIn(int x){
	if(chain[x].idIn) ins[chain[x].valIn ].erase(chain[x].idIn );
}

void removeIDOut(int x){
	if(chain[x].idOut) out[chain[x].valOut].erase(chain[x].idOut) ;
}

void rebuildIDIn(int x){
	removeIDIn(x);
	if(chain[x].inside.empty()){
		chain[x].idIn = 0 ;
		chain[x].valIn = 0 ;
		return ;
	}
	int max_depth = rdfn[*chain[x].inside.rbegin()];
	chain[x].idIn = max_depth ; 
	chain[x].valIn = get(max_depth) ;
	ins[chain[x].valIn].insert(max_depth) ;
}
// for outside the set take the nearly root vertex because the value of vertex  nearly the root bigger so  is better if those  in the set
void rebuildIDOut(int x){
	removeIDOut(x) ;
	if(chain[x].outside.empty()){
		chain[x].idOut = 0 ;
		chain[x].valOut = 0 ;
		return ;
	}
	int min_depth = rdfn[*chain[x].outside.begin()] ;
	chain[x].idOut = min_depth ; 
	chain[x].valOut = get(min_depth) ; 
	out[chain[x].valOut ].insert(min_depth) ;
}


void addIn(int x){
	int h = top[x] ;
	chain[h].inside.insert(dfn[x]);
	rebuildIDIn(h) ;
}
void addOut(int x){
	int h = top[x] ;
	chain[h].outside.insert(dfn[x]);
	rebuildIDOut(h);
}

void delIn(int x){
	int h = top[x] ;
	chain[h].inside.erase(dfn[x]);
	if(chain[h].idIn == x) rebuildIDIn(h);
}
void delOut(int x){
	int h = top[x] ; 
	chain[h].outside.erase(dfn[x]);
	if(chain[h].idOut == x) rebuildIDOut(h);
}

bool inRange(int x, int lf, int rt){
	return dfn[x] >= dfn[lf] && dfn[x] <= dfn[rt] ;
}


void moveOI(int x){
	delOut(x);
	addIn(x);
	in[x] = 1 ;
	X += 1 ;
}

void moveIO(int x){
	delIn(x) ; 
	addOut(x) ;
	in[x] = 0 ; 
	X -= 1;
}

void active(int x){
	fenwick.add(dfn[x] , 1) ;
	for(auto v = x ; v ; v = fa[top[v]]){
		int h = top[v] ;
		auto &boss = chain[h] ;
		if(boss.idIn && inRange(boss.idIn, h, v)){
			ins[boss.valIn].erase(boss.idIn);
			boss.valIn += 1 ;
			ins[boss.valIn].insert(boss.idIn);
		}
		if(boss.idOut && inRange(boss.idOut , h ,v)){
			out[boss.valOut].erase(boss.idOut);
			boss.valOut += 1 ;
			out[boss.valOut].insert(boss.idOut);
		}
		// if the weight of this top exceed Y , move it to the set C 
		if(boss.idOut != 0 && boss.valOut > Y){
			moveOI(boss.idOut);
		}
	}
	if(get(x) > Y) addIn(x) , in[x] = 1 , X++ ;
	else addOut(x) , in[x] = 0 ;
}

void deactive(int x){

	if(in[x]){
		delIn(x) ; 
		in[x] = 0 ;
		X -= 1 ;
	}
	else delOut(x) ;
	fenwick.add(dfn[x], -1);
	for(auto v = x ; v ; v = fa[top[v]]){
		int h = top[v] ; 
		auto &boss = chain[h] ;
		if(boss.idIn && inRange(boss.idIn, h, v)){
			ins[boss.valIn].erase(boss.idIn);
			boss.valIn -= 1 ;
			ins[boss.valIn].insert(boss.idIn);
		}
		if(boss.idOut && inRange(boss.idOut , h ,v)){
			out[boss.valOut].erase(boss.idOut);
			boss.valOut -= 1 ;
			out[boss.valOut].insert(boss.idOut);
		}
		if(boss.idIn != 0 && boss.valIn < Y){
			moveIO(boss.idIn);
		}
	}
}

void balance(){
	while(X > Y){
		if(ins[Y].empty() == false){
			int x = *ins[Y].begin() ; 
			moveIO(x);
		}
		else Y += 1 ;
	}
	while(X < Y){
		if(out[Y].empty() == false){
			int x = *out[Y].begin() ; 
			moveOI(x) ;
		}
		else Y -= 1 ;
	}
}





signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ; 
	
	
	
	cin >> n ; 
	L(i, 1, n){
		int u , v ; 
		cin >> u >> v ;
		g[u].push_back(v) ;
		g[v].push_back(u) ;
	}
	dep[1] = 1 ; 
	dfs1(1, 0) ; 
	dfs2(1, 1) ;
	fenwick.init(n) ;
	cin >> q ; 
	while(q--){
		int type , x ; 
		cin >> type >> x ; 
		if(type == 1) active(x) ; 
		else deactive(x);
		// after do operation , balance the max(X, Y) again 
		balance() ;
		cout << X << endl ; 
	}
} 


/* ALgorithms :  we greedily take in the set C which vertex having the bigger W , we notice that : 
    
    Having X = number of vertex in the set C , Y = the max W-value outside the set C , we try to make the min W[x] , x in C >= max W[x] , w not in C 
    
    - when X > Y , try to put the smallest W of non C vertex to C which W > Y , so X := X-1 , Y = max(Y, W[that_vertex])  . Notice that max(X, Y) can decreased or equal the initialize value
    - when X < Y , put the biggest in setC outside so the Y must decrased and X := X + 1 . Notice that this is not really make big different








*/






