#include<bits/stdc++.h>


using namespace std ; 

#define L(i , a , b) for(int i  =(a) ; i < (int)(b) ; i++)
#define R(i , a,  b) for(int i  =(a) ; i > (int)(b) ; i--)
#define ld long double 
#define all(x) (x).begin() , (x).end()

const ld INF  = 1e9  ;
const ld  EPS  = 1e-6 ; 

struct C{
	int t , dt ;
};

struct D{
	int w , t , dt ;
};

struct Line{
	ld k , b ;   // y = kx + b 
};

ld f(Line line , ld x){
	return line.k * x + line.b ;
}
// sort C foods by t-value
bool cmp(C a, C b){
	return a.t > b.t ;
}

// action for slope trick on D foods , check if l2 can be ignored 
bool bad(Line l1 , Line l2, Line l3){
	return (l2.b - l1.b) *(l1.k - l3.k ) >= (l3.b - l1.b) *(l1.k - l2.k) - EPS ;
}
// action for C foods when t-value of two type are equal , we merge their dt-value for easily execute 
ld merge_dt(ld a , ld b){
	if(fabsl(a) < EPS || fabsl(b) < EPS) return 0.0L ;
	return 1.0L/(1.0L/a + 1.0L/b) ;
}
// 
ld gain(ld a, ld b , ld c){
	return a * c - b * c * c / 2.0L ;
}

signed main(){
	ios :: sync_with_stdio(false) ; 
	cin.tie(0) ;
	int n , W ; cin >> n >> W ; 
	
	vector <C> c ;  // continous foods 
	vector <D> d ;  // discrete foods
	L(i , 0 , n){
		char type ; 
		cin >> type ; 
		if(type == 'C'){
			int t , dt ; 
			cin >> t >> dt ; 
			c.push_back({t, dt}) ;
		}
		else{
			int w , t , dt ;
			cin >> w >> t >> dt ;
			d.push_back({w , t , dt}) ;
		}
	}
	
	// Work on C foods 
	vector<ld>dpC(W+1, -INF) ;  // dpC[x] : best value using x C foods
	dpC[0] = 0; 
	sort(all(c) , cmp);  // desc sort
	ld cur_t = c[0].t ;   // current t
	ld cur_dt = c[0].dt;  // current dt 
	ld eaten = 0;     // current value eaten 
	ld val = 0 ;    // greedy best value 
	int pt = 1 ;
	L(i, 1 ,W+1){
		while(eaten + EPS < (ld)i){
			if(pt == (int)c.size()){
				val += gain(cur_t, cur_dt , i-eaten) ;
				cur_t -= cur_dt*(i-eaten) ;
				eaten = i  ;
				break ;
			}
			ld next_t = c[pt].t  ,  next_dt = c[pt].dt ;   // t , dt value of the next C foods may be can merge when the cur_t value we use equal to next_t value
			if(fabsl(cur_dt) < EPS || cur_t - cur_dt*(i - eaten) > next_t + EPS){
				val += gain(cur_t, cur_dt , i-eaten) ;
				cur_t -= cur_dt*(i-eaten) ;
				eaten = i ;
			}
			else{
				// the amount eat for cur and next equal at t-value
				ld ins = max(0.0,(cur_t - next_t) / cur_dt) ;
				val += gain(cur_t, cur_dt , ins) ;
				
				eaten += ins ; 
				cur_t =  next_t;
				// eat when two t_value equal 
				// after that merge two dt_value
				cur_dt=merge_dt(cur_dt , next_dt) ;
				
				pt += 1 ; 
			}
		}
		dpC[i] = val ;
	}
	
	
	// Work on D foods 
	vector<ld>dpD(W+1, -INF) ;   //  dpD[x] : best value using x D foods
	dpD[0] = 0 ; 
	for(auto [w , t, dt] : d){
		L(i , 0 , w+1){
			if(i > W) break ;
			int mx = (W - i) / w ;
			vector<ld> g( mx + 1 , -INF ) ;
		    L(j , 0 , mx){
			   g[j] = dpD[w * j + i] ;   // g[j] : old value at w * j + i 
		    } 
			deque<Line>dq ;   // deque for slope trick 
			auto make_line  = [&](int x) -> Line{
				ld k = dt * x ;
				ld b = g[x] -  t * x - dt * x * (x+1) / 2.0L ;
				return { k , b} ;
			};
			auto value = [&](int x) {
				return t * x - dt * x * (x - 1) / 2.0L ;
			};
			
			L(k , 0 , mx+1){
				if(g[k] >= -INF / 2){
					Line cur = make_line(k) ; 
					while(!dq.empty() && fabsl(dq.back().k - cur.k) < EPS){
						if(dq.back().b  >= cur.b - EPS) cur = dq.back() ;
						dq.pop_back() ;
					}
					while((int)dq.size() >=2 && bad(dq[(int)q.size()-2], dq.back(), cur)) dq.pop_back();
					dq.push_back(cur) ;
				}
				if(dq.empty()){
					dpD[w*k + i] = -INF ;
					continue ;
				}
				dpD[w * k + i] = value(k) + f(dq.front(), k) ;
			}
		
		}
	
	}
	int ans = -INF ;	
	L(i, 0, W+1){
		if(dpC[i] == -INF || dpD[W - i] == -INF) break ;
		ans   = max(ans, dpC[i] + dpD[W-i]) ;	 // ans = best value when we use x C foods and W - x D foods
	}
	cout << fixed << setprecision(10) << (ld)ans << endl ;
}




// Algorithms : 




/* Seperate 2 types : Discrete foods and Continous foods 
1. Continous foods : 
   - F(x) = t*w - dt*w*w / 2 . This F hard to execute , so we exchange to F' = t - dt*w . Realize that after eat w gram , the value t descrease to 
t- dt*w .
   - So we greedily eat the C food that have the biggest t-value 
   - But when two food have equal t-value . It seem hardly to adjust . Example :
     + F1 :  t = 8 , dt = 1/3 
     + F2 :  t = 8 , dt = 1/2 
     when we eat F1 , F1.t < F2.t 
     when we eat F2 after that , F2.t < F1.t 
    + it seem hardly when work seperately on them 
    - so we merge those dt-value to a new dt-value to keep their t-values equal at all time

2. Discrete foods :
   - Realize that after eating k a D foods , we earn : k * t-value - dt * k * (k-1) / 2 
   - We have a dp :   dp[i] = max j <= i {(old_dp[j] + (k)*t-value - dt*k*(k-1)/2} with k = i-j 
   - Translation : dp[i] = i * t - dt * i * (i-1) / 2 + max j<=i {old_dp[j] -j*t -dt*j*(j+1) + dt *i*j}
   - Observing the translation :
      +we see the first part : i*t-dt*i*(i-1)/2 can deal easily
      +we see the second part : max j<=i {....} is a type of convex hull trick : with the type kx + b with 
    k = dt*i  , x = j , b =old_dp[j] -t*j - dt*j*(j+1)/2
3. Combine :
   - Having max value of x gram C food , and D food , we combine them by taking the max of taking x gram C foods and W - x gram D foods , so we have the best value
of take enough W gram of both types */
