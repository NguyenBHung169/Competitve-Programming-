#include<bits/stdc++.h>


using namespace std ;



#define N 200005
#define LOG 22
#define MOD 998244353
#define all(x) (x).begin() , (x).end()
#define L(i, a, b) for(int  i =(a) ;i<(int)(b) ;i++)
#define R(i, a, b) for(int i =(a) ; i>(int)(b) ; i--)
/// round ver : base vertex in base graph , square ver : biconnected component compress to a vertex in BCT  
struct BCT{
	int W ; 
	int n , m , q ; 
	vector<int>U, V ; 
	vector<int>W ; 
	vector<vector<pair<int, int>>>g;  // base graph
	vector<int>dfn , low, stk ; 
	int timer ; 
	vector<vector<int>>comp;  // 1...n : base vertex , n +1 ... n+cnt_bcc : block node
	int tot, cnt_bcc ; 
	vector<vector<pair<int,int>>> tree ;  // bc graph ( to, occ_id)
     
    /// egde info 
    vector<int>blk , ver , type , bchain ;
    // blk[x] = a   : x in block a 
    // ver[x]   : x is what vertex in base graph
    /*type[x] = 0 / 1 / 2  : 0 : not have attribute 
    1 : pole A : dist  = 0  , 2 : pole B : dist = length of the chain  */
    /* bchain[x] : x is on which chain-id */
    /* In this problem , the answer we calculate base on the block in the block cut tree 
	and each blocks always have two source from left or right , every block have its type so we must
	have depend on the type of left source and right source and its struture in the block to calcute
	the value that this block contribute*/
	vector<int>bdist , bsum ; 
	/* bdist[x] : distance of left source a.k.a A to this x in its block 
	bsum[x] : total weight of the chain of x */
	
	
	/// block info 
	vector<int>blA , blB , numChain , blW ; 
	/* bla[i] , blb[i] : two source of block i 
	numChain[i] : number of chain from A to B in block i , blW : total egde-weight*/
	
	vector<int>dep , pedge;
	vector<array<int,LOG>>up , up2, num,sum ;
	
	 BCT(int nn = 0 , int mm = 0 ){
	 	init(nn, mm) ;
	 }
	 
	 void init(int nn , int mm){
	 	n = nn , m = mm ;
	 	U.assign(m+1,0) ; 
	 	V.assign(m+1,0);
	 	W.assign(m+1,0) ;
	 	g.assign(n+1,{});
	 }
	 void add_edge(int id , int u, int v , int w){
	 	U[id] = u , V[id] = v , W[id]=w;
	 	g[u].push_back(make_pair(v,id));
	 	g[v].push_back(make_pair(u,id));
	 }
	 int out(int x){
	 	if(x >= MOD) x-= MOD ;
	 	return x ;
	 }
	 int add(int a, int b){
	 	a += b ; 
	 	if(a>=MOD) a-= MOD ; 
	 	return a; 
	 }
	 int mul(int a, int b){
	 	return (a*b)%MOD ;
	 }
	 pair<int,int>merge(pair<int,int> x, pair<int,int>y){
	 	return make_pair(mul(x.first,y.first), add(mul(x.second,y.first),mul(x.first,y.second)));
	 }
	 
	 void tarjan(int u, int eid){
	 	dfn[u] = low[u] = ++time ;
	 	for(auto [v, id] : g[u]){
	 		if(id == eid){
	 			continue;
			 }
			 if(!dfn[v]){
			 	stk.push_back(id);
			 	tarjan(v,id);
			 	low[u] = min(low[u],low[v]);
			 	if(low[v] >= dfn[u]){
			 		vector<int>cmp;
			 		while(1){
			 			int x = stk.back();
			 			stk.pop_back();
			 			cmp.push_back(x);
			 			if(x==id) break ;
					 }
					 comp.push_back(cmp);
				 }
			 }
			 else if(dfn[v] < dfn[u]){
			 	stk.push_back(id);
			 	low[u] =min(low[u],dfn[v]);
			 }
		 }
	 }
	 
	 
	 void add_edgebct(int u, int v, int bid,int vert, int pole, int chain_id, int distA, int chainsum){
	 	int id = (int)blk.size();
	 	blk.push_back(bid);
	 	ver.push_back(vert);
	 	type.push_back(pole);
	 	bchain.push_back(chain_id);
	 	bdist.push_back(distA);
	 	bsum.push_back(chainsum);
	 	tree[u].push_back(make_pair(v,id));
	 	tree[v].push_back(make_pair(u,id));
	 }
	 
	 void built_in_block(int bid, vector<int>&ide){
	 	unordered_map<int,vector<pair<int,int>>>adj(ide.size()*2+1);
	 	vector<int>vers(ide.size()*2+1);
	 	int total = 0 ; 
	 	for(auto eid : ide){
	 		int u = U[eid] , v = V[eid] ; 
	 		adj[u].push_back(make_pair(v,eid));
	 		adj[v].push_back(make_pair(u,eid));
	 		vers.push_back(u) , vers.push_back(v);
	 		total += W[eid];
		 }
		 sort(all(verts));
		 reverse(all(verts));
		 int A = -1 , B = -1;
		 if((int)ide.isze()==1){
		 	A= U[ide[0]], B=V[ide[0]];
		 }
		 else{
		 	vector<int>poles; 
		 	for(auto x : verts){
		 		/*in a block , 2 pole A and b have deg != 2 , while other have deg = 2*/
		 		if((int)adj[x].size()!=2)poles.push_back(u);
		 		
			 }
			 if((int)poles.size()>=2){
			 	// if  block  cycle
			 	A = poles[0] , B = poles[1];
			 }
			 else{
			 	// if  block cycle
			 	A = U[ide[0]] , B = V[ide[0]];
			 }
		 }
		 blA[bid]=A , blB[bid]=B, blW[bid] = total ;
		 unorder_map<int,pair<pair<int,int>,int>>info(2*verts.size()+1);
		 unorder_set<int>used(ide.size()*2+1);
		 int cnt = 0  ;
		 for(auto [v, idd] : adj[A]){
		 	if(used.count(idd)) continue;
		 	cnt += 1;
		 	used.insert(idd);
		 	vector<pair<int,int>>path;
		 	int dist = W[idd];
		 	if(v != B){
		 		while(1){
		 			path.push_back(make_pair(v,dist));
		 			int nxt , nid;
		 			for(auto[vv,nidd] : adj[v]){
		 				if(idd!=nidd){
		 					nxt = vv, idd=nidd;
		 					break;
						 }
					 }
					 used.insert(idd);
					 v=nxt;
					 dist+=W[nidd];
					 if(cur==B) break;
				 }
			 }
			 int chainw= dist ;
			 for(auto[x, y]:path){
			 	info[x]={{cnt,y},chainw};
			 }
		 }
		 numChain[bid]=cnt;
		 for(auto v : verts){
		 	if(v == A) add_edgebct(n+bid, v,bid,v,1,-1,0,0);
		 	else if(v == B) add_edgebct(n+bid,v,bid,v,2,-1,0,0);
		 	else add_edgebct(n+bid,v,bid,v,0,info[u].first.first,info[u].first.second,info[u].second);
		 	
		 }
	 }
	 
	 pair<int,int>block_query(int x, int y){
	 	if(ver[x] == ver[y]) return make_pair(1,0);
	 	int bid = blk[x];
	 	int A = blA[bid] , b = blB[bid];
	 	int S = numChain[bid];
	 	int sumW = blW[bid];
	 	int same = 0 ;
	 /*	if(bchain[x] == bchain[y]) same = 1;
	 	if(same){
	 		int s = 0 , dx = 0 , dy = 0 ;
	 		if()
		 } */
		 if(type[x] == type[y]){
		 	return make_pair(out(S), out(R));
		 }
		 if((type[x] || type[y]) || type[x]==type[y]) same =1;
		 if(same){ // if same chain
		 	int s = 0 , dx = 0 , dy = 0 ;
		 	if(!type[x]){
		 		s= bsum[x] , dx = bdist[x]
			 }
			 if(!type[y]){
			 	s=bsum[y] , dy = bdist[y];
			 }
			 if(type[x]==1) dx=0;
			 else if(type[x]==2) dx=s;
			 if(type[y]==1)dy=0;
			 else if(type[y]==2)dy=s;
			 int ll = min(dx,dy), rr=max(dy,dy);
			 int ans = sumW + (S-2) *(ll +(s-rr));
			 return make_pair(out(S),out(ans));
		 }
		 else{
		 	int sx = bsum[x], sy=bsum[y];
			 int ans = 2*sumW + (S-3)*(sx+sy);
			 return make_pair(out(2*S-1) , out(ans));
		 }
	 }
	 
	void build(){
		dfn.assign(n+1, 0);
		low.assign(n+1, 0);
		stk.clear() ;
		comp.clear();
		timer = 0 ; 
		tarjan(1,-1);
		cnt_bcc = (int)comp.size() ; 
		tot = n + cnt_bcc ;
		
		tree.assign(tot+1,{});
		blk.clear(), ver.clear(),type.clear(),bchain.clear() ;
		bdist.clear() ; 
		bsum.clear();
		blA.assign(cnt_bcc+1,0);
		blB.assign(cnt_bcc+1,0);
		numChain.assign(cnt_bcc+1,0);
		blW.assign(cnt_bcc+1,0);
		dep.assign(tot + 1, -1);
        pedge.assign(tot + 1, -1);
        up.assign(tot + 1, {});
        up2.assign(n + 1, {});
        num.assign(n + 1, {});
        sum.assign(n + 1, {});

		for(int i = 1; i <= cnt_bcc; i++){
			built_in_block(i, comp[i-1]);
		}
		
	}
	 
};



signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ; 
	
	int n, m ,q ; 
	cin >> n >> m >> q ;
	BCT bct(n,m);
	L(i, 1 , m+1){
		int u, v, w; 
		cin >> u >>v >>W;
		bct.add_edge(i,u,v,w);
	}
	bc.build();
	while(q--){
		int s, t ; cin >> s >> t ; 
		cout << bct.solve(s,t) << endl ;
	}
	exit(0) ;
}



