#include<bits/stdc++.h>

using namespace std ;

#define N 1e5 + 5 
#define inf 1e9 + 10 

int n , m ,t , l, x[N];

struct egde{
	int u, v, w ;
}e[N];

vector<int>g[N] ;

int vis[N] ;
pair<int, int>dis[N][N][2] ;

priority_queue<pair<int,int>, vector<pair<int,int>,greater<pair<int,int>>pq;

struct path{
	int st , ed ; 
	int len ; 
	path(){
		st = -1 , ed = -1 , len = -inf ;
	}
};

void op(path&x , path y){
	if(x.len > y.len) x = y ;
}

array<path , 4>cand[N][N];

struct ST{
	array<path,4>t[N] ;
	array<path , 4 > merge(array<path,4>x, array<path , 4>y){
		path tmp[16];
		int tot = 0 ;
		for(int i = 0 ; i < 4; i++){
			for(int j = 0 ; j < 4; j++){
				if(x[i].ed!=y[i].st){
					tmp[++tot] = {x[i].st , y[j].ed, x[i].len+y[j].len};
				}
			}
		}
		array<path , 4>z;
		assert(tot >=4) ;
		for(int i = 0 ; i < tot; i++){
			op(z[0],tmp[i]);
		}
		for(int i = 0 ; i<tot; i++){
			if(tmp[i].ed !=z[0].ed ||tmp[i].st != z[0].st) op(z[1],tmp[i]);
		}
		if(z[0].st == z[1].st){
			for(int i = 0 ; i<tot; i++){
				if(tmp[i].st!=z[0].st)op(z[2],tmp[i]);
			}
			for(int i = 0 ; i < tot; i++){
				if(tmp[i].st!=z[0].st &&tmp[i].ed!=z[2].ed)op(z[3],tmp[i]);
			}
		}
		else if(z[0].ed == z[1].ed){
			for(int i= 0 ; i<tot;i++){
				if(tmp[i].ed!=z[0].ed) op(z[2],tmp[i]);
			}
			for(int i = 0 ; i<tot; i++){
				if(tmp[i].ed!=z[0].ed&&tmp[i].st!=z[2].st) op(z[3],tmp[i]);
			}
		}
		else{
			for(int i = 0 ; i<tot; i++){
				if(tmp[i].st!=z[0].st && tmp[i].ed!=z[1].ed) op(z[2],tmp[i]);
				
			}
			for(int i = 0 ; i<tot;i++){
				if(tmp[i].st!=z[1].st&&tmp[i].ed!=z[0].ed) op(z[3],tmp[i]);
			}
		}
		return z ;
	}
#define mid l + r >> 1 	
	void build(int u, int l, int r){
		if(l==r){
			t[u] = cand[x[l]][x[l+1]];
			return;
		}
		build(u<<1, l ,mid);
		build(u<<1|1 , mid+1, r);
		t[u] = merge(t[u<<1] , t[u<<1|1]);
	}
	
	void modify(int u , int l ,int r , int x, array<path , 4>y){
		if(l==r){
			t[x] = y ; 
			return ;
		}
		if(x<=mid) modify(u<<1, l, mid, x, y);
		else modify(u<<1|1, mid+1, r, x, y);
		t[u]= merge(t[u<<1],t[u<<1|1]);
	}
	int query(){
		if(t[1][0].len >= inf){
			return -1 ; 
		}
		return t[1][0].len ; 
	}
}st;

void find(int a, int b, int ban1, int ban2 , path&ans){
	for(int i = 0 ; i<(int)g[a].size();i++){
		if(g[a][i]==ban1) continue ; 
		for(int j = 0 ; j <2;j++){
			if(dis[i][b][j].second==ban2) continue ;
			op(ans, path(g[a][i],dis[i][b][j].second,dis[i][b][j].first));
		}
	}
}

signed main(){
	ios :: sync_with_stdio(false);
	cin.tie(0);
	cin >> n >> m >> t >> l ;
	for(int i = 0; i < m ; i++){
		cin >> e[i].u >> e[i].v >> e[i].w;
		g[e[i].u].push_back(i), g[e[i].v].push_back(i);
	}
	for(int i =1; i<n+1; i++){
		for(int j = 0 ; j<(int)g[i].size();j++){
			// build dis[j][k][0]
			for(int nxt =1; nxt <n+1; nxt++) dis[j][p][0]=dis[j][p][1]=inf;
			int id = g[i][j]; // id of the edge base on the loop of the input
			int k = e[id].u ^ e[id].v ^ i ;
			memset(vis, 0, sizeof(vis));
			pq.push({e[x].w, k});
			dis[j][k][0] = make_pair(e[x].w , x);
			while(!pq.empty()){
				int cur = pq.top().second();
				pq.pop();
				if(vis[cur]) continue;
				vis[cur]=1;
				for(auto nxt :g[cur]){
					int nxtid = e[nxt].u ^ e[nxt].v ^ cur;
					if(nxt==dis[j][cur][0].second) continue;
					if(dis[j][nxtid][0].first > dis[j][cur][0].first+e[nxt].w){
						dis[j][nxtid][0] = make_pair(dis[j][cur][0]+e[nxt].w,nxt);
						pq.push({dis[j][nxtid][0] , nxtid});
					}
				}
			}
			// build dis[j][k][1]
			for(int nxt =1; nxt <n+1; nxt++){
			//	int id = g[i][j]; // id of the edge base on the loop of the input
			    int k = e[id].u ^ e[id].v ^ i ;
			    memset(vis, 0, sizeof(vis));
			    pq.push({e[x].w, k});
			    dis[j][k][0] = make_pair(e[x].w , x);
			    while(!pq.empty()){
				   int cur = pq.top().second();
				   pq.pop();
				   if(vis[cur]) continue;
				   vis[cur]=1;
				   for(auto nxt :g[cur]){
					   int nxtid = e[nxt].u ^ e[nxt].v ^ cur;
					   if(nxt==dis[j][cur][0].second) continue;
					   if(dis[j][nxtid][0].first > dis[j][cur][0].first+e[nxt].w){
						   dis[j][nxtid][0] = make_pair(dis[j][cur][0]+e[nxt].w,nxt);
						   pq.push({dis[j][nxtid][0] , nxtid});
					    }
				    }
			    }
			}
			
		}
	}
}











