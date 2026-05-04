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
	 vector<int> ans;
    vector<int> pre, suf;
    deque<int> dq;

    // Dựa trên các khối đã truy vết, trước tiên xử lý các khối bắc qua vòng.
    // Mục tiêu là đưa chúng về hẳn prefix hoặc suffix để sau đó greedy LEFT/RIGHT xử lý.
    for (int i = 1; i <= m; i++) {
        int l = segs[i - 1].first;
        int r = segs[i - 1].second;

        if (r <= n) {
            // Khối nằm hoàn toàn trong suffix ban đầu.
            suf.push_back(r - l + 1);
        } else if (l > n) {
            // Khối nằm hoàn toàn trong prefix sau khi duỗi.
            pre.push_back(r - l + 1);
        } else {
            // Khối bắc qua vòng: gồm đoạn [l,n] và [1,r-n].
            int lhs = n - l + 1;
            int rhs = r - n;

            // Chọn hướng dồn có lợi hơn theo tiêu chí move_r >= move_l giống code gốc.
            if (move_r(l, r) >= move_l(l, r)) {
                // Dồn phần bên trái sang phải bằng thao tác 2.
                while (lhs >= 2) {
                    lhs -= 2;
                    rhs++;
                    ans.push_back(2);
                }

                // Nếu còn lẻ 1 phần tử bên trái thì dùng 1 rồi 2 để chỉnh lại.
                if (lhs == 1) {
                    rhs--;
                    ans.push_back(1);
                    ans.push_back(2);
                }

                pre.push_back(rhs);
            } else {
                // Dồn phần bên phải sang trái bằng thao tác 1.
                while (rhs >= 2) {
                    rhs -= 2;
                    lhs++;
                    ans.push_back(1);
                }

                // Nếu còn lẻ 1 phần tử bên phải thì dùng 2 rồi 1 để chỉnh lại.
                if (rhs == 1) {
                    lhs--;
                    ans.push_back(2);
                    ans.push_back(1);
                }

                suf.push_back(lhs);
            }
        }
    }

    // Sau khi xử lý khối bắc vòng, các độ dài khối còn lại được đưa vào deque.
    // pre nằm phía trước, suf nằm phía sau.
    for (int x : pre) dq.push_back(x);
    for (int x : suf) dq.push_back(x);

    // Gom khối ở bên trái.
    // Với một khối lớn, dùng 1 1 2 để giảm độ dài đi 3.
    // Khi còn 4 thì dùng 1 1 để đưa về 2.
    // Khi còn 2 thì dùng 1 để đưa về 1.
    auto LEFT = [&]() {
        while (!dq.empty() && dq.front() != 1) {
            int x = dq.front();
            dq.pop_front();

            while (x > 4) {
                x -= 3;
                ans.push_back(1);
                ans.push_back(1);
                ans.push_back(2);
            }

            if (x == 4) {
                x = 2;
                ans.push_back(1);
                ans.push_back(1);
            } else {
                x = 1;
                ans.push_back(1);
            }

            dq.push_back(x);
        }
    };

    // Gom khối ở bên phải.
    // Đối xứng với LEFT:
    // dùng 2 2 1 để giảm độ dài đi 3.
    // Khi còn 4 thì dùng 2 2 để đưa về 2.
    // Khi còn 2 thì dùng 2 để đưa về 1.
    auto RIGHT = [&]() {
        while (!dq.empty() && dq.back() != 1) {
            int x = dq.back();
            dq.pop_back();

            while (x > 4) {
                x -= 3;
                ans.push_back(2);
                ans.push_back(2);
                ans.push_back(1);
            }

            if (x == 4) {
                x = 2;
                ans.push_back(2);
                ans.push_back(2);
            } else {
                x = 1;
                ans.push_back(2);
            }

            dq.push_front(x);
        }
    };

    // Theo cấu trúc đồ thị trạng thái đơn giản:
    // chỉ cần quét trái, phải, trái, phải là gom được toàn bộ khối về đúng dạng.
    LEFT();
    RIGHT();
    LEFT();
    RIGHT();

    return ans;
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



