#include<bits/stdc++.h>

using namespace std ; 
#define inf 0x3f3f 

struct Dinic{
	struct Edge{
		int to , rev , cap ; 
		int id ; 
	};
	int n , s, t ; 
	vector<vector<Edge>> g;
	vector<int>dis, cur ; 
	Dinic(int n , int s, int t) : n(n) , s(s) , t(t){
		g.resize(n)  ;
		dis.resize(n) ; 
		cur.resize(n) ; 
	}
	void add(int u , int v, int cap ,int id = 0 ){
		Edge a{v, (int)g[v].size() , cap , id} ; 
		Edge b{u , (int)g[u].size() , 0 , 0} ; 
		g[v].push_back(a) ; 
		g[u].push_back(b) ; 
	}
	bool bfs(){
		fill(dis.begin() , dis.end() ,-1) ; 
		queue<int>q ; 
		q.push(s) ;
		dis[s] = 0 ; 
		while(!q.empty()){
			int u = q.front() ; 
			q.pop() ; 
			for(Edge &e :g[u]){
				if(e.cap >0 && dis[e.to] ==-1){
					dis[e.to]  =dis[u]+1 ; 
					q.push(e.to) ; 
				}
			}
		}
		return dis[t] != -1 ;
	}
	
	int dfs(int u , int fl){
		if(u ==t) return fl ; 
		for(int i = cur[u] ; i<(int)g[u].size() ; i++){
			Edge &e = g[u][i] ; 
			if(e.cap ==0 ||dis[e.to]!=dis[u]+1) continue ; 
			int pushed = dfs(e.to , min(fl , e.cap)) ; 
			if(!pushed) continue ; 
			e.cap -= pushed ; 
			g[e.to][e.rev].cap += pushed ; 
			return pushed ; 
		}
		return 0 ; 
	}
	int maxFlow(){
		int flow = 0 ; 
		while(bfs()){
			fill(cur.begin(), cur.end(), 0) ; 
			while(int pushed = dfs(s, inf)){
				flow += pushed ; 
			}
		}
		return flow ; 
	}
};

struct Task{
	int u , cu , v, cv , id ;
};
const int N = 25 ; 
int color[N] ;
vector<vector<pair<int, int>>>tr[N << 1] ; 
int dep[N] ; 
int n ; 
int node(int u , int c) return c*n+u ;
signed main(){
	ios ::sync_with_stdio(false) ; 
	cin.tie(0) ; 
	int tc ; cin >> tc ; 
	while(tc--){
	    cin >> n ; 
		for(int i=1 ;i <= n ;i++){
			char ch ; cin >> cj ; 
			if(ch == 'R') color[i] = 0 ; 
			else if(ch == 'G' ) color[i] = 1 ; 
			else color[i] = 2 ; 
		}
		for(int id =1 ; id <n ; id++){
			int u , v ; cin >> u >> v ; 
			tr[u].push_back({v, id}) ; 
			tr[v].push_back({u , id}) ; 
		}
		memset(dep , sizeof(dep) , -1) ; 
		queue<int>q ; 
		dep[1] = 0 ; 
		q.push(1) ; 
		while(!q.empty()){
			int u= q.front() ; 
			q.pop() ; 
			for(auto [v, id] :tr[u]){
				if(dep[v] !=-1) continue ;
				depth[v] = depth[u] ^ 1 ; 
				q.push(v) ;
			}
		}
		vector<array<int,3>>need(n+1) ; 
		for(int u =1 ; u<=n ;u++){
			int deg = (int)tree[u].size() ; 
			for(int c =0 ; c<3 ;c++){
				need[u][c] = deg/3 ; 
			}
			for(int j = 0 ; j < deg%3 ;j++) ++need[u][(color[u]+j)%3] ;
		}
		int source= 0 , sink =3 *n+1 ;
		Dinic dinic(sink+1,source,sink) ; 
		for(int u=1 ;u <= n; u++){
			if(!dep[u]){
				for(int c:{0,1,2}) dinic.add(source,node(u,c),need[u][c]);
				for(auto [v,id]:tr[u]){
					for(int cv :{0,1,2}) for(int cu:{0,1,2}){
						if(cu==cv) continue ; 
						dinic.add(node(u,cu),node(v,cv),1,id) ; 
					}
				}
			}
			else{
				for(int c:{0,1,2}) dinic.add(node(u,c),sink,need[u][c]) ; 
			}
		}
		if(dinic.maxFlow()!=n-1){
			cout<< "No" <<endl ; 
			continue ; 
		}
		vector<Task>task;
		for(int u=1 ; u<=n; u++){
			if(depth[u]!=0) continue ; 
			for(int cu:{0,1,2}){
				int state = node(u,cu) ; 
				for(auto &e : dinic.g[state]){
					if(e.id == 0|| e.cap !=0) continue ; 
					int v = (e.to-1)%n+1 ; 
					int cv = (e.to-1)/n ; 
					task.push_back({u,cu,v,cv,e.id}) ; 
				}
			}
		}
		vector<int>vis(n,0) , ans ; 
		for(int i =1; i<n; i++){
			bool fl = 1 ; 
			for(Task &t : task){
				if(vis[t.id]) continue ;
				if(color[t.u] ==t.cu &&color[t.v]==t.cv){
					if(vis[t.id]) continue ; 
					ans.push_back(t.id) ; 
					color[t.u]=(color[t.u]+1)%3 ; 
					color[t.v]=(color[t.v]+1)%3 ;
					fl=1 ;
					break ; 
				}
			}
		}
		cout<<"Yes"<<endl; 
		for(int i= 0 ;i<ans.size();i++)cout<<ans[i]<<" " ; 
		cout<<endl ;
	}
	
}



