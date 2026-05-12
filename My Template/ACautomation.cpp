#include <bits/stdc++.h>
using namespace std;

const int MAXNODE = 200000 + 5;
const int SIG = 26;

struct AC {
    int tot;
    int nxt[MAXNODE][SIG];
    int fail[MAXNODE];
    int pos[MAXNODE];

    vector<int> failTree[MAXNODE];

    long long cnt[MAXNODE];

    AC() {
        init();
    }

    void init() {
        tot = 1;

        for(int i = 0; i < MAXNODE; i++){
            fail[i] = 0;
            cnt[i] = 0;
            failTree[i].clear();

            for(int c = 0; c < SIG; c++){
                nxt[i][c] = 0;
            }
        }
    }

    int get(char c){
        return c - 'a';
    }

    void ins(int id, const string &s){
        int p = 1;

        for(char c : s){
            int x = get(c);

            if(!nxt[p][x]){
                nxt[p][x] = ++tot;
            }

            p = nxt[p][x];
        }

        pos[id] = p;
    }

    void build(){
        queue<int> q;

        for(int c = 0; c < SIG; c++){
            if(nxt[1][c]){
                fail[nxt[1][c]] = 1;
                q.push(nxt[1][c]);
            }
            else{
                nxt[1][c] = 1;
            }
        }

        while(!q.empty()){
            int u = q.front();
            q.pop();

            for(int c = 0; c < SIG; c++){
                if(nxt[u][c]){
                    fail[nxt[u][c]] = nxt[fail[u]][c];
                    q.push(nxt[u][c]);
                }
                else{
                    nxt[u][c] = nxt[fail[u]][c];
                }
            }
        }

        for(int u = 2; u <= tot; u++){
            failTree[fail[u]].push_back(u);
        }
    }

    void matchText(const string &text){
        int p = 1;

        for(char c : text){
            int x = get(c);
            p = nxt[p][x];
            cnt[p]++;
        }
    }

    void dfsFail(int u){
        for(int v : failTree[u]){
            dfsFail(v);
            cnt[u] += cnt[v];
        }
    }

    long long queryPattern(int id){
        return cnt[pos[id]];
    }
};
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    AC ac;

    for(int i = 1; i <= n; i++){
        string s;
        cin >> s;
        ac.ins(i, s);
    }

    ac.build();

    string text;
    cin >> text;

    ac.matchText(text);
    ac.dfsFail(1);

    for(int i = 1; i <= n; i++){
        cout << ac.queryPattern(i) << '\n';
    }

    return 0;
}
