#include<bits/stdc++.h>

using namespace std;


#define C 26
#define N 55

int n , k , len ;
char s[N << 1] ;

int pre[N] , nxt[N][C] , mn[N] , ret[N] ;

int f[2][N][N] ;
int dp[N][N];


bool check(){
	for(int i = 0 ; i < len ; i++) s[i+len] = s[i];
	for(int i = 1 ; i < len ; i++){
		int j = 0 ;
		while(j < len && s[st+ j]  == s[j]) j++ ;
		if(j == len || s[st+j] < s[j]) return false;
	}
	return true ;
}

bool work(){
	while(len && s[len-1]=='z') len--;
	if(len == 0) return false;
	s[len-1]++;
	return true;
}

void kmp(){
	memset(nxt[0] , 0 ,sizeof(nxt[0]));
	pre[0]=-1;
	mn[0] = 0;
	for(int i = 0 ; i<len;i++){
		int c = s[i]-'a';
		if(c < mn[i]) return 0;
		mn[i] =c;
		nxt[i][c]=i+1;
		if(pre[i]==-1) pre[i+1]= 0;
		else pre[i+1]=nxt[pre[i]][c];
		mn[i+1] = mn[pre[i+1]];
		for(int j = 0 ; j < 26 ; j++) nxt[i+1][j] = nxt[pre[i+1]][j];
	}
}

void get(){
	kmp();
	for(int i = 0 ; i<= len;i++){
		ret[i]= 0;
		int st = i ; 
		for(int j = 0 ; j<len; j++){
			int c = s[j]-'a';
			if(c < mn[st]){
				ret[i] = -1;
				break;
			}
			st = nxt[st][c];
			if(st == len) ret[i]++;
		}
	}
	memset(f,0,sizeof(f));
	memset(dp,0,sizeof(dp));
	f[len&1][pre[len]][0] = 1;
	for(int i = len ; i<= n ;i++){
		memset(f[(i+1)&1] , 0 ,sizeof(f[(i+1)&1]));
		for(int j = 0 ; j<len;j++){
			for(int k = 0 ; k + len <= n; k++){
				if(ret[j] >=0){
					dp[i][k+ret[j]] += f[i&1][j][k];
				}
				if(i==n) continue ;
				if(f[i&1][j][k] ==0) continue;
				if(j+1== len){
					f[(i+1)&1][pre[len]][k+1] += f[i&1][j][k];
				}
				else f[(i+1)&1][j+1][k] += f[i&1][j][k];
				
				f[(i+1)&1][0][k] += f[i&1][j][k] * (26-mn[i]-1);
			}
		}
	}
	for(int i = pre[len]; i!=0 ; i - pre[i]){
		int period = len - i ;
		if(ret[period] >= 0){
			for(int j = 2 ; j *period <=n ; j++){
				dp[i][j] -= dp[i*j][j];
			}
		}
	}
}









