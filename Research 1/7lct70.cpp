
#include<bits/stdc++.h>


using namespace std ; 
#define maxN 20005 
#define INF 1e9 +5


const int N = 1e5 + 9;
int cnt ; 
int to[N] , w[N] , nxt[N] , head[N] ;

void add(int u , int v , int www){
	to[++cnt] = v ;    // the next vertex
	w[cnt] = www ;   // current capacity of the edge
	nxt[cnt] = head[u];  // other edge that point to current node , the translation when the min egde now death
	head[u] = cnt ;  // min index egde alive 
	// if that edge death head[x] = nxt[head[x]]
}
struct LCT {
  struct Node{
  	int p = 0, val = 0 , mn = INF , lazy  = 0 , sz = 0 ;
  	int c[2] ={0 , 0} ; // left child && right child
  };
  vector<Node> t;
  void init(int n){
  	t.assign(n+1, Node());
  	t[0].mn = INF ;
  	t[0].sz = 0 ;
  }
  LCT() {}
  LCT(int n) {init(n);}

 
  int dir(int x, int y) { return t[x].c[1] == y; }
  void apply_dec(int v, int value){
  	if(!x) return ;
  	t[x].val -= value ;
  	t[x].mn  -= value ;
  	t[x].lazy+= value ;
  }
 
  void push(int x) { 
    if (!x) return;
    if(t[x].lazy == 0) return ;
    if(t[x].c[0]) apply_dec(t[x].c[0] , t[x].lazy);
    if(t[x].c[1]) apply_dec(t[x].c[1], t[x].lazy) ;
    t[x].lazy = 0 ;
  }
  void pull(int x){
  	if(!x) return ; 
  	t[x].sz = t[t[x].c[0]].sz + t[t[x].c[1]].sz+1 ;
  	t[x].mn = min({t[t[x].c[0]].mn , t[t[x].c[1]].mn ,t[x].val});
  }
  bool is_root(int x){
  	int p = t[x].p ;
	  return !(t[p].c[0]== x || t[p].c[1]==x); 
  }
  void set_child(int x, int d, int y){
  	t[x].c[d] = y ; 
  	if(y) t[y].p = x; 
  }
  void rotate(int x) { 
    int y = t[x].p, z = t[y].p;
  //  swap(t[x].pp, t[y].pp);
   int dx = dir(y, x) ;
   int dy = dir(z, y) ;
   int b = t[x].c[dx^1] ;
   if(is_root(y) == false) t[z].c[dy] =x ;
   t[x].p = z ;
   set_child(y, dx, b) ; 
   set_child(x, dx^1 , y) ;
   pull(y) ; pull(x) ; 
  }
  void push_path(int x){
  	int st[N] ; 
  	int top = 0 ; 
  	st[++top] = x ;
  	while(!is_root(x)) st[++top] = x = t[x].p ; 
	  while(top) push(st[top--]); 
  }
  void splay(int x) { 
    push_path(x) ; 
    while(!is_root(x)){
    	int y = t[x].p; 
		int z = t[y].p;
		if(!is_root(y)){
			if(dir(y,x)==dir(z,y)) rotate(y);
			else rotate(x);
		}
		rotate(x);
	}
	pull(x);
  }
  
  int access(int x) {
    int lst = 0 ;
    for(int y = 0 ; x ; y = x , x =t[x].p){
    	splay(x);
    	t[x].c[1]=y;
    	pull(x);
    	lst = x ;
	}
	return lst ;
  }
  int find_root(int x){
  	access(x) ;
  	splay(x) ;
  	while(t[x].c[0]){
  		push(x);
  		x= t[x].c[0];
	}
	splay(x);
	return x;
  }
  
};

int n , m , q ;

int uu[N], vv[N] , ww[N] ; 
vector<int>build_rev[N] ; 
bool vis[N] ; 
LCT lct ; 
// build the reverse graph to know which vertex point to the cur node ,app;y for link cut tree operation 
void dfs(int u , int p){
	lct.t[u].p = p ;
	vis[u] = 1; 
	for(auto v : build_rev[u]){
		if(!vis[v]) dfs(v, u);
	}
}
// find min index node with the w a.k.a capacity have value 1
pair<int , int>find_min_node(int x){
	if(lct.t[x].mn > 1) return make_pair(0, 0);
	int cnt = 0 ; 
	while(1){
		lct.push(x) ; 
		int l = lct.t[x].c[0] ; 
		int r = lct.t[x].c[1] ;
		// the path in the lct from right - cur node - left , so we iterate from right first 
		if(r && lct.t[r].mn == 1) cnt += lct.t[l].sz +1, x = r ;   // check if have right child and right have the min =1 on its side
		else if(lct.t[x].val==1) return make_pair(x, cnt + 1 +lct.t[l].sz); // if the current node now have capacity 1 , we make pair ( its index , its order-traversal index )
		else x = l ;   // if right and current unvalid , go to the left
	}
}
// decrease the capacity of the prefix first d-vertex path ( right - current node - left with 1 )
void dec(int x, int d){
	if(x == 0 || d <= 0) return ;
	if(d >= lct.t[x].sz){
		lct.apply_dec(x, 1) ;
		return ;
	}
	lct.push(x) ;
	int l = lct.t[x].c[0] , r = lct.t[x].c[1];
	dec(r, d) ;
	d-= lct.t[r].sz ; 
	if(d > 0){
		d -= 1 ; 
		lct.t[x].val -= 1 ;
	}
	dec(l, d);
	lct.pull(x) ;
}
// if the path dont have the node can be deleted ( val / capacity = 1 ) we simply output the d_th node of the query(start node , d_th node on the path)
int find_dth(int x, int d){
	while(1){
		lct.push(x);
		int l = lct.t[x].c[0] , r = lct.t[x].c[1];
		if(d <= lct.t[r].sz){
			x = r ; 
			continue ;
		}
		d-= lct.t[r].sz ; 
		if(d == 1) return x ; 
		d -= 1 ; 
		x = l ;
	}
}
/* if we notice that there is a egde with cap / val = 1 , this edge must be change , we do operation on the current node with that edge and we replace the current node
 with another valid egde */
void change(int x, int rt){ // x mean that node having the out egde that we must cutafter that we assign other egde nxt [,,,] for the affterlife query
	lct.access(to[head[rt]]) ;   //show the preferedpath of the lct 
	auto  p = lct.access(x) ;   
	lct.splay(x) ;
	// if x== rt mean this egde do not represented in the lct , so dont need to ope again on lct , only assign it on (*)
	if(x != rt){ // mean that this egde represented in the lct , so we ope on it again to make the lct correctly
		lct.access(to[head[x]]);
		lct.splay(x);
		lct.t[x].p = 0 ;
	}
	head[x] = nxt[head[x]] ;  //(*)
	if(x == p && x != rt){   // x=p mean that this egde on an cycle , so we must ope on the rt
		lct.splay(rt) ; 
		lct.t[rt].p = to[head[rt]] ;
	}
	int fr = lct.find(root(to[head[x]])) ;  // the ver x point to now
	if(fr != x){  // because this new egde is a tree-edge so we ope it on the lct
		lct.splay(x) ;
		lct.t[x].p = to[head[x]] ;
 	}
 	lct.splay(x) ;
 	lct.t[x].val = w[head[x]]; 
 	lct.pull(x) ;
}

int solve(int x , int d){
	if(d == 0 ) return x ; // if d= 0 , the cur vertex is ans
	int rt = lct.find_root(x);
	// access + splay : opes to chain the preferred path and do operations
	lct.access(x) ;
	lct.splay(x) ;
	// if x is on a path that go into the cycle , x jump only on that cycle
	if(x == to[head[rt]]){
		int jump = min(d/lct.t[x].sz , lct.t[x].mn -1) ;
		if(jump > 0 ){
			lct.apply_dec(x, jump) ;
			d-= jump * lct.t[x].sz ; 
		}
	}
	if(d == 0 ) return x ; 
	pair<int, int>node = find_min_node(x) ; // find node have the point-egde with cap / val =1
	if(node.first){ // if it exists
		int safe_steps = lct.t[x].sz - node.second() ;  // number of step to that node
		dec(x, min(d, safe_steps));  // apply -1 because we traver on that path
		if(safe_steps <= d){  // if we must over that egde
			int nxt = to[head[node.first]];   // we jump over that egde , after that its vap become 0 
			d-= safe_steps ;
			change(node.first, rt);  // change its point-edge
		//	apply_dec(x, safe_step)
			return solve(nxt , d-1) ;  // recursive solve of the node now 
		}
		else{ // if we don't have to jump over that node , we output the d-th node
			auto ans = find_dth(x,d) ;
			return to[head[ans]] ;
		}
	}
	// if it not exists
	dec(x, min(d, lct.t[x].sz)) ;  // decrease cap
	if(d > lct.t[x].sz){
		solve(to[head[rt]], d - lct.t[x].sz );
	}
	else{
		auto who = find_dth(x,d);
		return to[head[who]];
	}
	
}




#define INF 1e9 + 10 
signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ; 
	cin >> n >> m >> q ; 
	for(int i = 0  ; i < m ; i++){
		cin >> uu[i] >> vv[i] >> ww[i] ;
	}

	for(int i = 0 ; i < m ; i++) add(i, i , INF) ;
	for(int i = m -1; i>=0 ; i--){
		add(uu[i],vv[i],ww[i]);
	};
	lct.init(n) ; 
	for(int i = 0 ; i < m ;i++) build_rev[to[head[i]]].push_back(i);
	for(int  i = 0 ; i < n ; i ++){
		if(!vis[i]){
			int j = i ;
			while(!vis[j]){
				vis[j]=1;
				j = to[head[j]];
			}
			for(auto p = i ; vis[p] = true ; p = to[head[p]] ) vis[p]= false;
			dfs(j, 0);
		}
	}
	for(int i  =0 ; i < n;  i++){
		lct.t[i].mn = lct.t[i].val = w[head[i]];
		lct.t[i].sz = 1;
		
	}	
	while(q--){
		int x, s; cin >>x >>s ; 
		cout<< solve(x,s) ; 
		cout<<endl ; 
	}
}
