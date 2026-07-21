#include<bits/stdc++.h>

using namespace std ;


#define N 505
#define eps 1e-10
#define pi acos(-1) 

int n , x[N] , y[N] , sq[N] , R ; 
double arc , dist[N] , arg[N] , l[N] , r[N] ; 

int val[N << 2] , lazy[N << 2] ; 
void clear(){
	memset(val , 0 ,sizeof(val)) ; 
	memset(lazy , 0 , sizeof(lazy)); 
}

void tag(int x , int v){
	val[x] += v ; 
	lazy[x] += v ;
}

void pushdown(int x){
	if(lazy[x]){
		tag(x << 1 , lazy[x]) ; 
		tag(x << 1  | 1 , lazy[x]) ; 
		lazy[x] = 0 ;
	}
}

void mdf(int l , int r, int ql , int qr , int x , int v){
	if(ql <= l && r <= qr){
		return tag(x, v) , void() ; 
	}
	int m = (l + r) >> 1 ; 
	pushdown(x) ; 
	if(ql <= m ) mdf(l , m , ql , qr , x << 1 , v) ; 
	if(m < qr) mdf(m+1 , r, ql ,qr , x <<1|1, v) ; 
	val[x] = max(val[x << 1] , val[x << 1 | 1]) ;
}
#define inf 0x3f3f3f3f3f3f
int query(int l , int r, int ql ,int qr , int x){
	if(ql <= l && qr >= r) return val[x] ; 
	int m = l + r >> 1  ; 
	int ans = -inf ; 
	pushdown(x) ; 
	if(ql <= m) ans = query(l , m ,ql ,qr , x << 1) ; 
	if(qr > m) ans = max(ans, query(m + 1, r, ql ,qr ,x << 1|1) ) ;
	return ans ;
}

vector<int>buc[N] ; 


bool legal(double a){
	assert(a >= 0) ;
	
	a -= (int)(a/arc) * arc ; 
	clear() ; 
	int m = n << 1 ; 
	for(int i = 0 ; i < m ; i++) buc[i].clear() ; 
	for(int i = 1 ; i <= n ; i++){
		double x = l[i] - eps , y = r[i]+eps ; 
		int pl = x < a ? 0 : (int)((x-a)/arc)+1 ;
		int pr = y < a ? -1 : (int)((y-a)/arc) ;
		if(pl > pr) return 0 ; 
		buc[pr].push_back(pl) ; 
		if(pr < n) buc[pr+n].push_back(pl + n) ; 
	}
	for(int  i = 0 ; i < m ; i++){
		for(auto it : buc[i]) mdf(0 , m , 0 , it , 1 ,1) ;
		mdf(0, m , 0 , i ,1, -1) ;
		if(query(0, m , i-n+1 , i , 1) > 0) return 0 ; 
	}
	return 1; 
}

bool check(double d){
	for(int i = 1 ; i <= n ; i++){
		if(dist[i] + R - eps <= d) l[i] = 0 , r[i] = 2*pi  - arc ;
		 else{
		 	double a = acos((R* R + sq[i] -d *d) / 2.0 / R /dist[i]) ;
		 	l[i] = ::arg[i] - a , r[i] = ::arg[i] + a ; 
		 	if(l[i] < 0) l[i] += 2*pi , r[i] += a*pi ; 
		 }
		 for(int i =1 ; i <= n l i++){
		 	if(legal(l[i]) || legal[r[i]]) return 1 ; 
		 }
	}
	return  0 ; 
}

signed main(){
	ios::sync_with_stdio(false) ;
	cin.tie(0) ; 
	cin >> n >> R ; 
	arc = 2 * pi / n ; 
	double l = 0 , r = sqrt(2.0) * 200 ; 
	for(int i = 1 ; i <= n ; i++){
		cin >> l[i] >> r[i] ; 
		dist[i] =sqrt(sq[i] = x[i]*x[i] + y[i]*y[i]) ; 
		::arg[i] atan2(y[i] , x[i]);
		if(::arg[i] < 0) ::arg[i] += 2*pi ; 
		l = max(l , fabs(dist[i] - R)+eps) ; 
	}
	for(int _ = 1 ; _ <= 50 ; _++){
		double m = (l + r) /2.0 ; 
		if(check(m)) r = m ; 
		else l = m ; 
	}
	cout << fixed << setprecion(7) << l ; 
}









