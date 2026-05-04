#include<bits/stdc++.h>


using namespace std ;


#define N 505
#define M 205

int t , n , m ;
int a[N] , b[M] , px[N]

bitset<M>dp[M][N][4];

vector<pair<int,int>>pre[M][N][4] ;

int lr(int l , int r){
	return px[r] ^ px[l] ;
}
int movel(int l , int r){
	int lhs = n - l +1 , rhs = r -n ;
	if(lhs % 3 == rhs % 3) return -1;
	return lhs + rhs / 2 - (rhs & 1);
}

int mover(int l , int r){
	int lhs = n - l +1 , rhs = r - n ;
	if(lhs % 3 == rhs % 3) return -1;
	return lhs/ 2 + rhs - (lhs &1) ;
}

void trans(int i , int pj , int pk , int nj , int nk){
	if( i == 1){
		dp[i][nj][nk][pj] =1;
		return;
	}
	dp[i][nj][nk] |= dp[i-1][pj-1][pk];
	pre[i][nj][nk].push_back(make_pair(pj , pk));
}

void resetdp(){
	for(int i=1; i<=m ; i++){
		for(int j =1 ; j<=2*n; j++){
			for(int k = 0 ; k < 4 ; k++){
				dp[i][j][k].reset();
				pre[i][j][k].clear() ;
			}
		}
	}
}
vector<int>solve(){
	px[0] = 0 ;
	for(int i = 1; i<=n*2; i++) px[i] =px[i-1] ^ a[i];
	resetdp() ;
	for(int i=1; i<=m ; i++){
		int ll = 1 + (i!=1);
		int rr  = (i==1 ? n+1 : 2*n) ;
		for(int l =ll ; l <= rr ; l++){
			for(int d= 0 ; d < 3; d++){
				for(int r = l +d; r <= (l<=n ? n : n*2); r+=3){
					if(lr(l,r)!=b[i]) continue;
					int len = r-l+1;
					if(l<=n){
						if(len%3==2){ // 3k-1
							trans(i, l ,0 , r , r==n);
							if(i!=1) break;
						}
						else if(len>1)break;
					}
					else{
						if(len%3==1){
							trans(i,l,1,r,2);
							trans(i,l,2,r,3);
							trans(i,l,3,r,3);
							if(i!=1)break;
						}
						else if(len==1){
							trans(i,l,1,r,2);
							trans(i,l,2,r,2);
						}
						else break;
					}
				}
			}
			if(l>n) continue ; // case when over twoside
			for(int d = 0 ; d < 6 ; d++){
				for(int r = n+1+d; r <= l + n -m ; r+=6){
					if(lr(l,r)!=b[i])continue;
					int bl = movel(l,r);
					if(bl==-1)break;
					if(bl%3==2){
						trans(i,l,0,r,1);
						if(i!=1)break;
					}
					else break;
				}
			}
			for(int d= 0 ;d<6;d++){
				for(int r = n+1+d; r<=l+n-m;r+=6){
					if(lr(l,r)!=b[i])continue;
					int br=mover(l,r);
					if(br==-1)break;
					if(br==1){
						trans(i,l,0,r,2);
					}
					else if(br%3==1){
						trans(i,l,0,r,1)
						if(i!=1)break;
					}
					else break;				
				}
			}
		}
	}
	bool flag = 0 ;
	vector<pair<int,int>>seg;
	for(int i = 1; i<=m ; i++){
		int ll =  i + (i!=1);
		int rr = (i==1 ? n+1 : 2*n);
		for(int l =ll ; l <= rr;l++){
			for(int d=0;d<3;d++){
				for(int r = l+d;r<= (l<=n ; n : 2*n);r+=3){
					if(!dp[i][ll][rr]) continue;
					flag=1;
					seg.push_back(make_pair(ll ,rr));
					auto x = pre[i][ll][rr]; 
					int prel = x.first ,prer =x.second;
					while(prel!=-1 || prer !=-1){
						seg.push_back(make_pair(prel,prer));
						auto xx = pre[i][prel][prer];
						prel = xx.first , prer=xx.second;
					}
				}
			}
		}
	}
	if(flag==0){
		return vector<int>{-1};
	}
	vector<int>ans,pre,suf;
	deque<int>dq;
	for(int i = 1 ; i<=m ; i++){
		int l = seg[i-1].first , r = seg[i-1].second;
		if(r<=n) suf.push_back(r-l+1);
		else if(l>n) pre.push-back(r-l+1);
		else{
			int lhs = n-l+1, rhs= r-n; 
			if(mover(l,r)>=movel(l,r)){
				while(lhs>=2){
					lhs-=2;
					rhs++;
					ans.push_back(2);
				}
			}
		}
	}
}



signed main(){
	ios :: sync_with_stdio(false);
	cin.tie(0);
	cin >> t ; 
	while(t--){
		L(i , 0 , n) cin >> a[i] ; 
		L(i , 0,  m) cin >> b[i] ;
		L(i , 0 , n) a[i+n] =a[i];
		auto work = solve(a) ;
		if(work != vector<int>{-1}){
			cout << tot ; 
			for(auto x : ans){
				cout << x << " " ;
			}
		}
		reverse(all(a)) ; 
		reverse(all(b)) ;
		work = solve(a) ;
		if(work != vector<int>{-1}){
			cout << tot ; 
			for(auto x : ans){
				cout << x << " " ;
			}
		}
		cout << -1 ; 
	}
}



