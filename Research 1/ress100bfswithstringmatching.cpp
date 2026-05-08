#include<bits/stdc++.h>

using namespace std ; 

/*Algorightms : we have two type : 1. the string matching as the KMP , 2.the string is the prefix of the  final string t, when we meet the case 2 , we want our next char of the string is '0' */

int t, n ,k ;
char s[205] ;

int nxt[205] ;
int match[205][2] , acc[205][2] ;
int dep[205];

struct Trans{
	int ch ;   // letter add is 0 / 1
	int lp , lc, lk , lok ;
}dp[205][85][3005][2];

bool vis[205][85][3005][2];

queue<tuple<int,int,int,int>>q;



void precal(){
	nxt[1] = 0 ;
	dep[1] =(n!=1);
	// precalculate KMP
	for(int i=2 ; i<=n ;i++){
		int j =  0;
		while(j!=0 &&s[i]!=s[j+1]) j= nxt[j];
		if(s[i]==s[j+1]) j++;
		nxt[i]=j;
		dep[i]=dep[j]+(i!=n);
	}
	//precalculate match and acc
	/*match [i][j] mean : add char j at the index i add ? value */
	for(int i =0; i<=n;i++){
		for(int j=0; j<=1;j++){
			int pos = i;
			int cnt = 0 ;
			while(pos!=0 &&s[pos+1]!=j+'0') pos= nxt[pos];
			if(s[pos+1]==j+'0') pos++;
			match[i][j]=pos;
			pos = i;
			while(pos!=0){
				cnt+=(s[pos]=='0');
				pos = nxt[pos];
			}
			cnt+= s[1]=='1';
			acc[i][j] = cnt;
		}
	}
}

void NO(){
	cout << "Impossible";
	exit(0) ;
}


signed main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> t ;
	while(t--){
		cin >> n >> k >> s +1;
		precal();
		memset(vis,0,sizeof(vis));
		q.push({0,0,0,0,0});
		vis[0][0][0][0]=1;
		dp[0][0][0][0] ={0,0,0,0,0};
		bool fl =0;
		while(!q.empty()){
			auto [p,c,lk,ok] =q.front();
			if(ok &&  (lk == k)){
				st = {p , c , lk , ok};
				fl=1;
				break ;
			}
			for(int cc = 0 ; cc <2 ;cc++){
				int np = match[p][cc];
				int nc = c + (cc == '0')*acc[p][cc];
				int nk = lk+dep[np]+nc;
				int nok = ( p == n) || ok ;
				if(nk <=k){
					if(!vis[np][nc][nk][nok]){
						vis[np][nc][nk][nok]=1;
						dp[np][nc][nk][nok]={cc,p,c,lk,ok};
					}
					q.push({np,nc,nk,nok});
				}
			}
		}
		if(!fl){
			NO();
		}
		auto[p,c,k,ok]=st;
		string ans ="";
		while(p!=0 || c!=0 || k!=0 || ok!=0){
			auto[ch,np,nc,nk,nok]=dp[p][c][k][ok];
			p = np, c = nc , k = nk , ok = nok;
			ans += ch + " ";
		}
		cout << ans << endl;
	}
}









