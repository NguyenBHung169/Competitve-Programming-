#include <bits/stdc++.h>
using namespace std;

const int N = 200000 + 5;

struct DSU {
    int fa[N], sz[N];
    int comp;

    struct Save {
        int u, v;       // u bị gắn vào v
        int szv;        // size cũ của v
        int comp;       // số component cũ
        bool merged;    // có thật sự merge không
    };

    vector<Save> hist;

    // Khởi tạo n component
    void init(int n) {
        comp = n;
        hist.clear();

        for (int i = 1; i <= n; i++) {
            fa[i] = i;
            sz[i] = 1;
        }
    }

    // Không path compression
    int find(int x) {
        while (fa[x] != x) {
            x = fa[x];
        }

        return x;
    }

    bool same(int u, int v) {
        return find(u) == find(v);
    }

    // Lưu mốc hiện tại
    int snapshot() {
        return (int)hist.size();
    }

    // Quay lại trạng thái tại mốc snap
    void rollback(int snap) {
        while ((int)hist.size() > snap) {
            Save cur = hist.back();
            hist.pop_back();

            if (!cur.merged) continue;

            fa[cur.u] = cur.u;
            sz[cur.v] = cur.szv;
            comp = cur.comp;
        }
    }

    // Gộp hai component
    bool unite(int u, int v) {
        u = find(u);
        v = find(v);

        if (u == v) {
            hist.push_back({0, 0, 0, comp, false});
            return false;
        }

        // Gộp nhỏ vào lớn
        if (sz[u] > sz[v]) swap(u, v);

        // Lưu lại trạng thái trước khi gộp
        hist.push_back({u, v, sz[v], comp, true});

        fa[u] = v;
        sz[v] += sz[u];
        comp--;

        return true;
    }

    int size(int x) {
        x = find(x);
        return sz[x];
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    DSU dsu;
    dsu.init(n);

    int snap = dsu.snapshot();

    dsu.unite(1, 2);
    dsu.unite(2, 3);

    cout << dsu.same(1, 3) << '\n'; // 1

    dsu.rollback(snap);

    cout << dsu.same(1, 3) << '\n'; // 0

    return 0;
}
