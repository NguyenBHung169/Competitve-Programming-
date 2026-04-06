// Using when only care about the flow amount, faster than EdmondKarp

struct Dinic {
    struct edge {
        int to, rev, id;
        long long flow, w;
    };

    int n, s, t, mxid;
    vector<int> d, done, flow_through;
    vector<vector<edge>> g;

    Dinic() {}

    Dinic(int _n) {
        n = _n + 10;
        mxid = 0;
        g.resize(n);
    }

    void add_edge(int u, int v, long long w, int id = -1) {
        edge a = {v, (int)g[v].size(), id, 0, w};
        edge b = {u, (int)g[u].size(), -2, 0, 0};
        g[u].push_back(a);
        g[v].push_back(b);
        mxid = max(mxid, id);
    }

    // Xây level graph
    bool bfs() {
        d.assign(n, -1);
        d[s] = 0;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) {
                int v = e.to;
                if (d[v] == -1 && e.flow < e.w) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
        return d[t] != -1;
    }

    // Đẩy blocking flow trên level graph
    long long dfs(int u, long long flow) {
        if (u == t) return flow;

        for (int &i = done[u]; i < (int)g[u].size(); i++) {
            edge &e = g[u][i];
            if (e.w <= e.flow) continue;

            int v = e.to;
            if (d[v] != d[u] + 1) continue;

            long long nw = dfs(v, min(flow, e.w - e.flow));
            if (nw > 0) {
                e.flow += nw;
                g[v][e.rev].flow -= nw;
                return nw;
            }
        }
        return 0;
    }

    long long max_flow(int _s, int _t) {
        s = _s;
        t = _t;
        long long flow = 0;
        const long long inf = (long long)4e18;

        while (bfs()) {
            done.assign(n, 0);
            while (long long nw = dfs(s, inf)) {
                flow += nw;
            }
        }

        flow_through.assign(mxid + 1, 0);
        for (int i = 0; i < n; i++) {
            for (auto &e : g[i]) {
                if (e.id >= 0) {
                    flow_through[e.id] = e.flow;
                }
            }
        }
        return flow;
    }
};
