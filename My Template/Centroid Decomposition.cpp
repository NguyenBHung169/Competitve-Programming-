struct Centroid {
    int sz[N], mx[N];
    int all, best, cen;

    void dfs_sz(int u, int p) {
        sz[u] = 1;
        mx[u] = 0;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs_sz(v, u);
            sz[u] += sz[v];
            mx[u] = max(mx[u], sz[v]);
        }
        mx[u] = max(mx[u], all - sz[u]);
        if (mx[u] < best) {
            best = mx[u];
            cen = u;
        }
    }

    int get_centroid(int u, int tot) {
        all = tot;
        best = tot;
        cen = u;
        dfs_sz(u, 0);
        return cen;
    }

    // Sau khi đã chọn được centroid x, tính size các cây con của nó
    void dfs_real_sz(int u, int p) {
        sz[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs_real_sz(v, u);
            sz[u] += sz[v];
        }
    }
} CenDe;
