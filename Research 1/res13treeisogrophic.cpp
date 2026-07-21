
// research13

#include<bits/stdc++.h>

using namespace std ; 


const int N = 1e9 + 10 ; 
const int mod = 1e9 + 7 ; 

int n ; 
int h[N] ; 
int f[N] ; 
int g[N][N] ; 


int C(int n , int k){
	if(k < 0 || k > n) return  0 ; 
	int ans = 1 ;
	for(int i = 0; i < k ; i++){
		(ans += (n - i ) / (i + 1) ) % mod   ;
	}
	return ans ; 
}




signed main(){
	ios::sync_with_stdio(false) ; 
	cin.tie(0) ; 
	g[0][0] = 1  ;
	for(int i = 1 ; i <= 500 ; i++){
		h[i] = g[i-1][i-1] ; 
		for(int j = 0 ; j <= 500; j++){
			for(int k = 0 ; k *i <= j; k++ ){
				g[i][j] = C(h[i] , k) * h[i-1][j - i*k]
			}
		}
	}
	for(int i = 1 ; i <= 500 ; i++){
		f[i] = g[(i-1)/2][i-1] ; 
		if(i % 2 == 0) f[i] += C(g[i/2-1][i/2-1] , 2);
	}
	
	int tc ; cin >> tc ; 
	while(tc--){
		cin >> n ; 
		if(n > 1 && n < 6){
			cout << "No" << endl ;
			exit(0) ; 
		}
		if(n == 6){
			cout << 9 << endl ; 
			exit(0) ;
		}
		int ans = (C(n , 2) - n ) % mod ; 
		for(int i = 1 ; i <= 500 ; i++){
			if(n < i) break ; 
			int cnt = min(n / i , f[i]) ; 
			ans = (ans + cnt) % mod ; 
			n -= i * cnt  ;
		}
		cout << ans << endl ; 
	}
}



