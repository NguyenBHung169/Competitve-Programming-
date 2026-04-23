#include<bits/stdc++.h>


using namespace std ;

const int N = 27 ;

int n , c , cnt; 
int w[N][N]  ; //w[a][b] : cost of translate 'a' to 'b'
int nxt[N] ;   // nxt[u] = v : v is the smallest cost from u to in DAG
int b[N] ;  // number of cycle with length > 1
int in[N] ;
int W[N][N] ; // number of index i with s[i] = u and t[i] = v

char s[1e5 + 3], t[1e5 + 3] ;
/*dp[i][S][flag] : min cost of having desired the out-edge for the 1..i vertex and S 
with S is the set of the vertex have broken its cycle, flag =0/1 check if the edge we change 
is a egde in DAG pastly*/
int dp[N][1 << 13][2] ;
 // int ans ;
 int flag = 1 ;
int trans(char x){
	return (x) - 'a' + 1 ;
}
#define L(i , a , b) for(int i = (a) ; i < (int)(b) ; i++)
void topo(){
	L(i, 1, 27) in[nxt[i]] += 1 ;
	queue<int>q;
	L(i, 1, 27){
		if(!in[i]) q.emplace(i), flag = 0 ;
	}
	while(q.size()){
		int f = q.front() ; 
		q.pop() ;
		if((--in[nxt[f]]) == 0){
			q.emplace(nxt[f]) ;
		}
	}
	L(i, 1, 27){
		if(in[i] && nxt[i] != i){
			cnt += 1 ;
			int now = i ;
			while(in[now]){
				in[now] = 0 ; 
				b[now] = cnt ; 
				now = nxt[now] ;
			}
		}
	}
	// if no cycle , greedy take the cost
	if(cnt == 0){
		int sum = 0 ; 
		L(i, 1, 27){
			sum += w[i][nxt[i]] ; 
		}
		cout << sum << endl ;
		exit(0) ;
	}
}
#define INF 0x3f3f3f3f
signed main(){
	ios :: sync_with_stdio(false);
	cin.tie(0) ;
	
	cin >> n >> c ;
	cin >> s + 1 >> t + 1 ;
	L(i, 1, n+1 ){
		W[trans(s[i])][trans(t[i])] += 1 ;
	}
	L(i, 1, 27){
		w[i][0] = INF ; 
		int add = 0 ;
		L(j, 1, 27) add += W[i][j] ;
		L(j, 1, 27){
			w[i][j] = add - W[i][j] + (i != j ? c : 0) ;
			if(w[i][j] < w[i][nxt[i]]) nxt[i] = j ;
		}
		
	}
	topo() ;
	
	memset(dp, INF + 20 , sizeof(dp)) ; // initialize value of DP for taking the min cost 
	dp[0][0][0] = 0 ; 
	L(i, 1, 27){
		L(S , 0 , (1 << cnt)){
			L(t, 0 , 2){
				L(j, 1, 27){
					auto T = S ;   // the next state
					// if i in a cycle, we don't use nxt[i] , so its cycle broken
					if(b[i] && nxt[i] != j ){
						T |= (1 << (b[i] -1)) ; // add the index cycle of i to the ban-masked
					}
					// if j in a cycle , choose i->j make the cycle broken
					if(b[j] && (nxt[i] != j || b[i] != b[j])){
						T |= (1 << (b[j] -1));   // add the id cycle of j 
					}
					// change the flag t because do not choose the cycle-edge
					dp[i][T][t | (nxt[i] != j)] = min(dp[i][T][t | (nxt[i] != j)], dp[i-1][S][t] + w[i][j]);
				}
			}
		}
	}
	int ans = INT_MAX ; 
	// Loop all state of broken vertex apply to broken cycle , if in the mask S have cycle that not broken , we pay c*number_of_unbroken
	L(S , 0 , (1 << cnt)){
		L(t , flag , 2) ans = min(ans , dp[26][S][t] + c *(cnt - __builtin_popcount(S)));
	}
	cout << ans << endl ;
}





