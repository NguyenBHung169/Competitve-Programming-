#include<bits/stdc++.h>

using namespace std ; 

#define N 1e5 + 5
#define S 2e5 + 5
#define C 26

int n, m ; 
int block ;
vector<int>g[N];


int a[N]  , s[N];
char s[N] , t[N];
int ans = 0 ;

struct AC{
	int ch[S][C] , fa[S] , len[S] , son[S][C] , R[S] ,sz[S];
	int num[S] , ord[S] , buc[N] , ss[N];
	int tot, last ;
	AC(){
		tot = last = 1;
	}
	void insert(int c){
		int id = ++tot;
		int p = last;
		len[id]=len[p]+1;
		sz[id]=1;
		R[id] = len[id];
		while(p != 0 && !ch[p][c]) ch[p][c]=id, p=fa[p];
		if(p == 0) fa[id]=1;
		else{
			int q = ch[p][c] ; 
			if(len[q] == len[p]+1) fa[id]=q;
			else{
				int idd = ++tot;
				len[idd] = len[p] +1;
				fa[idd] = fa[q];
				for(int i= 0 ; i<26; i++){
					ch[idd][i] = ch[q][i];
				}
				fa[q] =fa[id]=idd ;
				while(p && ch[p][c]==q){
					ch[p][c] = idd , p = fa[p];
				}
			}
		}
		last = id ;
	}
	void build(){
		for(int i=1; i<tot+1 ;i++) buc[len[i]]++;
		for(int i=1 ; i< m+1 ;i++) buc[i] += buc[i-1];
		for(int i =1; i<tot+1;i++){
			ord[buc[len[i]]--] = i;
		}
		for(int i = tot ; i > 1 ; i--){
			int x = ord[i];
			int fa = fa[x];
			sz[fa] += sz[x];
			R[fa] = R[x] ;
			son[fa][s[R[x] - len[fa]]] = x ;
		}
	}
	void clear(){
		for(int i =1; i<tot+1 ; i++) num[i] = 0 ;
	}
	
	void collect(int u , int par , int st , int l , bool ban[]){
		int c = a[u] ;
		if(len[st] == l){
			st = son[st][c];
		}
		else{
			if(s[R[st]- l] != c) st = 0;
		}
		if(st == 0) return ;
		num[st]++;
		for(auto v : g[u]){
			if( v== par || ban[v]) continue ;
			coollect( v, u , st , l+1, ban);
		}
	}
	
	void push(){
		for(int i =2 ;i<tot+1; i++){
			int x = ord[i] ;
			num[x] += num[fa[x]];
		}
	}
}S1, S2;

int pos1[N] , pos2[N] ;

bool ban[N] ;
int sub[N] , sz , best , bestval ;

void getsize(int u, int par){
	sub[u] = 1;
	for(auto v : g[u]){
		if(v == par || ban[v]) continue ; 
		getsize(v, u) ; 
		sub[u] += sub[v] ;
	}
}

void getroot(int u , int par){
	sub[u] =1; 
	int mx = 0 ;
	for(auto v : g[u]){
		if(v == par || ban[v]) continue l 
		getroot(v, u);
		sub[u] += sub[v] ; 
		mx = max(mx, sub[v]);
	}
	mx = max(mx , sz - sub[u]);
	if(mx < bestval) bestval = mx , best = u ;
}
int tmp[N] , cnt ;
void list(int u , int par){
	tmp[++cnt] = u ; 
	for(auto v : g[u]){
		if( v== par || ban[v]) continue ;
		list( v, u);
	}
}
void brute(int u  , int par , int st){
	st = S1.ch[st][a[u]];
	if(st==0) return ; 
	ans += S1.sz[st];
	for(auto v : g[u]){
		if(v==par || ban[v]) continue ; 
		brute(v, u ,st) ;
	}
}
void solvebrute(int u){
	cnt =  0 ; 
	list(u, 0);
	for(int i=1; i<tc+1; i++) brute(tmp[i], 0 ,1);
}

void cal(int u , int par , int coef){
	S1.clear() , S2.clear() ; 
	if(par == 0){
		S1.collect(u, 0 , 1 , 0 ,ban);
		S2.collect(u, 0 , 1, 0 ,ban) ;
	}
	else{
		int c = a[par];
		if(S1.son[1][c]) S1.collect(u, 0 ,S1.son[1][c], 1, ban);
		if(S2.son[1][c]) S2.collect(u, 0 ,S2.con[1][c] , 0 ,ban);
		
	}
	S1.push(), S2.push() ;
	for(int i =1 ; i<m+1; i++){
		ans += coef * S1.num[pos1[i]] + S2.num[pos2[m-i+1]];
	}
	
}

void solve(int id , int szz){
	if(szz <= block){
		solvebrute(id);
		return ;
	}
	sz = szz ; 
	best = 0 , bestval = INT_MAX ;
	getroot(id, 0);
	int cen = best;
	ban[cen] = 1;
	cal(cen , 0 , 1);
	for(auto v : g[cen]){
		if(ban[v]) continue ;
		getsize(v, cen);
		cal(v, cen , -1) ;
	}
	for(auto v : g[cen]){
		if(ban[v]) continue ; 
		int subsz = sub[v] ; 
		solve(v, subsz) ;
	}
}

signed main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> n >> m ; 
	block = sqrt(n) + 1 ;
	
	for(int i =1;  i <n ; i++){
		int u, v; cin >> u >> v ;
		g[u].push_back(v) ;
		g[v].push_back(u) ;
	}
	cin >> s + 1 ;
	for(int i =1; i< n +1 ;i++){
		a[i] = s[i]-'a';
	}
	cin >> t+1;
	for(int i=1; i<m+1; i++){
		s[i] = t[i]-'a';
	}
	for(int i =1; i<m+1; i++){
		S1.insert(s[i]);
		pos1[i]=S1.last;
		S1.s[i] = s[i];
	}
	reverse(s+1, s+m+1);
	for(int i=1; i<m+1; i++){
		S2.insert(s[i]);
		pos2[i] = S2.last;
		S2.s[i] = s[i];
	}
	S1.build() ; 
	S2.build() ;
	solve(1, n); 
	cout << ans << endl ;
}






















