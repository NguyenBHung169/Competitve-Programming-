#include<bits/stdc++.h>


using namespace std ; 
#define L(i , j, k) for(int i = j ; i < (int)k ; i++)
#define R(i, j , k) for(int  i = j ; i>(int)k ; i--)
#define N 5e4 + 5 
#define S 1e5 + 5 
#define B 8 
#define MSK 1 << 8

int n, m , q ;
string s[N] ;
int a[N] , pos[N];



int tot = 1;
int nxt[S][2];
int fail[S];
vector<int>failTree[S];

int to[S][MSK];
int cnt[S][MSK];
int sum[S];

vector<int>block[N];


void ins(int id, string &s){
	int p = 1;
	for(char c : s){
		int ch = c - 'a';
		if(!nxt[p][ch]) nxt[p][ch] = ++tot;
		p=nxt[p][ch];
	}
	pos[id] = p;
}

void build(){
	queue<int>q;
	for(int i = 0 ; i < 2; i++){
		if(nxt[1][i]){
			fail[nxt[1][i]]=1;
			q.push(nxt[1][i]);
		}
		else nxt[1][i]=1;
	}
	while(!q.empty()){
		int f= q.front(); q.pop();
		for(int i = 0 ; i<2;i++){
			if(nxt[f][i]){
				fail[nxt[f][i]] = nxt[fail[f]][i];
				q.push(nxt[f][i]);
			}
			else nxt[f][i] = nxt[fail[f]][i];
		}
	}
	for(int i = 2; i< tot+1 ; i++){
		failTree[fail[i]].push_back(i);
	}
}

void dfs(int u){
	for(auto v : failTree[u]){
		dfs(v);
		sum[u] += sum[v];
	}
}

signed main(){
	ios :: sync_with_stdio(false);
	cin.tie(0) ;
	cin >> n >> m >> q ;
	for(int i =1; i<=n ; i++) cin >> s[i];
	for(int i = 1; i<=m; i++) cin >> a[i];
	for(int i=1; i<=n ; i++){
		int len = (int)s[i].size();
		if(len < B) continue;
		block[i].resize(len-B+1);
		int msk = 0 ;
		for(int k = 0; k < B ; k++){
			msk = (msk << 1)  | (s[i][k]-'a');
		}
		block[i][0] = msk ;
		for(int j =1; j < len - B +1 ; j++){
			msk = (msk <<1) & (MSK-1) ;
			msk |=( s[i][j+B-1]-'a');
			block[i][j]= msk ;
		}
	}
	
	for(int i=1; i<=q; i++){
		string query ; cin >> query;
		ins(i, query);
	}
	build();
	
	for(int i =1; i < tot+1 ; i++){
		to[i][0]=i ;
		for(int j = 1; j<= B ; j++){
			for(int mask =(1<<j)-1;mask >=0; mask--){
				to[i][mask]=nxt[to[i][mask>>1]][mask & 1];
			}
		}
	}
	int now =1;
	for(int i = 1; i<=m ; i++){
		int id = a[i] , pos= 0;
		int len = (int)s[id].size();
		while(pos + B <= len){
			int mask = block[id][pos];
			cnt[now][mask] += 1;
			now = to[now][mask];
			pos += B ;
		}
		while(pos < len){
			now = nxt[now][s[id][pos]-'a'];
			sum[now] += 1;
			pos+= 1;
		}
		
		
	}
	// PIE 
	for(int i=1; i< tot+1;i++){
		for(int mask = 0 ; mask < (1<<8) ; mask++){
			int time  = cnt[i][mask];
			if(time == 0) continue;
			int now = i ; 
			for(int k = B-1;~k ; k--){
					now  = nxt[now][(mask >> k)&1] ;
					sum[now] += time ;
				}
			}
		}
	dfs(1) ;
	for(int i=1 ; i<q+1 ; i++){
		cout << sum[pos[i]] << endl ;
	}
}
