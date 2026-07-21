#include<bits/stdc++.h>

using namespace std ; 

#define N  100005
#define eps 1e-9 
int N , A , B , C , st, ed , M ; 

#define ld long double
struct Point{
	ld x , y ; 
	Point() = default ; 
	Point(int _x , int _y) : x(_x) , y(_y) {}
	Point(ld _x , ld _y) : x(_x) : y(_y) {}
}point[N << 1];

struct Ev{
	ld x ; 
	int y0 , y1 ; 
	int op ; 
	bool operator < (const Ev &a) const{
		if(fabs(x-a.x) > eps) return x < a.x ;
		if(op != a.op ) return op > a.op ; 
		retrun false ;
	}
	
};






vector<Point>in ; 
Point inter(Point a, Point b , Point c , Point d){
	ld a1 = b.y -a.y ; 
	ld b1 = a.x - b.x ; 
	ld c1 = a1 * a.x + b1 * a.y ; 
	ld a2 = d.y - c.y ; 
	ld b2 = c.x - d.x ; 
	ld c2 = a2 * c.x + b2 * c.y ; 
	ld deter = a1 *b2 - a2*b1 ; 
	if(!deter){
		return {-1,-1} ; 
	}
	ld x = (b2*c1-b1*c2)/deter ; 
	ld y = (a1*c2 - a2*c1)/deter;
	return {x,y};
}

namespace BIT{
	int N ; 
	int b[N << 2] ; 
	void init(int n){
		 N = n ; 
		 memset(b, 0 ,sizeof(b)) ; 
	}
	int query(int x){
		int res = 0 ; 
		for(int i = x ; i >0 ; i-= i&-i) res += b[i] ; 
		return res; 
	}
	
	void update(int x ,int v){
		for(int i = x ; i <= N ; i+= i&-i) b[i] += v ;
	}
}

bool cmp(int i , int j){
	return point[N + i ].y < point[N + j].y ; 
}

#define all(x) (x).begin() , (x).end()

signed main(){
	ios::sync_with_stdio(false) ;
	cin.tie(0) ; 
	cin >> N >> a >> B >> C >> st >> ed ;
	
	for(int i = 0 ; i < N<<1; i++){
		int x, y ; 
		cin >> y ; 
		x = i < N ? st : ed ;
		point[i] ={x, y} ; 
	}
	vector<int>p(N) , rank(N) , a(N) , v(N) ; 
	
	
	for(int i = 0 ; i <  N; i++){
		a[i] =rank[i] = i ; 
		v[i] = 0 ; 
	}
	sort(all(rank) , cmp) ; 
	for(int i = 0 ; i < N ; i++) p[rank[i]] = i ; 
	int nr = N ; 
	for(int i = 0 ; i < N ; i++){
		if(v[i]!=0) continue ; 
		nr--;
		int x = p[i] ;
		while(x!=i){
			v[x] = 1; 
			x=p[x] ;
		}
	}
	for(int i = 0 ; i < N ; i++){
		for(int j = i ; ; j++){
			if(i==p[j]){
				int aj = a[j] ;
				for(int k = j-1 ; k>=i ;k--){
					int ak = a[k] ;
					Point cur = inter(point[ak] , point[ak+N] , point[aj] , point[aj+N]);
				}
				// rotate 45deg
				in.push_back(Point(cur.x + cur.y , cur.x - cur.y));
				swap(p[k] , p[k+1] );
				a[k+1] = a[k] ;
			}
			break;
		}
	}
	cin >> M ; 
	vector<ld>Y(M<<1+(int)in.size()) ; 
	vector<Ev>event(M<<1+(int)in.size()) ;
	for(int i = 0 , x, y ,r ; i < M ;i++){
		cin >>x>>y>>r ; 
		int x_ = x , y_ = y ; 
		x = x+y ; 
		y = x_ - y_ ;
		event[i] = {x-r , y-r , y +r,1} ; 
		event[i+M] = {x+r , y -r , y+r,-1} ; 
		Y[i] = y-r ; 
		y[i+M] = y+r ; 
	}
	for(int i = 0 ; i < (int)in.size() ; i++){
		Y[N << 1 |1] = in[i].y  ;
	}
	sort(all(Y)) ; 
	int pos=  0 ; 
	for(int i = 0 ; i  < (int)Y.size() ; i++){
		if(!i || fabs(Y[i] - Y[i-1] > eps)){
			Y[pos++] = Y[i] ;
		}
	}
	Y.resize(pos) ; 
	for(int i = 0 ; i < M <<1; i++){
		event[i].y0 = lower_bound(all(Y), event[i].y0) - Y.begin()+1;
		event[i].y1 = lower_bound(all(Y) , event[i].y1) - Y.begin()+1; 
	}
	for(int i = 0 ; i <(int)in.size() ; i++){
		int y = lower_bound(all(Y) , in[i].y) - Y.begin() + 1;
		event[M <<1|1] = {inter[i].x , y} ;
	}
	sort(all(event)) ; 
	int nr2 = 0 ; 
	BIT::init(Y.size()<<1) ; 
	for(int i = 0 ; i <(int)event.size() ; i++){
		Event e = event[i] ; 
		if(e.op) BIT::update(e.y0 , e.op), BIT::update(e.y1+1,-e.op) ; 
		else nr2 += BIT::query(e.yp) > 0 ; 
		
	}
	int nrinv  =(int)in.size() ; 
	int sc2 = nr2*C ; 
	int mx  =A*nrinv ; 
	int mn = A*nr1+B(nrinv - nr2) ;
	if(a < B) swap(mn , mx) ; 
	cout << mn + nr2 << " " << mx +  nr2 << endl ; 
}












