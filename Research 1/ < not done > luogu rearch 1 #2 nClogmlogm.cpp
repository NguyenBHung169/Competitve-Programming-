#include<bits/stdc++.h>



using namespace std ; 

#define L(i, a , b) for(int i = (a) ; i<(int)(b) ; i++)
#define R(i , a, b) for(int i =(a)  ; i>(int)(b) ; i--)
#define all(x) (x).begin() , (x).end()
#define VI vector<int>
namespace SAT2 {

	const int mN = 20005;

	VI e[mN];
	int n, cnt, dfn[mN], low[mN], st[mN], bel[mN], top, ind;
	bool ins[mN];

	// val[i] = giá trị của biến i (0/1)
	// giả sử mỗi biến i có 2 đỉnh:
	// i<<1     : false
	// i<<1 | 1 : true
	bool val[mN >> 1];

	void init(int ct) {
		n = ct;              // tổng số đỉnh của implication graph = 2 * số biến
		cnt = 0;
		top = 0;
		ind = 0;
		L(i, 0, n) {
			e[i].clear();
			dfn[i] = low[i] = bel[i] = 0;
			ins[i] = 0;
		}
	}

	void add(int u, int v) {
		e[u].push_back(v);
	}

	// phủ định của literal x
	int neg(int x) {
		return x ^ 1;
	}

	// thêm mệnh đề kéo theo: u -> v
	void add_imp(int u, int v) {
		add(u, v);
	}

	// thêm mệnh đề (u OR v)
	// tương đương (!u -> v) và (!v -> u)
	void add_or(int u, int v) {
		add(neg(u), v);
		add(neg(v), u);
	}

	// ép literal u = true
	// tức là (!u -> u)
	void add_true(int u) {
		add(neg(u), u);
	}

	// ép literal u = false
	// tức là (u -> !u)
	void add_false(int u) {
		add(u, neg(u));
	}

	// u và v phải cùng giá trị
	// (u == v) <=> (u -> v) & (v -> u) & (!u -> !v) & (!v -> !u)
	void add_equal(int u, int v) {
		add(u, v);
		add(v, u);
		add(neg(u), neg(v));
		add(neg(v), neg(u));
	}

	// u và v phải khác giá trị
	// (u != v) <=> (u OR v) & (!u OR !v)
	void add_xor(int u, int v) {
		add_or(u, v);
		add_or(neg(u), neg(v));
	}
	// thêm mệnh đề (u OR v)
    // tương đương (!u -> v) và (!v -> u)
    inline void add_or(int u, int v) {
        add(neg(u), v);
        add(neg(v), u);
    }

    // gỡ mệnh đề (u OR v) khỏi graph
    inline void remove_or(int u, int v) {
        e[neg(u)][v] = 0;
        g[v][neg(u)] = 0;

        e[neg(v)][u] = 0;
        g[u][neg(v)] = 0;
    }



	void tarjan(int u) {
		dfn[u] = low[u] = ++ind;
		ins[u] = 1;
		st[++top] = u;

		L(i, 0, (int)e[u].size()) {
			int v = e[u][i];
			if (!dfn[v]) {
				tarjan(v);
				low[u] = min(low[u], low[v]);
			}
			else if (ins[v]) {
				low[u] = min(low[u], dfn[v]); // Tarjan chuẩn: phải là dfn[v]
			}
		}

		if (dfn[u] == low[u]) {
			++cnt;
			while (1) {
				bel[st[top]] = cnt;
				ins[st[top]] = 0;
				if (st[top--] == u) break;
			}
		}
	}

	// chạy SCC
	void solve() {
		L(i, 0, n) {
			if (!dfn[i]) tarjan(i);
		}
		// Với Tarjan kiểu này:
		// nếu có cạnh SCC A -> SCC B thì bel[A] >= bel[B]
	}

	// vars = số biến
	// trả về true nếu có nghiệm, đồng thời sinh nghiệm vào val[]
	bool satisfiable(int vars) {
		solve();

		L(i, 0, vars) {
			if (bel[i << 1] == bel[i << 1 | 1]) return 0; // x và !x cùng SCC => vô nghiệm
		}

		L(i, 0, vars) {
			// SCC có topo đứng "trước" hơn sẽ quyết định giá trị
			// với quy ước bel lớn hơn đi về bel nhỏ hơn,
			// cách gán chuẩn là:
			val[i] = (bel[i << 1] < bel[i << 1 | 1]);
		}
		return 1;
	}
};
 const int N = 205 ;
 int a[N][N] ;
 int  n; 
 int fa[N] ;
 int path_xor[N] ;
 VI cand ;
 // two function apply for checking odd cycle
 int find(int x){
 	if(fa[x] == x) return x ;
 	int p = find(fa[x]) ;
 	path_xor[x] ^= path_xor[fa[x]];
 	return fa[x] =p;
 }
 
 bool unite(int x, int y){
 	int fx = find(x) ;
 	int fy = find(y) ;
 	if(fx == fy) return  (path_xor[x] ^ path_xor[y] ) == 1 ;
 	path_xor[fx] = path_xor[x] ^ path_xor[v] ^1 ;
 	fa[fx] =fy ;
 	return 1;
 }
 pair<int , int>s[N*N] ;
 int cnt , cl , cr ;
 bool check(int x , int y){
 	  // int lf = 0 , rt = tot ;
 	  int ini_cr = cr , ini_cl = cl ;
 	 while(cl_cand[cl].first >x ){
 	 	remove_or(cl , y);
 	 	cl--;
	  }
	  cl = ini_cl;
	  while(cl_cand[cr].first() > x){
	  	remove_or(cr , x);
	  	cr--;
	  }
	  cr = ini_cr;
	  while(cr_cand[cl].second() > y){
	  	remove_or(cl, x);
	  	cl--;
	  }
	  while(cr_cand[cr].second() > x){
	  	remove_or(cr, y);
	  	cr--;
	  }
	
	  
	 return SAT2::satisfiable(n); // check if clauses matching
 }
 
 void solve(int l, int r, int L, int R){
 	if(l > r || L > R) return ; 
 	int mid = l + r >> 1 ;
 	int cands = cand[mid] ;
 	int LL = L , RR = R ;
 	while(LL<RR){
 		int m = LL+RR>>1;
 		if(check(cands,m)) RR = m;
 		else LL = m+1;
	 }
	 if(check(cands, LL)) ans - min(ans, val[cands]+val[LL]);
	 solve(l,mid-1,LL,R); solve(mid+1,r,L,RR);
 }

signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ;
    cin >> n ;
    VI val ;
    val.push_back(0) ;
	L(i, 0, n){
		L(j , i+1 , n){
			cin >> a[i][j];
			a[j][i] = a[i][j] ;
			val.push_back(a[i][j]);
		}
	}
	sort(all(val)) ;
	val.erase(unique(all(val)),val.end());
	//compress value 
	L(i,0,n) L(j,i+1,n){
		a[i][j] = a[j][i] = lower_bound(all(val) , a[i][j]) - val.begin();
	}
	vector<vector<pair<int,int>>>check_parity((int)val.size());
	vector<pair<int,int>>edge;
	int cnt = 0 ;
	L(i,0,n){
		L(j,i+1,n){
			edge.push_back(make_pair(i, j));
			
			check_parity[a[i][j]].push_back({i, j});
		}
	}
	L(i,0,n){
		fa[i] = i ;
		path_xor[i] = 0;
	}
//	VI cand ; // candidate value for A , to binary search B
	bool ok = 0 ;
	R(id ,(int)val.size()-1 , -1){
		for(auto [u, v] : check_parity[id]){
			int fu = find(u) , fv = find(v) ;
			if(fu==fv){if(path_xor[u] ^ path_xor[v] ==0){ //odd cycle
				cand.push_back(id);
				ok =1;
				break;
			}
			continue;
		}
			
			cand.push_back(id);
			unite(u, v);
			
		}
		if(ok) break ;
	}
	cand.push_back(0);
	sort(all(cand));
	cand.erase(unique(all(cand)), cand.end()) ;
	int ans = 1e9 ;
	tot = 0 ; 
	cl = tot = 0 ;
	cr = (int)cand.size() ;
	L(i , 0 , (int)cand.size()){
		cl_cand[++tot]  = make_pair(cand[i] , cand[(int)cand.size() - i +1]);
		cr_cand[tot]    = make_pair(cand[(int)cand.size()-i+1] , cand[i]);
	}
	sort(all(cl_cand));   
	reverse(all(cl_cand)) ;
	sort(all(cr_cand));
/*	for(auto cands : cand){
		int l = 0 , r = (int)val.size()-1 , best = -1 ;
		while(l < r){
			int mid = l + r >> 1 ;
			if(check(cands, mid)){
				r = mid - 1 ;
				best = mid ;
			}
			else l = mid + 1 ;
		}
		if(best == -1) continue ;
		ans = min(ans, val[cands] + val[best]) ; // find min of A and B 
	}
*/  
    solve(0, (int)cand.size() -1 , 0 , (int)val.size()-1);  // using divide and conquer to decrease the complexity
	cout << ans << endl ;
}
