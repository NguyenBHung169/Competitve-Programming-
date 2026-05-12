#include <bits/stdc++.h>
using namespace std;

const int N = 200000 + 5;

struct DSU {
    int fa[N], sz[N];
    int comp;

    // Khởi tạo n node, mỗi node là một component riêng
    void init(int n) {
        comp = n;

        for (int i = 1; i <= n; i++) {
            fa[i] = i;
            sz[i] = 1;
        }
    }

    // Tìm root của x
    // Có path compression để tối ưu
    int find(int x) {
        if (fa[x] == x) return x;
        return fa[x] = find(fa[x]);
    }

    // Kiểm tra u và v có cùng component không
    bool same(int u, int v) {
        return find(u) == find(v);
    }

    // Gộp component chứa u và component chứa v
    // return true nếu gộp thành công
    // return false nếu đã cùng component
    bool unite(int u, int v) {
        u = find(u);
        v = find(v);

        if (u == v) return false;

        // Gộp cây nhỏ vào cây lớn
        if (sz[u] < sz[v]) swap(u, v);

        fa[v] = u;
        sz[u] += sz[v];
        comp--;

        return true;
    }

    // Size của component chứa x
    int size(int x) {
        x = find(x);
        return sz[x];
    }
};
