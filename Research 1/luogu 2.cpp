#include<bits/stdc++.h>


using namespace std ; 


#define L(i, a, b) for(int i = (a) ; i<(int)(b) ; i++)
#define R(i, a, b) for(int i=(a) ; i>(int)(b) ; i--)
#define all(x) (x).begin() , (x).end()
const int INF = 1e9 ; 
const int N = 25 ;

int dx[4] = {1, -1, 0, 0} ;
int dy[4] = {0, 0, 1, -1} ;

map<vector<int>, int >dp[N][N] ;    // dp.first : vector state of plug  , dp.second : longest path of now state

int n, m ;
int shortest_path[N][N] ;
int ban[N][N];
void find_shortest(){
	memset(shortest_path,-1,sizeof(shortest_path)) ;
	shortest_path[1][n] = 0 ;
	queue<pair<int, int>>q;
	q.push(make_pair(1,n));
    while(!q.empty()){
    	int x = q.front().first() ; 
    	int y = q.front().second() ;
    	L(i, 0, 4){
    		
    			int nx = x + dx[i];
    			int ny = y + dy[i] ;
    			
    			if(nx <1 || nx > n || ny > n ||ny <1) continue;
    			if(shortest_path[nx][ny] != -1) continue;
    			if(ban[nx][ny]) continue;
    			q.push(make_pair(nx,ny));
    			shortest_path[nx][ny] =shortest_path[x][y] +1;
			
		}
		q.pop() ;
	}
}
//vector<int>plug[25];

vector<int>build_plug(vector<int>&plug){
	int cnt = 1 ;
	int vis[30] ;
	memset(vis,0,sizeof(vis));
	L(i, 1 , n+2){
		if(plug[i] == 0 || plug[i] ==1) continue ;
		if(!vis[plug[i]]){
			vis[plug[i]] = ++ cnt ;
			plug[i] = cnt ;
		}
		else plug[i] = vis[plug[i]];
	}
	return plug ; 
}
vector<int>shift(vector<int>&plug){
	L(i, 1, n+1) plug[i] = plug[i+1] ;
	plug[n+1] = 0 ;
	return plug ;
}
vector<int>transform(vector<int>&plug ,int i){
	swap(plug[i] , plug[i+1]);
	return plug ;
}
vector<int>merge(vector<int>&plug ,int id){
	int p = plug[id] , q = plug[id+1] ;
	int ok = 20 ;
	if(p==1 || q==1) ok =1;
	L(i, 1 , n+2){
		if(plug[i] == p || plug[i] == q) plug[i] = ok;
	}
	plug[id] = plug[id+1] = 0; 
	build_plug(plug) ;   // build plug again after merge
	return plug ;
}

vector<int>create(vector<int>&plug ,int id){
	plug[id] = plug[id+1] = 20 ;
	return build_plug(plug) ;
}

signed main(){
	ios :: sync_with_stdio(false) ;
	cin.tie(0) ;
	cin >> n >> m ;
	memset(ban , 0 , sizeof(ban)) ;
	L(i, 0 , m){
		int x , y ; cin >> x >> y ;
		ban[x][y] = 1 ;
	}
	//build_plug() ;
	find_shortest() ; 
	vector<int>v(n+2 , 0) ; 
	v[n+1] = 1 ;
	dp[1][n-1][v] =1 ;
	v[n+1]=0 , v[n]=1 ;
	dp[1][n-1][v] = 1 ; 
	L(i,1 , n){
		R(j, n -(i==1) , 0){
			for(auto [x, y] : dp[i][j] ){  // y : best value now  , x : state plug  
				int p = x[j] , q = x[j+1] ;
				// !p && !q : no plug , we can skip 
				if(!p && !q){
					dp[i][j-1][x] = max(dp[i][j-1][x] , y) ;
				}
				if(ban[i][j]) continue ;
				// if no plug but not ban
				if(!p && !q){
					auto T = create(x , j) ;
					dp[i][j-1][T] = max(dp[i][j-1][T] , y) ;
				}
				// if one of two plug closed
				if((!p) + (!q) == 1){
					// go over (i,j)
					dp[i][j-1][x] = max(dp[i][j-1][x] , y+1) ;
					// exchange the direction 
					auto T = transform(x, j) ;
					dp[i][j-1][T] = max({dp[i][j-1][T] ,  y+1});
				}
			    if(p != q && p!=0 && q!=0){
					auto T = merge(x, j) ;
					dp[i][j-1][T] = max(dp[i][j-1][T] , v+1) ;
				}
			}
		}
		for(auto [x, y] : dp[i][0]){
			if(x[1] == 0){
				dp[i+1][n][shift(x)] = dp[i][0][x] ;
			}
		}
		
	}
	vector<int>way1(n+2) ,way2(n+2) ;
		way1[1]=1 ; 
		way2[2]=1;
		int ans = max(dp[n][1][way1] , dp[n][1][way2]) - shortest_path[n][1] ;
		cout << ans << endl ;
} 
