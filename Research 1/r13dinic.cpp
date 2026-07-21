#include<bits/stdc++.h>


using namespace std ; 
#define N 100005

int n , m ; 
int top = 1 ; 
int head[N] , in[N] ; 
int s, t , dist[N] ; 
int maxflow = mincost = flow = 0 ; 
struct Node{
	int v , val , next , cost ;
}node[N] ; 

void a(int u , int v , int val , int cost){
	node[++top].v = v ; 
	node[top].val = val ; 
	node[top].cost = cost ; 
	node[top].next = next ; 
	head[u] = top ; 
}

void add(int  u , int v , int val , int cost){
	a(u, v, val , cost) ; 
	a(v, u , 0 , -cost ); 
}

bool spfa(){
	memset(in, 0 ,sizeof(in)) ; 
	memset(dist . 0x3f , sizeof(dist)) ; 
	dist[s] = 0 ; 
	in[s] =1 ;
	queue<int>q ;
	q.push(s) ; 
	while(!q.empty()){
		int u = q.front() ; 
		q.pop(); 
		in[u] = 0 ; 
		for(int i = head[u] ; i ; i = node[i].next ){
			int v = node[i].v ; 
			if(node[i].val && dist[v] > dist[u] + node[i].cost ){
				dist[v] = dist[u] + node[i].cost ; 
				if(!in[v]){
					in[v] = 1 ; 
					q.push(v) ;
				}
			}
		}
	}
	return dist[t] != 0x3f3f3f ; 
}

int dfs(int u , int low){
	if(u == t){
		in[t] = 1; 
		maxflow += low ; 
		return low ; 
	}
	int used = 0 ; 
	in[u] = 1 ; 
	for(int i = head[u] ; i ; i = node[i].next){
		int v = node[i].v ; 
		if((in[v]== 0 || v== t) && node[i].val == 0 &&dist[v] == dist[u]+ node[i].cost ){
			int pushed = dfs(v, min(low - used , node[i].val));
			if(pushed){
				mincost += node[i].cost * pushed ; 
				node[i].val -= pushed ; 
				node[i^1].val += pushed ; 
				used += pushed;
			}
			if(used == low) break ; 
		}
	}
	return used ; 
}

int Dinic(){
	while(spfa()){
		in[t] = 1 ; 
		while(in[t]){
			memset(in , 0 , sizeof(in)) ; 
			dfs(s, 0x3f3f3f3f) ; 
		}
	}
	return maxflow ; 
}

bool check(int i , int j){
	return i > 0 && i <= n && j > 0 && j <= m ;
}

int up(int i , int j){
	return 4 *((i-1)*m+j)+0 ;
}
int down(int i , int j){
	return 4 *((i-1)*m+j)+1;
}
int left(int i , int j){
	return 4*((i-1)*m+j)+2 ; 
}
int right(int i , int j){
	return 4*((i-1)*m+j)+3 ; 
}
 int i , j , mask , color  = 0;
signed main(){
	ios::sync_with_stdio(false) ; 
	cin.tie(0) ; 
	cin >> n >> m ; 
	s = 10010 , t = 10011 ; 
	for(int i = 1 ; i <= n ; i++){
		color = i & 2 ; 
		for(int j = 1 ; j <= m  ; j++){
			cin >> mask ;
			color ^= 1 ; 
			if(color == 0){
				if(check(i, j-1)) add(left(i , j) ,roght(i,j-1),1,0);
				if(check(i , j+1)) add(right(i,j) , left(i,j+1),1,0);
				if(check(i-1,j)) add(up(i,j),down(i-1,j),1,0);
				if(check(i+1,j)) add(down(i,j) , up(i+1,j),1,0);
			}
			if (mask == 1) {
                realflow++;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);              // Giữ nguyên hướng lên.
                    add(up(i, j), right(i, j), 1, 1);   // Xoay 1 lần thành phải.
                    add(up(i, j), left(i, j), 1, 1);    // Xoay 1 lần thành trái.
                    add(up(i, j), down(i, j), 1, 2);    // Xoay 2 lần thành xuống.
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(right(i, j), up(i, j), 1, 1);
                    add(left(i, j), up(i, j), 1, 1);
                    add(down(i, j), up(i, j), 1, 2);
                }

                continue;
            }

            // mask = 2 = 0010: chỉ có đầu nối hướng phải.
            if (mask == 2) {
                realflow++;

                if (colour == 0) {
                    add(s, right(i, j), 1, 0);
                    add(right(i, j), up(i, j), 1, 1);
                    add(right(i, j), down(i, j), 1, 1);
                    add(right(i, j), left(i, j), 1, 2);
                }

                if (colour == 1) {
                    add(right(i, j), t, 1, 0);
                    add(up(i, j), right(i, j), 1, 1);
                    add(down(i, j), right(i, j), 1, 1);
                    add(left(i, j), right(i, j), 1, 2);
                }

                continue;
            }

            // mask = 3 = 0011: chữ L gồm hướng lên và phải.
            if (mask == 3) {
                realflow += 2;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);
                    add(s, right(i, j), 1, 0);

                    // U+R -> D+R hoặc U+L; dùng cả hai cạnh là xoay 180 độ.
                    add(up(i, j), down(i, j), 1, 1);
                    add(right(i, j), left(i, j), 1, 1);
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(right(i, j), t, 1, 0);
                    add(down(i, j), up(i, j), 1, 1);
                    add(left(i, j), right(i, j), 1, 1);
                }

                continue;
            }

            // mask = 4 = 0100: chỉ có đầu nối hướng xuống.
            if (mask == 4) {
                realflow++;

                if (colour == 0) {
                    add(s, down(i, j), 1, 0);
                    add(down(i, j), right(i, j), 1, 1);
                    add(down(i, j), left(i, j), 1, 1);
                    add(down(i, j), up(i, j), 1, 2);
                }

                if (colour == 1) {
                    add(down(i, j), t, 1, 0);
                    add(right(i, j), down(i, j), 1, 1);
                    add(left(i, j), down(i, j), 1, 1);
                    add(up(i, j), down(i, j), 1, 2);
                }

                continue;
            }

            // mask = 5 = 0101: ống thẳng trên-dưới, không được xoay.
            if (mask == 5) {
                realflow += 2;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);
                    add(s, down(i, j), 1, 0);
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(down(i, j), t, 1, 0);
                }
            }

            // mask = 6 = 0110: chữ L gồm phải và xuống.
            if (mask == 6) {
                realflow += 2;

                if (colour == 0) {
                    add(s, right(i, j), 1, 0);
                    add(s, down(i, j), 1, 0);
                    add(right(i, j), left(i, j), 1, 1);
                    add(down(i, j), up(i, j), 1, 1);
                }

                if (colour == 1) {
                    add(right(i, j), t, 1, 0);
                    add(down(i, j), t, 1, 0);
                    add(left(i, j), right(i, j), 1, 1);
                    add(up(i, j), down(i, j), 1, 1);
                }
            }

            // mask = 7 = 0111: chữ T gồm lên, phải, xuống; thiếu trái.
            if (mask == 7) {
                realflow += 3;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);
                    add(s, right(i, j), 1, 0);
                    add(s, down(i, j), 1, 0);

                    // Xoay một lần: thay U hoặc D bằng L.
                    // Xoay hai lần: thay R bằng L.
                    add(up(i, j), left(i, j), 1, 1);
                    add(down(i, j), left(i, j), 1, 1);
                    add(right(i, j), left(i, j), 1, 2);
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(right(i, j), t, 1, 0);
                    add(down(i, j), t, 1, 0);
                    add(left(i, j), up(i, j), 1, 1);
                    add(left(i, j), down(i, j), 1, 1);
                    add(left(i, j), right(i, j), 1, 2);
                }
            }

            // mask = 8 = 1000: chỉ có đầu nối hướng trái.
            if (mask == 8) {
                realflow++;

                if (colour == 0) {
                    add(s, left(i, j), 1, 0);
                    add(left(i, j), up(i, j), 1, 1);
                    add(left(i, j), down(i, j), 1, 1);
                    add(left(i, j), right(i, j), 1, 2);
                }

                if (colour == 1) {
                    add(left(i, j), t, 1, 0);
                    add(up(i, j), left(i, j), 1, 1);
                    add(down(i, j), left(i, j), 1, 1);
                    add(right(i, j), left(i, j), 1, 2);
                }
            }

            // mask = 9 = 1001: chữ L gồm lên và trái.
            if (mask == 9) {
                realflow += 2;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);
                    add(s, left(i, j), 1, 0);
                    add(up(i, j), down(i, j), 1, 1);
                    add(left(i, j), right(i, j), 1, 1);
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(left(i, j), t, 1, 0);
                    add(down(i, j), up(i, j), 1, 1);
                    add(right(i, j), left(i, j), 1, 1);
                }
            }

            // mask = 10 = 1010: ống thẳng trái-phải, không được xoay.
            if (mask == 10) {
                realflow += 2;

                if (colour == 0) {
                    add(s, left(i, j), 1, 0);
                    add(s, right(i, j), 1, 0);
                }

                if (colour == 1) {
                    add(left(i, j), t, 1, 0);
                    add(right(i, j), t, 1, 0);
                }

                continue;
            }

            // mask = 11 = 1011: chữ T gồm lên, trái, phải; thiếu xuống.
            if (mask == 11) {
                realflow += 3;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);
                    add(s, left(i, j), 1, 0);
                    add(s, right(i, j), 1, 0);

                    add(left(i, j), down(i, j), 1, 1);
                    add(right(i, j), down(i, j), 1, 1);
                    add(up(i, j), down(i, j), 1, 2);
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(left(i, j), t, 1, 0);
                    add(right(i, j), t, 1, 0);
                    add(down(i, j), left(i, j), 1, 1);
                    add(down(i, j), right(i, j), 1, 1);
                    add(down(i, j), up(i, j), 1, 2);
                }
            }

            // mask = 12 = 1100: chữ L gồm xuống và trái.
            if (mask == 12) {
                realflow += 2;

                if (colour == 0) {
                    add(s, left(i, j), 1, 0);
                    add(s, down(i, j), 1, 0);
                    add(left(i, j), right(i, j), 1, 1);
                    add(down(i, j), up(i, j), 1, 1);
                }

                if (colour == 1) {
                    add(left(i, j), t, 1, 0);
                    add(down(i, j), t, 1, 0);
                    add(right(i, j), left(i, j), 1, 1);
                    add(up(i, j), down(i, j), 1, 1);
                }
            }

            // mask = 13 = 1101: chữ T gồm lên, xuống, trái; thiếu phải.
            if (mask == 13) {
                realflow += 3;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);
                    add(s, down(i, j), 1, 0);
                    add(s, left(i, j), 1, 0);

                    add(up(i, j), right(i, j), 1, 1);
                    add(down(i, j), right(i, j), 1, 1);
                    add(left(i, j), right(i, j), 1, 2);
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(down(i, j), t, 1, 0);
                    add(left(i, j), t, 1, 0);
                    add(right(i, j), up(i, j), 1, 1);
                    add(right(i, j), down(i, j), 1, 1);
                    add(right(i, j), left(i, j), 1, 2);
                }
            }

            // mask = 14 = 1110: chữ T gồm trái, xuống, phải; thiếu lên.
            if (mask == 14) {
                realflow += 3;

                if (colour == 0) {
                    add(s, left(i, j), 1, 0);
                    add(s, down(i, j), 1, 0);
                    add(s, right(i, j), 1, 0);

                    add(left(i, j), up(i, j), 1, 1);
                    add(right(i, j), up(i, j), 1, 1);
                    add(down(i, j), up(i, j), 1, 2);
                }

                if (colour == 1) {
                    add(left(i, j), t, 1, 0);
                    add(down(i, j), t, 1, 0);
                    add(right(i, j), t, 1, 0);
                    add(up(i, j), left(i, j), 1, 1);
                    add(up(i, j), right(i, j), 1, 1);
                    add(up(i, j), down(i, j), 1, 2);
                }
            }

            // mask = 15 = 1111: dấu cộng, xoay không làm thay đổi trạng thái.
            if (mask == 15) {
                realflow += 4;

                if (colour == 0) {
                    add(s, up(i, j), 1, 0);
                    add(s, right(i, j), 1, 0);
                    add(s, left(i, j), 1, 0);
                    add(s, down(i, j), 1, 0);
                }

                if (colour == 1) {
                    add(up(i, j), t, 1, 0);
                    add(down(i, j), t, 1, 0);
                    add(left(i, j), t, 1, 0);
                    add(right(i, j), t, 1, 0);
                }
            }

            // mask = 0 không có đầu nối nên không cần thêm cạnh nào.
        }
    }
    Dinic() ; 
    if(maxflow *2 != flow){
    	cout << -1 << endl ; 
	}
	else{
		cout << mincost <<endl ; 
	}
		
	
}









